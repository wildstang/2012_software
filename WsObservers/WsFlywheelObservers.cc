#include "WsFlywheelObservers.hh"
#include "WsInputFacade.hh"
#include "WsFlywheel.hh"
#include "WsLogger.hh"
#include "WsConfigFacade.hh"
#include <math.h>

#include "Timer.h"
#include "DriverStation.h"

#define PRESET_DEBOUNCE_LOWER_LIMIT 0.1f
#define PRESET_DEBOUNCE_UPPER_LIMIT 1.0f

//-----------------------------------------------------------------------------
// WsFlywheelFloodLightStateObserver
//-----------------------------------------------------------------------------

WsFlywheelFloodLightStateObserver::WsFlywheelFloodLightStateObserver(WsFlywheel* p_shooterFlywheel)
    : WsObserver("WsFlywheelFloodLightStateObserver")
    , ap_subsystem(p_shooterFlywheel)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getFloodLightButtonState(a_currentState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewFloodlightState(a_currentState);
}

WsFlywheelFloodLightStateObserver::~WsFlywheelFloodLightStateObserver(void)
{
    bool new_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getFloodLightButtonState(new_state);
    p_subject->detach(this);
}

void
WsFlywheelFloodLightStateObserver::update(void)
{
    bool new_state = false;
    WsInputFacade::instance()->getFloodLightButtonState(new_state);
    WS_LOG_DEBUG("a_currentState = %d, new_state = %d",
                 a_currentState,
                 new_state);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewFloodlightState(new_state);
    
    // Note the new state
    a_currentState = new_state;
}

//-----------------------------------------------------------------------------
// WsFlywheelFreshImageObserver
//-----------------------------------------------------------------------------

WsFlywheelFreshImageObserver::WsFlywheelFreshImageObserver(WsFlywheel* p_shooterFlywheel)
    : WsObserver("WsFlywheelFreshImageObserver")
    , ap_subsystem(p_shooterFlywheel)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getFreshImageFlag(a_currentState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewFreshImageFlag(a_currentState);
}

WsFlywheelFreshImageObserver::~WsFlywheelFreshImageObserver(void)
{
    bool new_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getFreshImageFlag(new_state);
    p_subject->detach(this);
}

void
WsFlywheelFreshImageObserver::update(void)
{
    bool new_state = false;
    WsInputFacade::instance()->getFreshImageFlag(new_state);
    
    WS_LOG_DEBUG("a_currentState = %d, new_state = %d",
                 a_currentState,
                 new_state);
    a_currentState = new_state;
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewFreshImageFlag(a_currentState);
}

//-----------------------------------------------------------------------------
// WsFlywheelLocationSpeedMappingObserver
//-----------------------------------------------------------------------------

WsFlywheelLocationSpeedMappingObserver::WsFlywheelLocationSpeedMappingObserver(WsFlywheel* p_flywheelSubsystem)
    : WsObserver("WsFlywheelLocationSpeedMappingObserver")
    , ap_subsystem(p_flywheelSubsystem)
    , a_currentConfig()
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsConfigFacade::instance()->getFlywheelLocationSpeedMapping(a_currentConfig);
    p_subject->attach(this);
    
    // Push the current config into the subsystem
    ap_subsystem->acceptNewLocationSpeedMapping(a_currentConfig);
}

WsFlywheelLocationSpeedMappingObserver::~WsFlywheelLocationSpeedMappingObserver(void)
{
    WsTypes::WsLocationSpeedMappingType new_config;
    WsSubject* p_subject =
        WsConfigFacade::instance()->getFlywheelLocationSpeedMapping(new_config);
    p_subject->detach(this);
}

void
WsFlywheelLocationSpeedMappingObserver::update(void)
{
    WsTypes::WsLocationSpeedMappingType new_config;
    WsConfigFacade::instance()->getFlywheelLocationSpeedMapping(new_config);
    
    WS_LOG_DEBUG("a_currentConfig.fender_fws = %.05f, new_config.fender_fws = %.05f",
                 a_currentConfig.fender_fws,
                 new_config.fender_fws);
    WS_LOG_DEBUG("a_currentConfig.key_bottom_fws = %.05f, new_config.key_bottom_fws = %.05f",
                 a_currentConfig.key_bottom_fws,
                 new_config.key_bottom_fws);
    WS_LOG_DEBUG("a_currentConfig.key_middle_fws = %.05f, new_config.key_middle_fws = %.05f",
                 a_currentConfig.key_middle_fws,
                 new_config.key_middle_fws);
    WS_LOG_DEBUG("a_currentConfig.key_top_fws = %.05f, new_config.key_top_fws = %.05f",
                 a_currentConfig.key_top_fws,
                 new_config.key_top_fws);
                 
    // Push the current config into the subsystem
    ap_subsystem->acceptNewLocationSpeedMapping(new_config);
    
    // Note the new config
    a_currentConfig = new_config;
}

//-----------------------------------------------------------------------------
// WsFlywheelManualPotObserver
//-----------------------------------------------------------------------------

WsFlywheelManualPotObserver::WsFlywheelManualPotObserver(WsFlywheel* p_shooterFlywheel)
    : WsObserver("WsFlywheelManualPotObserver")
    , ap_subsystem(p_shooterFlywheel)
    , a_currentFlywheelManualPotValue(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelManualPot(a_currentFlywheelManualPotValue);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewFlywheelManualPotValue(a_currentFlywheelManualPotValue);
}

WsFlywheelManualPotObserver::~WsFlywheelManualPotObserver(void)
{
    float value = 0.0;
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelManualPot(value);
    p_subject->detach(this);
}

void
WsFlywheelManualPotObserver::update(void)
{
    WsInputFacade::instance()->getFlywheelManualPot(a_currentFlywheelManualPotValue);
    WS_LOG_DEBUG("a_currentFlywheelManualPot = %.05f",
                 a_currentFlywheelManualPotValue);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewFlywheelManualPotValue(a_currentFlywheelManualPotValue);
}

//-----------------------------------------------------------------------------
// WsFlywheelStatePotValueObserver
//-----------------------------------------------------------------------------

WsFlywheelStatePotValueObserver::WsFlywheelStatePotValueObserver(WsFlywheel* p_shooterFlywheel)
    : WsObserver("WsFlywheelStatePotValueObserver")
    , ap_subsystem(p_shooterFlywheel)
    , a_currentValue(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelStatePotValue(a_currentValue);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewFlywheelStatePotValue(a_currentValue);
}

WsFlywheelStatePotValueObserver::~WsFlywheelStatePotValueObserver(void)
{
    float value = 0.0;
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelStatePotValue(value);
    p_subject->detach(this);
}

void
WsFlywheelStatePotValueObserver::update(void)
{
    float new_value = 0.0f;
    WsInputFacade::instance()->getFlywheelStatePotValue(new_value);
    
    WS_LOG_ALWAYS("a_currentValue = %.05f, new_value = %.05f",
                  a_currentValue,
                  new_value);
                  
    // Push the current value into the subsystem
    ap_subsystem->acceptNewFlywheelStatePotValue(new_value);
    
    // Note the new value
    a_currentValue = new_value;
}

//-----------------------------------------------------------------------------
// WsFlywheelPidConfigObserver
//-----------------------------------------------------------------------------
WsFlywheelPidConfigObserver::WsFlywheelPidConfigObserver(WsFlywheel* p_flywheelSubsystem)
    : WsObserver("WsFlywheelPidConfigObserver")
    , ap_subsystem(p_flywheelSubsystem)
    , a_currentConfig()
{
    // Zero-out the current config
    ::memset(&a_currentConfig, 0x00, sizeof(a_currentConfig));
    
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsConfigFacade::instance()->getFlywheelPidConfig(a_currentConfig);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewPidConfig(a_currentConfig);
}

WsFlywheelPidConfigObserver::~WsFlywheelPidConfigObserver(void)
{
    WsTypes::WsPidConfigurationDataT new_config;
    ::memset(&new_config, 0x00, sizeof(new_config));
    
    WsSubject* p_subject =
        WsConfigFacade::instance()->getFlywheelPidConfig(new_config);
    p_subject->detach(this);
}

void
WsFlywheelPidConfigObserver::update(void)
{
    WsTypes::WsPidConfigurationDataT new_config;
    ::memset(&new_config, 0x00, sizeof(new_config));
    
    WsConfigFacade::instance()->getFlywheelPidConfig(new_config);
    WS_LOG_DEBUG("FLYWHEEL FINE PID");
    WS_LOG_DEBUG("a_currentConfig.k_p = %.05f, new_config.k_p = %.05f", a_currentConfig.k_p, new_config.k_p);
    WS_LOG_DEBUG("a_currentConfig.k_i = %.05f, new_config.k_i = %.05f", a_currentConfig.k_i, new_config.k_i);
    WS_LOG_DEBUG("a_currentConfig.k_d = %.05f, new_config.k_d = %.05f", a_currentConfig.k_d, new_config.k_d);
    WS_LOG_DEBUG("a_currentConfig.error_epsilon = %.05f, new_config.error_epsilon = %.05f", a_currentConfig.error_epsilon, new_config.error_epsilon);
    WS_LOG_DEBUG("a_currentConfig.static_epsilon = %.05f, new_config.static_epsilon = %.05f", a_currentConfig.static_epsilon, new_config.static_epsilon);
    WS_LOG_DEBUG("a_currentConfig.max_integral = %.05f, new_config.max_integral = %.05f", a_currentConfig.max_integral, new_config.max_integral);
    WS_LOG_DEBUG("a_currentConfig.error_increment = %.05f, new_config.error_increment = %.05f", a_currentConfig.error_increment, new_config.error_increment);
    WS_LOG_DEBUG("a_currentConfig.integral_error_thresh = %.05f, new_config.integral_error_thresh = %.05f", a_currentConfig.integral_error_thresh, new_config.integral_error_thresh);
    WS_LOG_DEBUG("a_currentConfig.differentiator_band_limit = %.05f, new_config.differentiator_band_limit = %.05f", a_currentConfig.differentiator_band_limit, new_config.differentiator_band_limit);
    WS_LOG_DEBUG("a_currentConfig.max_output = %.05f, new_config.max_output = %.05f", a_currentConfig.max_output, new_config.max_output);
    WS_LOG_DEBUG("a_currentConfig.min_output = %.05f, new_config.min_output = %.05f", a_currentConfig.min_output, new_config.min_output);
    WS_LOG_DEBUG("a_currentConfig.max_input = %.05f, new_config.max_input = %.05f", a_currentConfig.max_input, new_config.max_input);
    WS_LOG_DEBUG("a_currentConfig.min_input = %.05f, new_config.min_input = %.05f", a_currentConfig.min_input, new_config.min_input);
    WS_LOG_DEBUG("a_currentConfig.min_on_target_time = %.05f, new_config.min_on_target_time = %.05f", a_currentConfig.min_on_target_time, new_config.min_on_target_time);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewPidConfig(new_config);
    
    // Note the new value
    a_currentConfig = new_config;
}


//-----------------------------------------------------------------------------
// WsFlywheelRequestedPresetObserver
//-----------------------------------------------------------------------------
WsFlywheelRequestedPresetObserver::WsFlywheelRequestedPresetObserver(WsFlywheel* p_subsystem)
    : WsObserver("WsFlywheelRequestedPresetObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(WsTypes::WS_SHOOTER_PRESET_NONE)
    , a_requestedValue(WsTypes::WS_SHOOTER_PRESET_NONE)
    , ap_debounceTimer(NULL)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getShooterPreset(a_currentValue);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewRequestedPreset(a_currentValue);
    
    // Initialize the timer
    ap_debounceTimer = new Timer;
    ap_debounceTimer->Start();
    ap_debounceTimer->Reset();
}

WsFlywheelRequestedPresetObserver::~WsFlywheelRequestedPresetObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getShooterPreset(a_currentValue);
    p_subject->detach(this);
}

void
WsFlywheelRequestedPresetObserver::update(void)
{
    WsTypes::WsShooterPresetType new_value = WsTypes::WS_SHOOTER_PRESET_NONE;
    WsInputFacade::instance()->getShooterPreset(new_value);
    
    //
    // @TODO:  HACK!!
    //  This is less than ideal, but we have no time...
    //  If we are in autonomous, don't debounce the buttons, just push the value
    //  through to the subsystem.  If we write "bouncy" autonomous code,
    //  then shame on us.
    //
    DriverStation* p_ds = DriverStation::GetInstance();
    if (true == p_ds->IsAutonomous())
    {
        if (WsTypes::WS_SHOOTER_PRESET_NONE != new_value)
        {
            // Push the current value into the subsystem
            a_currentValue = new_value;
            a_requestedValue = new_value;
            ap_subsystem->acceptNewRequestedPreset(a_currentValue);
        }
        
        // We're done, don't process anything else!
        return;
    }
    
    // Debounce test: If a preset change from (NONE) to (SOMETHING) is requested, note the requested state
    //  and start a timer. Next time the preset changes from (SOMETHING) to (NONE), check to see if enough time,
    //  but not too much time has passed. If the requested value passes the test, pass it on to the cubsystem
    //  and note the new value.
    
    if (WsTypes::WS_SHOOTER_PRESET_NONE != new_value)
    {
        WS_LOG_DEBUG("New preset requested: a_currentValue = %d, a_requestedValue = %d",
                     a_currentValue,
                     a_requestedValue);
                     
        a_requestedValue = new_value;
        ap_debounceTimer->Reset();
    }
    else if (WsTypes::WS_SHOOTER_PRESET_NONE == new_value)
    {
        WS_LOG_DEBUG("Transition to (NONE). Debouncing...");
        if ((true == ap_debounceTimer->HasPeriodPassed(PRESET_DEBOUNCE_LOWER_LIMIT)) &&
            (false == ap_debounceTimer->HasPeriodPassed(PRESET_DEBOUNCE_UPPER_LIMIT)))
        {
            WS_LOG_DEBUG("Debounce test passed: a_currentValue = %d, new_value = %d",
                         a_currentValue,
                         a_requestedValue);
                         
            // Note the new value
            a_currentValue = a_requestedValue;
            
            // Push the current value into the subsystem
            ap_subsystem->acceptNewRequestedPreset(a_currentValue);
            
            // Reset the timer
            ap_debounceTimer->Reset();
        }
        else
        {
            WS_LOG_ERROR("Debounce failed. Time either too long or too short.");
            
            // Reset the timer
            ap_debounceTimer->Reset();
        }
    }
    else
    {
        WS_LOG_ERROR("Bad preset value: %d", new_value);
    }
}


//-----------------------------------------------------------------------------
// WsFlywheelPresetConfigObserver
//-----------------------------------------------------------------------------
WsFlywheelPresetConfigObserver::WsFlywheelPresetConfigObserver(WsFlywheel* p_subsystem)
    : WsObserver("WsFlywheelPresetConfigObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue()
{
    ::memset(&a_currentValue, 0x00, sizeof(a_currentValue));
    
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsConfigFacade::instance()->getShooterPresetConfig(a_currentValue);
    p_subject->attach(this);
}

WsFlywheelPresetConfigObserver::~WsFlywheelPresetConfigObserver(void)
{
    WsSubject* p_subject =
        WsConfigFacade::instance()->getShooterPresetConfig(a_currentValue);
    p_subject->detach(this);
}

void
WsFlywheelPresetConfigObserver::update(void)
{
    WsTypes::WsShooterPresetConfigT new_value;
    ::memset(&new_value, 0x00, sizeof(new_value));
    
    WsConfigFacade::instance()->getShooterPresetConfig(new_value);
    
    WS_LOG_DEBUG("current flywheel_key_forward_speed = %f, new flywheel_key_forward_speed = %f",
                 a_currentValue.flywheel_key_forward_speed,
                 new_value.flywheel_key_forward_speed);
    WS_LOG_DEBUG("current flywheel_key_forward_speed = %f, new flywheel_key_forward_speed = %f",
                 a_currentValue.flywheel_key_forward_speed,
                 new_value.flywheel_key_forward_speed);
    WS_LOG_DEBUG("current flywheel_key_reverse_speed = %f, new flywheel_key_reverse_speed = %f",
                 a_currentValue.flywheel_key_reverse_speed,
                 new_value.flywheel_key_reverse_speed);
    WS_LOG_DEBUG("current flywheel_key_reverse_speed = %f, new flywheel_key_reverse_speed = %f",
                 a_currentValue.flywheel_key_reverse_speed,
                 new_value.flywheel_key_reverse_speed);
    WS_LOG_DEBUG("current flywheel_fender_left_speed = %f, new flywheel_fender_left_speed = %f",
                 a_currentValue.flywheel_fender_left_speed,
                 new_value.flywheel_fender_left_speed);
    WS_LOG_DEBUG("current flywheel_fender_left_speed = %f, new flywheel_fender_left_speed = %f",
                 a_currentValue.flywheel_fender_left_speed,
                 new_value.flywheel_fender_left_speed);
    WS_LOG_DEBUG("current flywheel_fender_right_speed = %f, new flywheel_fender_right_speed = %f",
                 a_currentValue.flywheel_fender_right_speed,
                 new_value.flywheel_fender_right_speed);
    WS_LOG_DEBUG("current flywheel_fender_right_speed = %f, new flywheel_fender_right_speed = %f",
                 a_currentValue.flywheel_fender_right_speed,
                 new_value.flywheel_fender_right_speed);
    WS_LOG_DEBUG("current flywheel_auton_speed = %f, new flywheel_auton_speed = %f",
                 a_currentValue.flywheel_auton_speed,
                 new_value.flywheel_auton_speed);
    WS_LOG_DEBUG("current flywheel_home_speed = %f, new flywheel_auton_speed = %f",
                 a_currentValue.flywheel_auton_speed,
                 new_value.flywheel_auton_speed);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewPresetConfig(new_value);
    
    // Note the new value
    a_currentValue = new_value;
}

//-----------------------------------------------------------------------------
// WsFlywheelSpeedCompensationFactorObserver
//-----------------------------------------------------------------------------
WsFlywheelSpeedCompensationFactorObserver::
WsFlywheelSpeedCompensationFactorObserver(WsFlywheel* p_subsystem)
    : WsObserver("WsFlywheelSpeedCompensationFactorObserver")
    , ap_subsystem(p_subsystem)
    , a_currentSwitchState(false)
    , a_currentPotValue(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelSpeedCompensationPotValue(a_currentPotValue);
    p_subject->attach(this);
    p_subject =
        WsInputFacade::instance()->getFlywheelSpeedCompensationEnableSwitchState(a_currentSwitchState);
    p_subject->attach(this);
}

WsFlywheelSpeedCompensationFactorObserver::
~WsFlywheelSpeedCompensationFactorObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelSpeedCompensationPotValue(a_currentPotValue);
    p_subject->detach(this);
    p_subject =
        WsInputFacade::instance()->getFlywheelSpeedCompensationEnableSwitchState(a_currentSwitchState);
    p_subject->detach(this);
}

void
WsFlywheelSpeedCompensationFactorObserver::update(void)
{
    float new_pot_value = 0.0f;
    bool  new_switch_state = false;
    float percent_change = 1.0f;
    WsInputFacade::instance()->getFlywheelSpeedCompensationPotValue(new_pot_value);
    WsInputFacade::instance()->getFlywheelSpeedCompensationEnableSwitchState(new_switch_state);
    
    WS_LOG_WARNING("Got Here, npv = %f, nss = %d", new_pot_value, new_switch_state);
    
    if ((true == new_switch_state) &&
        ((fabs(a_currentPotValue - new_pot_value)) >= 0.03f))
    {
        //
        // Calculate the percentage change in RPM that the user is requesting.
        //  The pot value goes from 0.0V to 3.3V.  We scale that down to +/-
        //  20%, while accounting for the DC bias of (Vmax - Vmin)/2 == 1.65V.
        //
        // So, we calculate a number here that can be used to directly scale
        //  the default preset values stored in the config facade.
        //
        percent_change += (new_pot_value - 1.65) * 0.303f * .4f;
        
        //
        // Send the new factor to the subsystem.  This will cause the subsystem
        //  to re-read the preset config, and apply this scaling factor.
        //
        ap_subsystem->acceptNewCompensationFactor(percent_change);
        WS_LOG_WARNING("Got Here, pc = %f", percent_change);
        
        // Note the current pot value
        a_currentPotValue = new_pot_value;
    }
    else if ((false == new_switch_state) &&
             (true == a_currentSwitchState))
    {
        //
        // Send the "don't compensate" factor to the subsystem.
        //  This will cause the subsystem to re-read the preset config,
        //  and apply this scaling factor.  Since we are scaling by 1.0, this
        //  basically means the subsystem will return to the default configuration.
        //
        ap_subsystem->acceptNewCompensationFactor(percent_change);
        WS_LOG_WARNING("Got Here, pc = %f", percent_change);
        
        // Make sure future enable events result in an immediate %-change.
        a_currentPotValue = 0.0f;
    }
    else // disabled to disabled
    {
        //Don't do anything.
    }
    
    // Update the newest data in the class
    a_currentSwitchState = new_switch_state;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

