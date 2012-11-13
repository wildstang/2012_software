#include "WsAutoDriveProgram.hh"
#include "WsLogger.hh"
#include "WsAutonomousProgram.hh"
#include "Timer.h"
#include "WsInputFacade.hh"
#include "WsOutputFacade.hh"
#include "WsDataIndicationFacade.hh"
#include "WsConfigFacade.hh"
#include "WsTypes.hh"

const float OI_FLYWHEEL_POT_MANUAL = 2.79f;
const float OI_FLYWHEEL_POT_PRESET = 1.65;
const float OI_FLYWHEEL_POT_OFF = 0.01f;

WsAutoDriveProgram::WsAutoDriveProgram(void)
    : WsAutonomousProgram("WsAutoDriveProgram")
    , a_done(false)
    , a_gyro_at_start(0.0f)
    , a_currentState(WS_AUTO_DRIVE_STATE_START)
    , ap_stateTimer(NULL)
{
    WS_LOG_TRACE("Begin");
    
    ::memset(&a_configVals, 0x00, sizeof(a_configVals));
    
    ap_stateTimer = new Timer();
    ap_stateTimer->Stop();
    ap_stateTimer->Reset();
    
    WS_LOG_TRACE("End");
}

WsAutoDriveProgram::~WsAutoDriveProgram(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_stateTimer;
    ap_stateTimer = NULL;
    
    WS_LOG_TRACE("End");
}

void WsAutoDriveProgram::init(void)
{
    WsAutonomousProgram::init();
    
    WsConfigFacade::instance()->getAutoDriveConfig(a_configVals);
    
    //Ensure Hood is up
    WsTypes::WsHoodCylinderStateType hood_state = WsTypes::WS_HOOD_CYLINDER_INVALID;
    WsOutputFacade::instance()->getHoodState(hood_state);
    if (WsTypes::WS_HOOD_CYLINDER_DOWN == hood_state)
    {
        WsInputFacade::instance()->setHoodButtonState(true);
    }
    
    a_gyro_at_start = 0.0f;
    WsInputFacade::instance()->getRobotHeadingGyroAngle(a_gyro_at_start);
    
    ap_stateTimer->Start();
}

bool
WsAutoDriveProgram::run(void)
{
    WS_LOG_TRACE("Begin");
    
    INT32 left_enc;
    WsInputFacade::instance()->getLeftDriveEncoderCount(left_enc);
    
    INT32 right_enc;
    WsInputFacade::instance()->getRightDriveEncoderCount(right_enc);
    
    bool is_on_target;
    WsDataIndicationFacade::instance()->getTurretAtTargetPositionFlag(is_on_target);
    
    WsAutoDriveStateType new_state = a_currentState;
    bool shoot_button = false;
    float flywheel_state = OI_FLYWHEEL_POT_PRESET;
    WsTypes::WsShooterPresetType preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
    bool shoot_override_button = false;
    bool escalator_button = false;
    float drive_throttle = 0.0f;
    float drive_heading = 0.0f;
    bool accumulator_motor_button = false;
    bool accumulator_actuator_button = false;
    
    switch (a_currentState)
    {
        case WS_AUTO_DRIVE_STATE_START:
        {
            new_state = WS_AUTO_DRIVE_STATE_DRIVE_TO_COOP_BRIDGE;
            break;
        }
        case WS_AUTO_DRIVE_STATE_DRIVE_TO_COOP_BRIDGE:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_OFF;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = a_configVals.ws_auto_drive_coop_bridge_throttle;
            drive_heading = a_configVals.ws_auto_drive_coop_bridge_heading;
            accumulator_motor_button = false;
            accumulator_actuator_button = false;
            
            if ((abs(left_enc) >= a_configVals.ws_auto_drive_coop_bridge_encoder_target) ||
                (abs(right_enc) >= a_configVals.ws_auto_drive_coop_bridge_encoder_target) ||
                (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_drive_coop_bridge_timeout)))
            {
                new_state = WS_AUTO_DRIVE_STATE_WAIT_FOR_COAST;
                
            }
            break;
        }
        case WS_AUTO_DRIVE_STATE_WAIT_FOR_COAST:
        {
        
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_OFF;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.00f;
            accumulator_motor_button = false;
            accumulator_actuator_button = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_drive_wait_for_coast_timeout))
            {
                accumulator_actuator_button = true;
                new_state = WS_AUTO_DRIVE_STATE_PUT_DOWN_COOP_BRIDGE;
            }
            break;
        }
        case WS_AUTO_DRIVE_STATE_PUT_DOWN_COOP_BRIDGE:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            accumulator_motor_button = true;
            accumulator_actuator_button = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_drive_put_down_coop_bridge_timeout))
            {
                new_state = WS_AUTO_DRIVE_STATE_DRIVE_BACK_TO_SHOOTING_POSITION;
            }
            
            break;
        }
        case WS_AUTO_DRIVE_STATE_DRIVE_BACK_TO_SHOOTING_POSITION:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = false;
            escalator_button = true;
            drive_throttle = a_configVals.ws_auto_drive_shooting_position_throttle;
            drive_heading = a_configVals.ws_auto_drive_shooting_position_heading;
            accumulator_motor_button = true;
            accumulator_actuator_button = false;
            
            if ((abs(left_enc) >= a_configVals.ws_auto_drive_shooting_position_encoder_target) ||
                (abs(right_enc) >= a_configVals.ws_auto_drive_shooting_position_encoder_target) ||
                (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_drive_shooting_position_timeout)))
            {
                new_state = WS_AUTO_DRIVE_STATE_WAIT_FOR_ANGLE_CALC;
            }
            
            break;
        }
        case WS_AUTO_DRIVE_STATE_WAIT_FOR_ANGLE_CALC:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_NONE;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            accumulator_motor_button = true;
            accumulator_actuator_button = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_drive_wait_for_angle_timeout))
            {
                new_state = WS_AUTO_DRIVE_STATE_ADJUST_PRESET_FOR_ANGLE;
            }
        } break;
        case WS_AUTO_DRIVE_STATE_ADJUST_PRESET_FOR_ANGLE:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_NONE;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            accumulator_motor_button = true;
            accumulator_actuator_button = false;
            //Get the gyro angle
            float angle = 0.0f;
            WsInputFacade::instance()->getRobotHeadingGyroAngle(angle);
            angle = angle - a_gyro_at_start;
            //Read the shooter presets
            WsTypes::WsShooterPresetConfigT current_presets;
            ::memset(&current_presets, 0x00, sizeof(current_presets));
            WsConfigFacade::instance()->getShooterPresetConfig(current_presets);
            float oneDegreeVoltage = ((current_presets.turret_key_forward_voltage - current_presets.turret_key_reverse_voltage) / (a_configVals.ws_auto_drive_angle_between_facing_and_not_facing));
            float offset = oneDegreeVoltage * angle ;
            float turret = current_presets.turret_auton_voltage + offset;
            
            WS_LOG_ALWAYS("gyro:%f gyro_at_start:%f oneDeg: %f offset: %f newTurret:%f , prevTurret: %f ", angle, a_gyro_at_start, oneDegreeVoltage, offset , turret, current_presets.turret_auton_voltage);
            //Adjust the auton turret pot value for the offset angle
            current_presets.turret_auton_voltage = turret;
            current_presets.flywheel_auton_speed = a_configVals.ws_auto_drive_second_flywheel_setpoint;
            //Push the presets back in
            WsConfigFacade::instance()->setShooterPresetConfig(current_presets);
            
            new_state = WS_AUTO_DRIVE_STATE_WAIT_FOR_TURRET_STATE;
            
        } break;
        case WS_AUTO_DRIVE_STATE_WAIT_FOR_TURRET_STATE:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            accumulator_motor_button = true;
            accumulator_actuator_button = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.ws_auto_drive_wait_for_turret_timeout))
            {
                new_state = WS_AUTO_DRIVE_STATE_WAIT_FOR_FLYWHEEL_1;
                
            }
        } break;
        case WS_AUTO_DRIVE_STATE_WAIT_FOR_FLYWHEEL_1:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            accumulator_motor_button = true;
            accumulator_actuator_button = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_spin_fly_time_1))
            {
                new_state = WS_AUTO_DRIVE_STATE_SHOOT_1;
                accumulator_actuator_button = true;
            }
            
            break;
        }
        case WS_AUTO_DRIVE_STATE_SHOOT_1:
        {
            shoot_button = true;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = true;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            accumulator_motor_button = false;
            accumulator_actuator_button = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_shoot_1_time))
            {
                new_state = WS_AUTO_DRIVE_STATE_WAIT_FOR_FLYWHEEL_2;
            }
            
            break;
        }
        case WS_AUTO_DRIVE_STATE_WAIT_FOR_FLYWHEEL_2:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            accumulator_motor_button = false;
            accumulator_actuator_button = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_spin_fly_time_2))
            {
                new_state = WS_AUTO_DRIVE_STATE_SHOOT_2;
            }
            
            break;
        }
        case WS_AUTO_DRIVE_STATE_SHOOT_2:
        {
            shoot_button = true;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = true;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            accumulator_motor_button = false;
            accumulator_actuator_button = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_shoot_2_time))
            {
                new_state = WS_AUTO_DRIVE_STATE_WAIT_FOR_FLYWHEEL_3;
            }
            
            break;
        }
        case WS_AUTO_DRIVE_STATE_WAIT_FOR_FLYWHEEL_3:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            accumulator_motor_button = false;
            accumulator_actuator_button = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_spin_fly_time_3))
            {
                new_state = WS_AUTO_DRIVE_STATE_SHOOT_3;
            }
            
            break;
        }
        case WS_AUTO_DRIVE_STATE_SHOOT_3:
        {
            shoot_button = true;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = true;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.0f;
            accumulator_motor_button = false;
            accumulator_actuator_button = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_shoot_3_time))
            {
                new_state = WS_AUTO_DRIVE_STATE_DONE;
            }
            
            break;
        }
        case WS_AUTO_DRIVE_STATE_DONE:
        {
            a_done = true;
            
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_OFF;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.00f;
            accumulator_motor_button = false;
            accumulator_actuator_button = false;
            
            WS_LOG_ALWAYS("Done");
            
            bool acceleration = true;
            WsInputFacade::instance()->setDriveBaseAccelerationButtonState(acceleration);
            
            
            break;
        }
        case WS_AUTO_DRIVE_STATE_NUM:
        case WS_AUTO_DRIVE_STATE_NONE:
        default:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_OFF;
            preset = WsTypes::WS_SHOOTER_PRESET_AUTON;
            shoot_override_button = false;
            escalator_button = false;
            drive_throttle = 0.0f;
            drive_heading = 0.00f;
            accumulator_motor_button = false;
            accumulator_actuator_button = false;
            
            WS_LOG_ERROR("Bad case in autonomous program!");
            WS_LOG_ERROR("Switching to done");
            
            new_state = WS_AUTO_DRIVE_STATE_DONE;
            break;
        }
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
    WsInputFacade::instance()->setDriverThrottleValue(drive_throttle);
    WsInputFacade::instance()->setDriverHeadingValue(drive_heading);
    WsInputFacade::instance()->setAccumulatorMotorButtonState(accumulator_motor_button);
    WsInputFacade::instance()->setAccumulatorDeployButtonState(accumulator_actuator_button);
    
    WS_LOG_NOTICE("autoDrive_driveThrottle = %.05f", (double)drive_throttle);
    WS_LOG_NOTICE("if_leftEncoderCount = %d", left_enc);
    WS_LOG_NOTICE("if_rightEncoderCount = %d", right_enc);
    
    
    WS_LOG_TRACE("End");
    return true;
}

void
WsAutoDriveProgram::clear(void)
{
    a_currentState = WS_AUTO_DRIVE_STATE_START;
    
    ap_stateTimer->Stop();
    ap_stateTimer->Reset();
    
    a_done = false;
}

bool
WsAutoDriveProgram::isDone(void)
{
    return a_done;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

