#include "WsDriveBase.hh"
#include "WsLogger.hh"
#include "WsInputFacade.hh"
#include "WsOutputFacade.hh"
#include "WsDataIndicationFacade.hh"
#include "WsDriveBaseObservers.hh"
#include "WsTypes.hh"

#include <math.h>
#include "Timer.h"


#define WS_HEADING_SENSITIVITY         1.800f // @TODO:  Make this configurable (see B-01123)
#define WS_THROTTLE_ACCEL_FACTOR       0.250f // @TODO:  Make this configurable (see B-01123)
#define WS_HEADING_ACCEL_FACTOR        0.500f // @TODO:  Make this configurable (see B-01123)
#define WS_THROTTLE_INPUT_MAXIMUM      1.000f
#define WS_HEADING_INPUT_MAXIMUM       1.000f
#define WS_ANTI_TURBO_MAX_DEFLECTION   0.500f // @TODO:  Make this configurable (see B-01123)
#define WS_MOTOR_OUTPUT_MAXIMUM        1.000f
#define LOCK_LANDING_GEAR_DEPLOY_TIME  0.040f // (40 msec)   // @TODO:  Remove this code
#define LOCK_LANDING_GEAR_RETRACT_TIME 0.500f // (500 msec)  // @TODO:  Remove this code

WsDriveBase::WsDriveBase(void)
    : WsSubsystem("WsDriveBase")
    , a_driveBaseHeading(0.0f)
    , a_driveBaseThrottle(0.0f)
    , a_landingGearFlag(false)
    , a_shifterFlag(false)
    , a_driveStraightFlag(false)
    , a_quickAutoTurnFlag(false)
    , a_antiTurboFlag(false)
    , a_accelerationEnableFlag(true)
    , a_landingGearOverrideState(WS_DB_LG_OVERRIDE_STATE_NORMAL)
    , ap_landingGearOverrideTimer(NULL)
    , ap_antiturboButtonObserver(NULL)
    , ap_shifterButtonObserver(NULL)
    , ap_landingGearButtonObserver(NULL)
    , ap_landingGearRetractionSwitchObserver(NULL)
    , ap_accelerationEnableFlagObserver(NULL)
{
    WS_LOG_TRACE("Begin");
    
    ap_landingGearOverrideTimer = new Timer();
    ap_landingGearOverrideTimer->Start();
    ap_landingGearOverrideTimer->Reset();
    
    // Make sure the dashboard has the correct default information
    WsDataIndicationFacade::instance()->setDriveBaseAntiTurboEnabled(a_antiTurboFlag);
    WsDataIndicationFacade::instance()->setDriveBaseAccelerationEnabled(a_accelerationEnableFlag);
    
    //
    // The observers must be the last objects created here...
    //
    ap_antiturboButtonObserver             = new WsAntiturboButtonObserver(this);
    ap_shifterButtonObserver               = new WsShifterButtonObserver(this);
    ap_landingGearButtonObserver           = new WsLandingGearButtonObserver(this);
    ap_landingGearRetractionSwitchObserver = new WsLandingGearRetractionSwitchObserver(this);
    ap_accelerationEnableFlagObserver      = new WsAccelerationEnableFlagObserver(this);
    
    WS_LOG_TRACE("End");
}

WsDriveBase::~WsDriveBase(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_landingGearOverrideTimer;              ap_landingGearOverrideTimer = NULL;
    delete ap_antiturboButtonObserver;               ap_antiturboButtonObserver = NULL;
    delete ap_shifterButtonObserver;                 ap_shifterButtonObserver = NULL;
    delete ap_landingGearButtonObserver;             ap_landingGearButtonObserver = NULL;
    delete ap_landingGearRetractionSwitchObserver;   ap_landingGearRetractionSwitchObserver = NULL;
    delete ap_accelerationEnableFlagObserver;        ap_accelerationEnableFlagObserver = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsDriveBase::update(void)
{
    WS_LOG_TRACE("Begin");
    
    //Get the inputs for throttle and heading each loop and update subsystem values
    float throttle_val = 0.0f;
    WsInputFacade::instance()->getDriverThrottleValue(throttle_val);
    
    float heading_val  = 0.0f;
    WsInputFacade::instance()->getDriverHeadingValue(heading_val);
    
    // Set those values in the drive base subsystem
    this->setThrottle(throttle_val);
    this->setHeading(heading_val);
    
    //Use updated values to update quickTurnFlag
    bool autoQuickTurnFlag = false;
    this->checkAutoQuickTurn(autoQuickTurnFlag);
    this->setAutoQuickTurn(autoQuickTurnFlag);
    
    // Get a stack-local reference to the facade
    WsOutputFacade* p_facade = WsOutputFacade::instance();
    
    // Set the drive motor outputs
    this->updateDriveMotors();
    
    // Set landing gear output
    this->updateLandingGear();
    
    // Set gear shifter output
    WsTypes::WsShifterStateType sst =
        (true == a_shifterFlag)
        ? WsTypes::WS_S_HIGHGEAR
        : WsTypes::WS_S_LOWGEAR;
    p_facade->setShifterState(sst);
    
    WS_LOG_TRACE("End");
}

void
WsDriveBase::updateDriveMotors(void)
{
    //
    // Calculate the requested angular power for current inputs.  We have
    //  defined a heading dead-band where insignificant heading stick deflections
    //  will not result in angular power.  This helps guard against gamepads
    //  whose joysticks are not perfectly centered.  This deadband should be
    //  smaller than the threshold for zero-point turning.
    //
    float angular_power = 0.0f;
    if (fabs(a_driveBaseHeading) > 0.05f)
    {
        angular_power = fabs(a_driveBaseThrottle) *
                        a_driveBaseHeading *
                        WS_HEADING_SENSITIVITY;   // @TODO:  Make this configurable!!
    }
    
    // Calculate motor outputs
    float right_motor_speed = 0.0;
    float left_motor_speed = 0.0;
    
    if (true == a_quickAutoTurnFlag)
    {
        right_motor_speed = 0.0f;
        left_motor_speed = 0.0f;
        a_driveBaseThrottle = 0.0f;
        
        // Quick turn does not take throttle into account
        left_motor_speed += a_driveBaseHeading;
        right_motor_speed -= a_driveBaseHeading;
    }
    else
    {
        // Not in quick-turn, don't let motors rotate in opposite directions
        right_motor_speed = a_driveBaseThrottle - angular_power;
        left_motor_speed = a_driveBaseThrottle + angular_power;
        
        if (a_driveBaseThrottle >= 0.0)
        {
            right_motor_speed = (right_motor_speed >= 0.0)
                                ? right_motor_speed
                                : 0.0;
            left_motor_speed = (left_motor_speed >= 0.0)
                               ? left_motor_speed
                               : 0.0;
        }
        else
        {
            right_motor_speed = (right_motor_speed < 0.0)
                                ? right_motor_speed
                                : 0.0;
            left_motor_speed = (left_motor_speed < 0.0)
                               ? left_motor_speed
                               : 0.0;
        }
    }
    
    // Cap motor output at the allowable maximum
    if (right_motor_speed >  WS_MOTOR_OUTPUT_MAXIMUM) { right_motor_speed =  WS_MOTOR_OUTPUT_MAXIMUM; }
    if (left_motor_speed >   WS_MOTOR_OUTPUT_MAXIMUM) { left_motor_speed =   WS_MOTOR_OUTPUT_MAXIMUM; }
    if (right_motor_speed < -WS_MOTOR_OUTPUT_MAXIMUM) { right_motor_speed = -WS_MOTOR_OUTPUT_MAXIMUM; }
    if (left_motor_speed < - WS_MOTOR_OUTPUT_MAXIMUM) { left_motor_speed =  -WS_MOTOR_OUTPUT_MAXIMUM; }
    
    //WS_LOG_INFO("<T,H,L,M> = <%.3f,%.3f,%.3f,%.3f>",
    //            a_driveBaseThrottle,
    //            a_driveBaseHeading,
    //            left_motor_speed,
    //            right_motor_speed);
    
    // Finally, set the drive motor outputs
    WsOutputFacade::instance()->setRightDriveMotorSpeed(right_motor_speed);
    WsOutputFacade::instance()->setLeftDriveMotorSpeed(left_motor_speed);
}

void
WsDriveBase::updateLandingGear(void)
{
    bool lg_button_state = false;
    bool lg_sensor_state = false;
    WsInputFacade::instance()->getLandingGearButtonState(lg_button_state);
    WsInputFacade::instance()->getLandingGearRetractionSwitch(lg_sensor_state);
    WsLandingGearOverrideStateType new_state = a_landingGearOverrideState;
    
    switch (a_landingGearOverrideState)
    {
        case WS_DB_LG_OVERRIDE_STATE_NORMAL:
        {
            if ((true == lg_button_state) &&
                (true == lg_sensor_state))
            {
                ap_landingGearOverrideTimer->Reset();
                new_state = WS_DB_LG_OVERRIDE_STATE_KEEP_DEPLOYED;
                WS_LOG_DEBUG("TRANSITION TO DEPLOY LOCK");
            }
            break;
        }
        
        case WS_DB_LG_OVERRIDE_STATE_KEEP_DEPLOYED:
        {
            // Make sure the LG is out
            a_landingGearFlag = true;
            
            if (true == ap_landingGearOverrideTimer->HasPeriodPassed(LOCK_LANDING_GEAR_DEPLOY_TIME))
            {
                ap_landingGearOverrideTimer->Reset();
                new_state = WS_DB_LG_OVERRIDE_STATE_KEEP_RETRACTED;
                WS_LOG_DEBUG("TRANSITION TO RETRACT LOCK");
            }
            break;
        }
        
        case WS_DB_LG_OVERRIDE_STATE_KEEP_RETRACTED:
        {
            a_landingGearFlag = false;
            
            if ((true == ap_landingGearOverrideTimer->HasPeriodPassed(LOCK_LANDING_GEAR_RETRACT_TIME)) &&
                (false == lg_button_state))
            {
                ap_landingGearOverrideTimer->Reset();
                new_state = WS_DB_LG_OVERRIDE_STATE_NORMAL;
                WS_LOG_DEBUG("TRANSITION TO NORMAL");
            }
            break;
        }
        
        case WS_DB_LG_OVERRIDE_STATE_NUM:
        case WS_DB_LG_OVERRIDE_STATE_NONE:
        default:
        {
            WS_LOG_ERROR("Invalid State");
            new_state = WS_DB_LG_OVERRIDE_STATE_NORMAL;
            break;
        }
    } // end switch()
    
    // Not the next state
    a_landingGearOverrideState = new_state;
    
    // Set landing gear output
    WsTypes::WsLandingGearStateType lgst =
        (true == a_landingGearFlag)
        ? WsTypes::WS_LG_DEPLOYED
        : WsTypes::WS_LG_RETRACTED;
    WsOutputFacade::instance()->setLandingGearState(lgst);
}


void
WsDriveBase::setThrottle(const float& rc_throttle)
{
    WS_LOG_TRACE("Begin");
    
    // Taking into account Anti-Turbo
    float new_throttle = rc_throttle;
    if (true == a_antiTurboFlag)
    {
        // Multiply by the anti-turbo ceiling to give a bigger range
        new_throttle *= WS_ANTI_TURBO_MAX_DEFLECTION;
        
        // Cap the heading and throttle at the anti-turbo allowed maximum
        if (new_throttle >  WS_ANTI_TURBO_MAX_DEFLECTION) { new_throttle =  WS_ANTI_TURBO_MAX_DEFLECTION; }
        if (new_throttle < -WS_ANTI_TURBO_MAX_DEFLECTION) { new_throttle = -WS_ANTI_TURBO_MAX_DEFLECTION; }
    }
    
    //
    // The robot is very jumpy due to the pneumatic tires... so we limit
    //  acceleration by applying a fast & easy low-pass filter to the inputs:
    //  y[n] is the current output
    //  x[n] is the current input
    //  alpha is the acceleration factor
    //
    //  This is the acceleration algorithm
    //   y[n] = y[n-1] + (x[n] - y[n-1]) * alpha
    //
    //  We only apply this acceleration-limiter technique if the accelerationEnableFlag
    //   is true.  Otherwise, the driver-provided inputs are used directly.
    //
    a_driveBaseThrottle = (true == a_accelerationEnableFlag)
                          ? a_driveBaseThrottle + (new_throttle - a_driveBaseThrottle) * WS_THROTTLE_ACCEL_FACTOR
                          : new_throttle;
                          
    // Clip the inputs just in case...
    if (a_driveBaseThrottle >  WS_THROTTLE_INPUT_MAXIMUM) { a_driveBaseThrottle =  WS_THROTTLE_INPUT_MAXIMUM; }
    if (a_driveBaseThrottle < -WS_THROTTLE_INPUT_MAXIMUM) { a_driveBaseThrottle = -WS_THROTTLE_INPUT_MAXIMUM; }
    
    WS_LOG_TRACE("End");
}

void
WsDriveBase::setHeading(const float& rc_heading)
{
    WS_LOG_TRACE("Begin");
    
    // Taking into account Anti-Turbo
    float new_heading = rc_heading;
    if (true == a_antiTurboFlag)
    {
        // Multiply by the anti-turbo ceiling to give a bigger range
        new_heading *= WS_ANTI_TURBO_MAX_DEFLECTION;
        
        // Cap the heading and throttle at the anti-turbo allowed maximum
        if (new_heading >   WS_ANTI_TURBO_MAX_DEFLECTION) { new_heading =   WS_ANTI_TURBO_MAX_DEFLECTION; }
        if (new_heading <  -WS_ANTI_TURBO_MAX_DEFLECTION) { new_heading =  -WS_ANTI_TURBO_MAX_DEFLECTION; }
    }
    
    //
    // The robot is very jumpy due to the pneumatic tires... so we limit
    //  acceleration by applying a fast & easy low-pass filter to the inputs:
    //  y[n] is the current output
    //  x[n] is the current input
    //  alpha is the acceleration factor
    //
    //  This is the acceleration algorithm
    //   y[n] = y[n-1] + (x[n] - y[n-1]) * alpha
    //
    a_driveBaseHeading = (true == a_accelerationEnableFlag)
                         ? a_driveBaseHeading + (new_heading - a_driveBaseHeading) * WS_HEADING_ACCEL_FACTOR
                         : new_heading;
                         
    // Clip the inputs just in case...
    if (a_driveBaseHeading >  WS_HEADING_INPUT_MAXIMUM) { a_driveBaseHeading =  WS_HEADING_INPUT_MAXIMUM; }
    if (a_driveBaseHeading < -WS_HEADING_INPUT_MAXIMUM) { a_driveBaseHeading = -WS_HEADING_INPUT_MAXIMUM; }
    
    WS_LOG_TRACE("End");
}

void
WsDriveBase::setLandingGear(const bool& rc_landingGear)
{
    WS_LOG_TRACE("Begin");
    a_landingGearFlag = rc_landingGear;
    WS_LOG_TRACE("End");
}

void
WsDriveBase::getShifterState(bool& r_shifterState)
{
    WS_LOG_TRACE("Begin");
    r_shifterState = a_shifterFlag;
    WS_LOG_TRACE("End");
}

void
WsDriveBase::setShifterState(const bool& rc_shifterState)
{
    WS_LOG_TRACE("Begin");
    a_shifterFlag = rc_shifterState;
    WS_LOG_TRACE("End");
}

void
WsDriveBase::setDriveStraight(const bool& rc_driveStraight)
{
    WS_LOG_TRACE("Begin");
    a_driveStraightFlag = rc_driveStraight;
    WS_LOG_TRACE("End");
}

void
WsDriveBase::setAutoQuickTurn(const bool& rc_quickTurn)
{
    WS_LOG_TRACE("Begin");
    a_quickAutoTurnFlag = rc_quickTurn;
    WS_LOG_TRACE("End");
}

void
WsDriveBase::checkAutoQuickTurn(bool& r_state)
{
    float throttle = a_driveBaseThrottle;
    float heading = a_driveBaseHeading;
    
    throttle = fabs(throttle);
    heading = fabs(heading);
    
    if ((throttle < 0.1) && (heading > 0.1))
    {
        r_state = true;
    }
    else
    {
        r_state = false;
    }
}

void
WsDriveBase::acceptNewAntiturboButtonState(const bool& rc_oldState, const bool& rc_newState)
{
    // Toggle the processed "sticky" button state on raw transition
    if ((false == rc_oldState) &&
        (true == rc_newState))
    {
        a_antiTurboFlag = !a_antiTurboFlag;
        WsDataIndicationFacade::instance()->setDriveBaseAntiTurboEnabled(a_antiTurboFlag);
    }
}

void
WsDriveBase::acceptNewShifterButtonState(const bool& rc_oldState, const bool& rc_newState)
{
    if ((false == rc_oldState) &&
        (true == rc_newState))
    {
        a_shifterFlag = !a_shifterFlag;
    }
}

void
WsDriveBase::notifyLandingGearInputChange(void)
{
    // Get the landing gear switch state from the InputFacade
    bool sensor_state = true;
    bool button_state = true;
    WsInputFacade::instance()->getLandingGearRetractionSwitch(sensor_state);
    WsInputFacade::instance()->getLandingGearButtonState(button_state);
    
    // If the button is pressed but the switch isn't, deploy the landing gear
    if ((button_state) && (!sensor_state))
    {
        a_landingGearFlag = true;
    }
    else
    {
        a_landingGearFlag = false;
    }
}

void
WsDriveBase::acceptNewAccelerationEnableFlag(const bool& rc_oldState, const bool& rc_newState)
{
    if ((false == rc_oldState) &&
        (true == rc_newState))
    {
        a_accelerationEnableFlag = !a_accelerationEnableFlag;
        WsDataIndicationFacade::instance()->setDriveBaseAccelerationEnabled(a_accelerationEnableFlag);
    }
}


//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------
