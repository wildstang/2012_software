#include "WsAutoFenderLeftProgram.hh"
#include "WsLogger.hh"
#include "WsAutonomousProgram.hh"
#include "Timer.h"
#include "WsInputFacade.hh"
#include "WsOutputFacade.hh"
#include "WsConfigFacade.hh"
#include "WsTypes.hh"

const float OI_FLYWHEEL_POT_MANUAL = 2.79f;
const float OI_FLYWHEEL_POT_PRESET = 1.65;
const float OI_FLYWHEEL_POT_OFF = 0.01f;

WsAutoFenderLeftProgram::WsAutoFenderLeftProgram(void)
    : WsAutonomousProgram("WsAutoFenderLeftProgram")
    , a_done(false)
    , a_currentState(WS_AUTO_FENDER_LEFT_STATE_SPIN_FLY_AND_DRIVE)
    , ap_stateTimer(NULL)
{
    WS_LOG_TRACE("Begin");
    
    ::memset(&a_configVals, 0x00, sizeof(a_configVals));
    
    ap_stateTimer = new Timer();
    ap_stateTimer->Stop();
    ap_stateTimer->Reset();
    
    WS_LOG_TRACE("End");
}

WsAutoFenderLeftProgram::~WsAutoFenderLeftProgram(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_stateTimer;
    ap_stateTimer = NULL;
    
    WS_LOG_TRACE("End");
}

void WsAutoFenderLeftProgram::init(void)
{
    WsConfigFacade::instance()->getAutoFenderLeftConfig(a_configVals);
    
    WsTypes::WsHoodCylinderStateType hood_state = WsTypes::WS_HOOD_CYLINDER_INVALID;
    WsOutputFacade::instance()->getHoodState(hood_state);
    if (WsTypes::WS_HOOD_CYLINDER_UP == hood_state)
    {
        WsInputFacade::instance()->setHoodButtonState(true);
    }
    
    WsAutonomousProgram::init();
    
    ap_stateTimer->Start();
}

bool
WsAutoFenderLeftProgram::run(void)
{
    WS_LOG_TRACE("Begin");
    
    INT32 left_enc;
    WsInputFacade::instance()->getLeftDriveEncoderCount(left_enc);
    
    INT32 right_enc;
    WsInputFacade::instance()->getLeftDriveEncoderCount(right_enc);
    
    WsAutoFenderLeftProgramStateType new_state = a_currentState;
    bool shoot_button = false;
    float flywheel_state = OI_FLYWHEEL_POT_PRESET;
    WsTypes::WsShooterPresetType preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
    bool shoot_override_button = false;
    bool escalator_button = false;
    bool accum_motor_state = false;
    bool accum_actuator_state = false;
    float drive_throttle = 0.0f;
    float drive_heading = 0.0f;
    
    switch (a_currentState)
    {
        case WS_AUTO_FENDER_LEFT_STATE_SPIN_FLY_AND_DRIVE:
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = a_configVals.ws_auto_fender_left_drive_fender_throttle;
            drive_heading = a_configVals.ws_auto_fender_left_drive_fender_heading;
            
            if ((abs(left_enc) >= a_configVals.ws_auto_fender_left_drive_fender_encoder_target) ||
                (abs(right_enc) >= a_configVals.ws_auto_fender_left_drive_fender_encoder_target) ||
                (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_fender_left_drive_fender_timeout)))
            {
                new_state = WS_AUTO_FENDER_LEFT_STATE_SQUARE_UP_ON_FENDER;
            }
            
            break;
        case WS_AUTO_FENDER_LEFT_STATE_SQUARE_UP_ON_FENDER:
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = a_configVals.ws_auto_fender_left_square_up_throttle;
            drive_heading = 0.0f;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_fender_left_square_up_timeout))
            {
                new_state = WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_COAST;
            }
            
            break;
        case WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_COAST:
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_fender_left_coast_timeout))
            {
                new_state = WS_AUTO_FENDER_LEFT_STATE_SHOOT_FIRST_BALL;
            }
            
            break;
        case WS_AUTO_FENDER_LEFT_STATE_SHOOT_FIRST_BALL:
            shoot_button = true;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = true;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_fender_left_shoot_first_timeout))
            {
                new_state = WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_FLYWHEEL;
            }
            
            break;
        case WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_FLYWHEEL:
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_fender_left_wait_for_flywheel_timeout))
            {
                new_state = WS_AUTO_FENDER_LEFT_STATE_BACK_UP;
            }
            
            break;
        case WS_AUTO_FENDER_LEFT_STATE_BACK_UP:
        {
        
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_OFF;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = a_configVals.ws_auto_fender_left_back_up_throttle;
            drive_heading = a_configVals.ws_auto_fender_left_back_up_heading;
            
            WS_LOG_NOTICE("if_leftEncoderCount = %d", left_enc);
            WS_LOG_NOTICE("if_rightEncoderCount = %d", right_enc);
            
            if ((abs(left_enc) >= a_configVals.ws_auto_fender_left_back_up_encoder_target) ||
                (abs(right_enc) >= a_configVals.ws_auto_fender_left_back_up_encoder_target) ||
                (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_fender_left_back_up_timeout)))
            {
                new_state = WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_TURN;
            }
        } break;
        
        case WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_TURN:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            
            WS_LOG_NOTICE("if_leftEncoderCount = %d", left_enc);
            WS_LOG_NOTICE("if_rightEncoderCount = %d", right_enc);
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_fender_left_wait_for_turn_timeout))
            {
                new_state = WS_AUTO_FENDER_LEFT_STATE_TURN_TO_BARRIER;
            }
        } break;
        
        case WS_AUTO_FENDER_LEFT_STATE_TURN_TO_BARRIER:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_OFF;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = a_configVals.ws_auto_fender_left_turn_throttle;
            drive_heading = a_configVals.ws_auto_fender_left_turn_heading;
            
            WS_LOG_NOTICE("if_leftEncoderCount = %d", left_enc);
            WS_LOG_NOTICE("if_rightEncoderCount = %d", right_enc);
            
            if ((abs(left_enc) >= a_configVals.ws_auto_fender_left_turn_encoder_target) ||
                (abs(right_enc) >= a_configVals.ws_auto_fender_left_turn_encoder_target) ||
                (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_fender_left_turn_timeout)))
            {
                new_state = WS_AUTO_FENDER_LEFT_STATE_DRIVE_TO_BARRIER;
            }
        } break;
        
        case WS_AUTO_FENDER_LEFT_STATE_DRIVE_TO_BARRIER:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_OFF;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = a_configVals.ws_auto_fender_left_drive_barrier_throttle;
            drive_heading = a_configVals.ws_auto_fender_left_drive_barrier_heading;
            
            WS_LOG_NOTICE("if_leftEncoderCount = %d", left_enc);
            WS_LOG_NOTICE("if_rightEncoderCount = %d", right_enc);
            
            if ((abs(left_enc) >= a_configVals.ws_auto_fender_left_drive_barrier_encoder_target) ||
                (abs(right_enc) >= a_configVals.ws_auto_fender_left_drive_barrier_encoder_target) ||
                (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_fender_left_drive_barrier_timeout)))
            {
                new_state = WS_AUTO_FENDER_LEFT_STATE_COAST_TO_BARRIER;
            }
        } break;
        
        case WS_AUTO_FENDER_LEFT_STATE_COAST_TO_BARRIER:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_OFF;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            
            WS_LOG_NOTICE("if_leftEncoderCount = %d", left_enc);
            WS_LOG_NOTICE("if_rightEncoderCount = %d", right_enc);
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_fender_left_coast_barrier_timeout))
            {
                new_state = WS_AUTO_FENDER_LEFT_STATE_DONE;
            }
        } break;
        
        case WS_AUTO_FENDER_LEFT_STATE_DONE:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_OFF;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            
            a_done = true;
            
        } break;
        
        case WS_AUTO_FENDER_LEFT_STATE_NUM:
        case WS_AUTO_FENDER_LEFT_STATE_NONE:
        default:
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_OFF;
            preset = WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT;
            shoot_override_button = false;
            escalator_button = false;
            accum_motor_state = false;
            accum_actuator_state = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            
            WS_LOG_ERROR("Invalid Autonomous State! ");
            
            new_state = WS_AUTO_FENDER_LEFT_STATE_DONE;
            
            break;
    }
    
    if (a_currentState != new_state)
    {
        WS_LOG_ALWAYS("LEAVE a_currentState: %d GOTO new_state: %d", a_currentState, new_state);
        WsInputFacade::instance()->resetDriveEncoderCounts();
        ap_stateTimer->Reset();
    }
    
    a_currentState = new_state;
    
    WsInputFacade::instance()->setBallShootButtonState(shoot_button);
    WsInputFacade::instance()->setFlywheelStatePotValue(flywheel_state);
    WsInputFacade::instance()->setShooterPreset(preset);
    WsInputFacade::instance()->setBallShootOverrideButtonState(shoot_override_button);
    WsInputFacade::instance()->setEscalatorUpButtonState(escalator_button);
    WsInputFacade::instance()->setAccumulatorMotorButtonState(accum_motor_state);
    WsInputFacade::instance()->setDriverThrottleValue(drive_throttle);
    WsInputFacade::instance()->setDriverHeadingValue(drive_heading);
    WsInputFacade::instance()->setAccumulatorDeployButtonState(accum_actuator_state);
    
    WS_LOG_TRACE("End");
    return true;
}

void
WsAutoFenderLeftProgram::clear(void)
{
    a_currentState = WS_AUTO_FENDER_LEFT_STATE_SPIN_FLY_AND_DRIVE;
    
    ap_stateTimer->Stop();
    ap_stateTimer->Reset();
    
    a_done = false;
}

bool
WsAutoFenderLeftProgram::isDone(void)
{
    return a_done;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

