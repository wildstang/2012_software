#include "WsAutoShoot3Program.hh"
#include "WsLogger.hh"
#include "WsAutonomousProgram.hh"
#include "Timer.h"
#include "WsInputFacade.hh"
#include "WsOutputFacade.hh"
#include "WsConfigFacade.hh"
#include "WsTypes.hh"

const float OI_FLYWHEEL_POT_MANUAL = 2.79f;
const float OI_FLYWHEEL_POT_PRESET = 1.65f;
const float OI_FLYWHEEL_POT_OFF = 0.01f;


WsAutoShoot3Program::WsAutoShoot3Program(void)
    : WsAutonomousProgram("WsAutoShoot3Program")
    , a_done(false)
    , a_currentState(WS_AUTO_SHOOT_STATE_PRIME_FLY_1)
    , ap_stateTimer(NULL)
{
    WS_LOG_TRACE("Begin");
    
    ::memset(&a_configVals, 0x00, sizeof(a_configVals));
    
    ap_stateTimer = new Timer();
    ap_stateTimer->Stop();
    ap_stateTimer->Reset();
    
    WS_LOG_TRACE("End");
}

WsAutoShoot3Program::~WsAutoShoot3Program(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_stateTimer;
    ap_stateTimer = NULL;
    
    WS_LOG_TRACE("End");
}

void WsAutoShoot3Program::init(void)
{
    WsConfigFacade::instance()->getAutoShoot3Config(a_configVals);
    
    //Ensure Hood is up
    WsTypes::WsHoodCylinderStateType hood_state = WsTypes::WS_HOOD_CYLINDER_INVALID;
    WsOutputFacade::instance()->getHoodState(hood_state);
    if (WsTypes::WS_HOOD_CYLINDER_DOWN == hood_state)
    {
        WsInputFacade::instance()->setHoodButtonState(true);
    }
    
    WsAutonomousProgram::init();
    
    ap_stateTimer->Start();
}

bool
WsAutoShoot3Program::run(void)
{
    WS_LOG_TRACE("Begin");
    
    WsAutoShoot3ProgramStateType new_state = a_currentState;
    bool shoot_button = false;
    float flywheel_state = OI_FLYWHEEL_POT_PRESET;
    WsTypes::WsShooterPresetType preset = WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD;
    bool shoot_override_button = false;
    bool escalator_button = false;
    bool accum_state = false;
    
    switch (a_currentState)
    {
        case WS_AUTO_SHOOT_STATE_PRIME_FLY_1:
        {
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            shoot_button = false;
            preset = WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD;
            shoot_override_button = false;
            escalator_button = false;
            accum_state = false;
            
            //Speed: 2.8
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_prime_fly_1_time))
            {
                ap_stateTimer->Reset();
                new_state = WS_AUTO_SHOOT_STATE_SPIN_FLY_1;
                float speed = 0.0;
                WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
                WS_LOG_ERROR("End of STATE PRIME FLY 1 Speed: %.5f", speed);
                
            }
            break;
        }
        case WS_AUTO_SHOOT_STATE_SPIN_FLY_1:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD;
            shoot_override_button = false;
            escalator_button = false;
            accum_state = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_spin_fly_time_1))
            {
                ap_stateTimer->Reset();
                new_state = WS_AUTO_SHOOT_STATE_SHOOT_1;
                float speed = 0.0;
                WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
                WS_LOG_ERROR("End of STATE SPIN FLY 1 Speed: %.5f", speed);
                
            }
            break;
        }
        case WS_AUTO_SHOOT_STATE_SHOOT_1:
        {
            shoot_button = true;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD;
            shoot_override_button = true;
            escalator_button = false;
            accum_state = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_shoot_1_time))
            {
                ap_stateTimer->Reset();
                new_state = WS_AUTO_SHOOT_STATE_PRIME_FLY_2;
                float speed = 0.0;
                WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
                WS_LOG_ERROR("End of STATE SHOOT 1 Speed: %.5f", speed);
                
            }
            break;
        }
        case WS_AUTO_SHOOT_STATE_PRIME_FLY_2:
        {
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            shoot_button = false;
            preset = WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD;
            shoot_override_button = false;
            escalator_button = false;
            accum_state = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_prime_fly_2_time))
            {
                ap_stateTimer->Reset();
                new_state = WS_AUTO_SHOOT_STATE_SPIN_FLY_2;
                float speed = 0.0;
                WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
                WS_LOG_ERROR("End of STATE PRIME FLY 2 Speed: %.5f", speed);
                
            }
            break;
        }
        case WS_AUTO_SHOOT_STATE_SPIN_FLY_2:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD;
            shoot_override_button = false;
            escalator_button = false;
            accum_state = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_spin_fly_time_2))
            {
                ap_stateTimer->Reset();
                new_state = WS_AUTO_SHOOT_STATE_SHOOT_2;
                float speed = 0.0;
                WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
                WS_LOG_ERROR("End of STATE SPIN FLY 2 Speed: %.5f", speed);
                
            }
            break;
        }
        case WS_AUTO_SHOOT_STATE_SHOOT_2:
        {
            shoot_button = true;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD;
            shoot_override_button = true;
            escalator_button = true;
            accum_state = false;
            
            float speed = 0.0;
            WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
            
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_shoot_2_time))
            {
                ap_stateTimer->Reset();
                
                new_state = WS_AUTO_SHOOT_STATE_PRIME_FLY_3;
                float speed = 0.0;
                WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
                WS_LOG_ERROR("End of STATE SHOOT 2 Speed: %.5f", speed);
                
            }
            break;
        }
        case WS_AUTO_SHOOT_STATE_PRIME_FLY_3:
        {
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            shoot_button = false;
            preset = WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD;
            shoot_override_button = false;
            escalator_button = false;
            accum_state = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_prime_fly_2_time))
            {
                ap_stateTimer->Reset();
                new_state = WS_AUTO_SHOOT_STATE_SPIN_FLY_3;
                float speed = 0.0;
                WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
                WS_LOG_ERROR("End of STATE PRIME FLY 2 Speed: %.5f", speed);
                
            }
            break;
        }
        case WS_AUTO_SHOOT_STATE_SPIN_FLY_3:
        {
            shoot_button = false;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD;
            shoot_override_button = false;
            escalator_button = false;
            accum_state = false;
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_spin_fly_time_2))
            {
                ap_stateTimer->Reset();
                new_state = WS_AUTO_SHOOT_STATE_SHOOT_3;
                float speed = 0.0;
                WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
                WS_LOG_ERROR("End of STATE SPIN FLY 2 Speed: %.5f", speed);
                
            }
            break;
        }
        case WS_AUTO_SHOOT_STATE_SHOOT_3:
        {
            shoot_button = true;
            flywheel_state = OI_FLYWHEEL_POT_PRESET;
            preset = WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD;
            shoot_override_button = true;
            escalator_button = true;
            accum_state = false;
            
            float speed = 0.0;
            WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
            
            
            if (ap_stateTimer->HasPeriodPassed(a_configVals.shoot_state_shoot_2_time))
            {
                ap_stateTimer->Reset();
                
                new_state = WS_AUTO_SHOOT_STATE_DONE;
                float speed = 0.0;
                WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
                WS_LOG_ERROR("End of STATE SHOOT 2 Speed: %.5f", speed);
                
            }
            break;
        }
        case WS_AUTO_SHOOT_STATE_DONE:
        {
            ap_stateTimer->Stop();
            shoot_button = false;
            flywheel_state = 0.00f;
            preset = WsTypes::WS_SHOOTER_PRESET_NONE;
            shoot_override_button = false;
            escalator_button = false;
            accum_state = false;
            
            WS_LOG_ERROR("STATE DONE");
            
            // We're done. This will continue to run, but isDone() will return true.
            a_done = true;
            break;
        }
        case WS_AUTO_SHOOT_STATE_NUM:
        case WS_AUTO_SHOOT_STATE_NONE:
        default:
        {
            WS_LOG_ERROR("BAD AUTO PROGRAM STATE!");
            WS_LOG_ERROR("Switching to done.");
            
            new_state = WS_AUTO_SHOOT_STATE_DONE;
            break;
        }
    }
    
    a_currentState = new_state;
    
    WsInputFacade::instance()->setBallShootButtonState(shoot_button);
    WsInputFacade::instance()->setFlywheelStatePotValue(flywheel_state);
    WsInputFacade::instance()->setShooterPreset(preset);
    WsInputFacade::instance()->setBallShootOverrideButtonState(shoot_override_button);
    WsInputFacade::instance()->setEscalatorUpButtonState(escalator_button);
    WsInputFacade::instance()->setAccumulatorDeployButtonState(accum_state);
    
    // We shouldn't be moving!!
    float not_moving = 0.0f;
    WsInputFacade::instance()->setDriverThrottleValue(not_moving);
    WsInputFacade::instance()->setDriverHeadingValue(not_moving);
    
    WS_LOG_TRACE("End");
    return true;
}

void
WsAutoShoot3Program::clear(void)
{
    a_currentState = WS_AUTO_SHOOT_STATE_PRIME_FLY_1;
    
    ap_stateTimer->Stop();
    ap_stateTimer->Reset();
    
    a_done = false;
}

bool
WsAutoShoot3Program::isDone(void)
{
    return a_done;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

