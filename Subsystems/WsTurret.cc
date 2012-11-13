#include "WsTurret.hh"
#include "WsLogger.hh"
#include "WsOutputFacade.hh"
#include "WsInputFacade.hh"
#include "WsDataIndicationFacade.hh"
#include "WsPidController.hh"
#include "WsTurretPidOutput.hh"
#include "WsTurretPidInput.hh"
#include "WsTurretObservers.hh"
#include <math.h>

WsTurret::WsTurret(void)
    : WsSubsystem("WsTurret")
    , a_headingAxisValue(0.0f)
    , a_headingPotValue(0.0f)
    , a_turretControlState(WsTurret::WS_TURRET_OFF)
    , a_cwLimitSwitchState(false)
    , a_ccwLimitSwitchState(false)
    , a_turboFlag(false)
    , ap_turretPidController(NULL)
    , ap_turretPidInput(NULL)
    , ap_turretPidOutput(NULL)
    , a_presetConfig()
    , ap_headingAxisValueObserver(NULL)
    , ap_headingPotValueObserver(NULL)
    , ap_turboButtonStateObserver(NULL)
    , ap_cwLimitSwitchStateObserver(NULL)
    , ap_ccwLimitSwitchStateObserver(NULL)
    , ap_turretPidConfigObserver(NULL)
    , ap_requestedPresetObserver(NULL)
    , a_autoTrackingState(WS_TRACKING_STATE_NO_TARGET)
{
    WS_LOG_TRACE("Begin");
    
    WsTypes::WsHoodCylinderStateType state_up = WsTypes::WS_HOOD_CYLINDER_UP;
    WsOutputFacade::instance()->setHoodState(state_up);
    
    ap_turretPidInput = new WsTurretPidInput();
    ap_turretPidOutput = new WsTurretPidOutput();
    
    // @TODO Perhaps remove PID/Ee/Es from required ctor args...
    ap_turretPidController =
        new WsPidController(ap_turretPidInput,     // Read process variable from here
                            ap_turretPidOutput,    // Write new outputs here
                            "Turret PID");
    ap_turretPidController->Reset();
    ap_turretPidController->Disable();
    
    ::memset(&a_presetConfig, 0x00, sizeof(a_presetConfig));
    
    //
    // The observers must be the last objects created here...
    //
    ap_headingAxisValueObserver       = new WsTurretHeadingAxisValueObserver(this);
    ap_headingPotValueObserver        = new WsTurretHeadingPotValueObserver(this);
    ap_turboButtonStateObserver       = new WsTurretTurboButtonStateObserver(this);
    ap_cwLimitSwitchStateObserver     = new WsTurretCwLimitSwitchStateObserver(this);
    ap_ccwLimitSwitchStateObserver    = new WsTurretCcwLimitSwitchStateObserver(this);
    ap_turretPidConfigObserver        = new WsTurretPidConfigObserver(this);
    ap_requestedPresetObserver        = new WsTurretRequestedPresetObserver(this);
    ap_presetConfigObserver           = new WsTurretPresetConfigObserver(this);
    
    WS_LOG_TRACE("End");
}

WsTurret::~WsTurret(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_turretPidController;          ap_turretPidController = NULL;
    delete ap_turretPidInput;               ap_turretPidInput = NULL;
    delete ap_turretPidOutput;              ap_turretPidOutput = NULL;
    delete ap_headingAxisValueObserver;     ap_headingAxisValueObserver = NULL;
    delete ap_headingPotValueObserver;      ap_headingPotValueObserver = NULL;
    delete ap_turboButtonStateObserver;     ap_turboButtonStateObserver = NULL;
    delete ap_cwLimitSwitchStateObserver;   ap_cwLimitSwitchStateObserver = NULL;
    delete ap_ccwLimitSwitchStateObserver;  ap_ccwLimitSwitchStateObserver = NULL;
    delete ap_turretPidConfigObserver;      ap_turretPidConfigObserver = NULL;
    delete ap_requestedPresetObserver;      ap_requestedPresetObserver = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsTurret::update(void)
{
    WS_LOG_TRACE("Begin");
    
    float turret_motor_output = 0.0f;
    
    //
    // Update the Turret Heading
    //
    switch (a_turretControlState)
    {
        case WsTurret::WS_TURRET_MANUAL_CONTROL:
        {
            if (true == a_turboFlag)
            {
                turret_motor_output = (a_headingAxisValue > 0.0f)
                                      ? -1.0f
                                      :  1.0f;
            }
            else
            {
                // Cap at a max speed of 30% and linear from the heading axis
                turret_motor_output = a_headingAxisValue * -0.3;
            }
            
            // Check motor rotation against limit switches...
            // @TODO We don't actually have limit switches working!!
            if (turret_motor_output >= 0.0f)
            {
                if (true == a_cwLimitSwitchState)
                {
                    // If the turret is trying to rotate past the clockwise limit, stop it from doing so
                    turret_motor_output = 0.0f;
                }
            }
            else
            {
                if (true == a_ccwLimitSwitchState)
                {
                    // If the turret is trying to rotate past the counterclockwise limit, stop it from doing so
                    turret_motor_output = 0.0f;
                }
            }
            
            // Finally update the motor output
            WsOutputFacade::instance()->setTurretHeadingSpeed(turret_motor_output);
        } break;
        
        case WsTurret::WS_TURRET_PRESET_CONTROL:
        {
            // Default to a known position
            float setpoint = a_presetConfig.turret_auton_voltage;
            
            switch (a_currentRequestedPreset)
            {
                case WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD:
                {
                    setpoint = a_presetConfig.turret_key_forward_voltage;
                } break;
                
                case WsTypes::WS_SHOOTER_PRESET_KEY_BACK:
                {
                    setpoint = a_presetConfig.turret_key_reverse_voltage;
                } break;
                
                case WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT:
                {
                    setpoint = a_presetConfig.turret_fender_left_voltage;
                } break;
                
                case WsTypes::WS_SHOOTER_PRESET_FENDER_RIGHT:
                {
                    setpoint = a_presetConfig.turret_fender_right_voltage;
                } break;
                
                case WsTypes::WS_SHOOTER_PRESET_AUTON:
                {
                    setpoint = a_presetConfig.turret_auton_voltage;
                } break;
                
                case WsTypes::WS_SHOOTER_PRESET_NONE:
                {
                    // Don't update the PID, but this is not an error-case
                } break;
                
                default:
                {
                    // This shouldn't happen, get back to a good state if it does
                    WS_LOG_ERROR("Invalid Requested Preset: %d", a_currentRequestedPreset);
                    a_currentRequestedPreset = WsTypes::WS_SHOOTER_PRESET_NONE;
                    a_turretControlState = WsTurret::WS_TURRET_OFF;
                    setpoint = a_presetConfig.turret_auton_voltage;
                    ap_turretPidController->Disable();
                }
            }
            
            // Allow the PID to update
            ap_turretPidController->setSetPoint(setpoint);
            ap_turretPidController->calcPid();
        } break;
        
        case WsTurret::WS_TURRET_OFF:
        {
            // Make sure the motor is not moving
            turret_motor_output = 0.0f;
            WsOutputFacade::instance()->setTurretHeadingSpeed(turret_motor_output);
        } break;
        
        default:
        {
            WS_LOG_ERROR("Invalid Turret Control State: %d", a_turretControlState);
            a_turretControlState = WsTurret::WS_TURRET_OFF;
        }
    }
    
    // Push updates to the dashboard
    float setpoint      = 0.0;  ap_turretPidController->getSetPoint(setpoint);
    bool is_stable      = ap_turretPidController->isStabilized();
    bool is_near_target = ((false == is_stable) && ap_turretPidController->isOnTarget());
    bool is_enabled     = ap_turretPidController->isEnabled();
    WsDataIndicationFacade::instance()->setTurretPidEnabled(is_enabled);
    WsDataIndicationFacade::instance()->setTurretPidSetpoint(setpoint);
    WsDataIndicationFacade::instance()->setTurretNearTargetPositionFlag(is_near_target);
    WsDataIndicationFacade::instance()->setTurretAtTargetPositionFlag(is_stable);
    
    WS_LOG_TRACE("End");
}

void
WsTurret::acceptNewHeadingAxisValue(const float& rc_newValue)
{
    if (fabs(rc_newValue) > 0.1)
    {
        // User requested manual control of the turret
        a_headingAxisValue = rc_newValue;
        a_turretControlState = WsTurret::WS_TURRET_MANUAL_CONTROL;
        
        // Make sure the auto-tracking is off
        ap_turretPidController->Disable();
        a_autoTrackingState = WS_TRACKING_STATE_NO_TARGET;
    }
    else if ((fabs(rc_newValue) < 0.1) &&
             (fabs(a_headingAxisValue) > 0.1))
    {
        // Last loop, user wanted manual control, this loop he doesn't
        a_turretControlState = WsTurret::WS_TURRET_OFF;
        a_headingAxisValue = 0.0f;
        
        // Make sure the auto-tracking is off
        ap_turretPidController->Disable();
        a_autoTrackingState = WS_TRACKING_STATE_NO_TARGET;
    }
    else
    {
        //
        // User didn't want manual conrol last time, and he still doesn't
        //  Since we might be in preset-control, let's not change the control
        //  state here.  If we did, we might prematurely stop the turret
        //  on its way to a preset location.
        //
        a_headingAxisValue = 0.0f;
    }
}

void
WsTurret::acceptNewHeadingPotValue(const float& rc_newValue)
{
}

void
WsTurret::acceptNewTurboButtonState(const bool& rc_newButtonState)
{
    a_turboFlag = rc_newButtonState;
}

void
WsTurret::acceptNewCwLimitSwitchState(const bool& rc_newSwitchState)
{
    a_cwLimitSwitchState = rc_newSwitchState;
}

void
WsTurret::acceptNewCcwLimitSwitchState(const bool& rc_newSwitchState)
{
    a_ccwLimitSwitchState = rc_newSwitchState;
}

void
WsTurret::acceptNewPidConfig(const WsTypes::WsPidConfigurationDataT& rc_new_data)
{
    bool is_enabled = ap_turretPidController->isEnabled();
    
    // Disable Pid Controller if needed
    if (true == is_enabled)
    {
        ap_turretPidController->Disable();
    }
    
    ap_turretPidController->setConstants(rc_new_data.k_p, rc_new_data.k_i, rc_new_data.k_d);
    ap_turretPidController->setStaticEpsilon(rc_new_data.static_epsilon);
    ap_turretPidController->setErrorEpsilon(rc_new_data.error_epsilon);
    ap_turretPidController->setErrorIncrementPercentage(rc_new_data.error_increment);
    ap_turretPidController->setMaxIntegral(rc_new_data.max_integral);
    ap_turretPidController->setIntegralErrorThresh(rc_new_data.integral_error_thresh);
    ap_turretPidController->setDifferentiatorBandLimit(rc_new_data.differentiator_band_limit);
    ap_turretPidController->setMinMaxOutput(rc_new_data.min_output, rc_new_data.max_output);
    ap_turretPidController->setMinMaxInput(rc_new_data.min_input, rc_new_data.max_input);
    ap_turretPidController->setMinStabilizationTime(rc_new_data.min_on_target_time);
    
    // Re-Enable Pid Controller if needed
    if (true == is_enabled)
    {
        ap_turretPidController->Enable();
    }
}

void
WsTurret::acceptNewRequestedPreset(const WsTypes::WsShooterPresetType& rc_newValue)
{
    if (WsTypes::WS_SHOOTER_PRESET_NONE != rc_newValue)
    {
        a_currentRequestedPreset = rc_newValue;
        a_turretControlState = WsTurret::WS_TURRET_PRESET_CONTROL;
        
        // The PID should be enabled here
        ap_turretPidController->Enable();
    }
    else
    {
        //
        // Leave the pre-set alone.  We only change the current preset position
        //  if the user changes the requested position.  In this case, the user
        //  selected a preset last time, and this time he let the button go.
        //
    }
}

void
WsTurret::updateAutoTracking(void)
{
//
//    // @TODO Observe the num visible targets
//    int num_targets = 0;
//    WsInputFacade::instance()->getNumVisibleTargets(num_targets);
//
//    WsAutoTrackingStateType new_state = a_autoTrackingState;
//    float heading = 0.0;
//
//    switch (a_autoTrackingState)
//    {
//        case WS_TRACKING_STATE_NO_TARGET:
//        {
//            ap_turretFinePidController->Disable();
//            ap_turretCoarsePidController->Disable();
//            heading = 0.0;
//            WsOutputFacade::instance()->setTurretHeadingSpeed(heading);
//            WS_LOG_INFO("No Targets Found!!");
//
//            if (num_targets > 0)
//            {
//                ap_turretCoarsePidController->Enable();
//                new_state = WS_TRACKING_STATE_COARSE;
//            }
//        } break;
//
//        case WS_TRACKING_STATE_COARSE:
//        {
//            ap_turretCoarsePidController->calcPid();
//            if (num_targets < 1)
//            {
//                // We lost the targets completely, stop where we are
//                ap_turretFinePidController->Disable();
//                ap_turretCoarsePidController->Disable();
//                new_state = WS_TRACKING_STATE_NO_TARGET;
//            }
//            else if (ap_turretCoarsePidController->isOnTarget())
//            {
//                // We hit our "big target" range, let's do fine-tuning now
//                ap_turretCoarsePidController->Disable();
//                ap_turretFinePidController->Enable();
//                new_state = WS_TRACKING_STATE_FINE;
//            }
//            else
//            {
//                // Not in range yet, let's keep moving...
//                new_state = WS_TRACKING_STATE_COARSE;
//            }
//        } break;
//
//        // The implementations of 'fine' and 'on-target' are the same
//        case WS_TRACKING_STATE_FINE:
//        case WS_TRACKING_STATE_ON_TARGET:
//        {
//            ap_turretFinePidController->calcPid();
//            if (num_targets < 1)
//            {
//                // We lost the targets completely, stop where we are
//                ap_turretFinePidController->Disable();
//                ap_turretCoarsePidController->Disable();
//                new_state = WS_TRACKING_STATE_NO_TARGET;
//            }
//            else
//            {
//                // We still see targets, let's find out where we are now
//                if (true == ap_turretFinePidController->isStabilized())
//                {
//                    // We are locked on target, perfect!!
//                    new_state = WS_TRACKING_STATE_ON_TARGET;
//                }
//                else if (true == ap_turretFinePidController->isOnTarget())
//                {
//                    // We are on target, but not for long enough... keep going
//                    new_state = WS_TRACKING_STATE_FINE;
//                }
//                else
//                {
//                    // We are not on target, let's see how bad it is...
//                    if (fabs(ap_turretFinePidController->getError()) >
//                        ap_turretCoarsePidController->getEpsilon())
//                    {
//                        // We're way off, switch back to coarse tuning
//                        ap_turretFinePidController->Disable();
//                        ap_turretCoarsePidController->Enable();
//                        new_state = WS_TRACKING_STATE_COARSE;
//
//                        // Oops!  Make sure the drivers know we aren't on target
//                    }
//                    else
//                    {
//                        // We aren't off by that much, but we are off...
//                        //  Keep the fine-tuning PID running...
//                        new_state = WS_TRACKING_STATE_FINE;
//                    }
//                }
//            }
//        } break;
//
//        case WS_TRACKING_STATE_NUM:
//        default:
//        {
//            ap_turretFinePidController->Disable();
//            ap_turretCoarsePidController->Disable();
//            heading = 0.0;
//            WsOutputFacade::instance()->setTurretHeadingSpeed(heading);
//
//            // This isn't good...
//            WS_LOG_ERROR("Invalid State Found: %d, Returning to No Target State", a_autoTrackingState);
//            new_state = WS_TRACKING_STATE_NO_TARGET;
//        } break;
//    }
//
//    // Update driver indications
//    bool turret_at_target_position_flag =
//        (WS_TRACKING_STATE_ON_TARGET == new_state);
//
//    bool turret_near_target_position_flag =
//        ((WS_TRACKING_STATE_FINE == new_state) ||
//         (WS_TRACKING_STATE_ON_TARGET == new_state));
//
//    WsDataIndicationFacade::instance()->setTurretAtTargetPositionFlag(turret_at_target_position_flag);
//    WsDataIndicationFacade::instance()->setTurretNearTargetPositionFlag(turret_near_target_position_flag);
//
//    WS_LOG_DEBUG("Old State: %d, New State: %d", a_autoTrackingState, new_state);
//    a_autoTrackingState = new_state;
}

void
WsTurret::acceptNewPresetConfig(const WsTypes::WsShooterPresetConfigT& rc_newConfig)
{
    a_presetConfig = rc_newConfig;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

