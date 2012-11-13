#include "WsTurretObservers.hh"
#include "WsInputFacade.hh"
#include "WsConfigFacade.hh"
#include "WsTurret.hh"
#include "WsLogger.hh"

#include "Timer.h"
#include "DriverStation.h"

#define PRESET_DEBOUNCE_LOWER_LIMIT 0.1f
#define PRESET_DEBOUNCE_UPPER_LIMIT 1.0f

//-----------------------------------------------------------------------------
// WsTurretCcwLimitSwitchStateObserver
//-----------------------------------------------------------------------------
WsTurretCcwLimitSwitchStateObserver::WsTurretCcwLimitSwitchStateObserver(WsTurret* p_subsystem)
    : WsObserver("WsTurretCcwLimitSwitchStateObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getTurretCcwLimitSwitchState(a_currentState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewCcwLimitSwitchState(a_currentState);
}

WsTurretCcwLimitSwitchStateObserver::~WsTurretCcwLimitSwitchStateObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getTurretCcwLimitSwitchState(new_btn_state);
    p_subject->detach(this);
}

void
WsTurretCcwLimitSwitchStateObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getTurretCcwLimitSwitchState(new_btn_state);
    WS_LOG_DEBUG("a_currentState = %d, new_btn_state = %d",
                 a_currentState,
                 new_btn_state);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewCcwLimitSwitchState(new_btn_state);
    
    // Note the new value
    a_currentState = new_btn_state;
}


//-----------------------------------------------------------------------------
// WsTurretCwLimitSwitchStateObserver
//-----------------------------------------------------------------------------
WsTurretCwLimitSwitchStateObserver::WsTurretCwLimitSwitchStateObserver(WsTurret* p_subsystem)
    : WsObserver("WsTurretCwLimitSwitchStateObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getTurretCwLimitSwitchState(a_currentState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewCwLimitSwitchState(a_currentState);
}

WsTurretCwLimitSwitchStateObserver::~WsTurretCwLimitSwitchStateObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getTurretCwLimitSwitchState(new_btn_state);
    p_subject->detach(this);
}

void
WsTurretCwLimitSwitchStateObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getTurretCwLimitSwitchState(new_btn_state);
    WS_LOG_DEBUG("a_currentState = %d, new_btn_state = %d",
                 a_currentState,
                 new_btn_state);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewCwLimitSwitchState(new_btn_state);
    
    // Note the new value
    a_currentState = new_btn_state;
}


//-----------------------------------------------------------------------------
// WsTurretHeadingAxisValueObserver
//-----------------------------------------------------------------------------
WsTurretHeadingAxisValueObserver::WsTurretHeadingAxisValueObserver(WsTurret* p_subsystem)
    : WsObserver("WsTurretHeadingAxisValueObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getRequestedTurretDirection(a_currentValue);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewHeadingAxisValue(a_currentValue);
}

WsTurretHeadingAxisValueObserver::~WsTurretHeadingAxisValueObserver(void)
{
    float new_value = 0.0;
    WsSubject* p_subject =
        WsInputFacade::instance()->getRequestedTurretDirection(new_value);
    p_subject->detach(this);
}

void
WsTurretHeadingAxisValueObserver::update(void)
{
    float new_value = 0.0;
    WsInputFacade::instance()->getRequestedTurretDirection(new_value);
    WS_LOG_DEBUG("a_currentValue = %.05f, new_value = %.05f",
                 a_currentValue,
                 new_value);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewHeadingAxisValue(new_value);
    
    // Note the new value
    a_currentValue = new_value;
}


//-----------------------------------------------------------------------------
// WsTurretHeadingPotValueObserver
//-----------------------------------------------------------------------------
WsTurretHeadingPotValueObserver::WsTurretHeadingPotValueObserver(WsTurret* p_subsystem)
    : WsObserver("WsTurretHeadingPotValueObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getTurretHeadingPotValue(a_currentValue);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewHeadingPotValue(a_currentValue);
}

WsTurretHeadingPotValueObserver::~WsTurretHeadingPotValueObserver(void)
{
    float new_value = 0.0;
    WsSubject* p_subject =
        WsInputFacade::instance()->getTurretHeadingPotValue(new_value);
    p_subject->detach(this);
}

void
WsTurretHeadingPotValueObserver::update(void)
{
    float new_value = 0.0;
    WsInputFacade::instance()->getTurretHeadingPotValue(new_value);
    WS_LOG_DEBUG("a_currentValue = %.05f, new_value = %.05f",
                 a_currentValue,
                 new_value);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewHeadingPotValue(new_value);
    
    // Note the new value
    a_currentValue = new_value;
}


//-----------------------------------------------------------------------------
// WsTurretTurboButtonStateObserver
//-----------------------------------------------------------------------------
WsTurretTurboButtonStateObserver::WsTurretTurboButtonStateObserver(WsTurret* p_subsystem)
    : WsObserver("WsTurretTurboButtonStateObserver")
    , ap_subsystem(p_subsystem)
    , a_currentButtonState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getTurretTurboButtonState(a_currentButtonState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewTurboButtonState(a_currentButtonState);
}

WsTurretTurboButtonStateObserver::~WsTurretTurboButtonStateObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getTurretTurboButtonState(new_btn_state);
    p_subject->detach(this);
}

void
WsTurretTurboButtonStateObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getTurretTurboButtonState(new_btn_state);
    WS_LOG_DEBUG("a_currentButtonState = %d, new_btn_state = %d",
                 a_currentButtonState,
                 new_btn_state);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewTurboButtonState(new_btn_state);
    
    // Note the new value
    a_currentButtonState = new_btn_state;
}


//-----------------------------------------------------------------------------
// WsTurretPidConfigObserver
//-----------------------------------------------------------------------------
WsTurretPidConfigObserver::WsTurretPidConfigObserver(WsTurret* p_subsystem)
    : WsObserver("WsTurretPidConfigObserver")
    , ap_subsystem(p_subsystem)
    , a_currentConfig()
{
    // Zero-out the current config
    ::memset(&a_currentConfig, 0x00, sizeof(a_currentConfig));
    
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsConfigFacade::instance()->getTurretPidConfig(a_currentConfig);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewPidConfig(a_currentConfig);
}

WsTurretPidConfigObserver::~WsTurretPidConfigObserver(void)
{
    WsTypes::WsPidConfigurationDataT new_config;
    ::memset(&new_config, 0x00, sizeof(new_config));
    
    WsSubject* p_subject =
        WsConfigFacade::instance()->getTurretPidConfig(new_config);
    p_subject->detach(this);
}

void
WsTurretPidConfigObserver::update(void)
{
    WsTypes::WsPidConfigurationDataT new_config;
    ::memset(&new_config, 0x00, sizeof(new_config));
    
    WsConfigFacade::instance()->getTurretPidConfig(new_config);
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
// WsTurretRequestedPresetObserver
//-----------------------------------------------------------------------------
WsTurretRequestedPresetObserver::WsTurretRequestedPresetObserver(WsTurret* p_subsystem)
    : WsObserver("WsTurretRequestedPresetObserver")
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

WsTurretRequestedPresetObserver::~WsTurretRequestedPresetObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getShooterPreset(a_currentValue);
    p_subject->detach(this);
    
    delete ap_debounceTimer;
    ap_debounceTimer = NULL;
}

void
WsTurretRequestedPresetObserver::update(void)
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
// WsTurretPresetConfigObserver
//-----------------------------------------------------------------------------
WsTurretPresetConfigObserver::WsTurretPresetConfigObserver(WsTurret* p_subsystem)
    : WsObserver("WsTurretPresetConfigObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue()
{
    ::memset(&a_currentValue, 0x00, sizeof(a_currentValue));
    
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsConfigFacade::instance()->getShooterPresetConfig(a_currentValue);
    p_subject->attach(this);
    
    // Push the initial config into the subsystem
    ap_subsystem->acceptNewPresetConfig(a_currentValue);
}

WsTurretPresetConfigObserver::~WsTurretPresetConfigObserver(void)
{
    WsSubject* p_subject =
        WsConfigFacade::instance()->getShooterPresetConfig(a_currentValue);
    p_subject->detach(this);
}

void
WsTurretPresetConfigObserver::update(void)
{
    WsTypes::WsShooterPresetConfigT new_value;
    ::memset(&new_value, 0x00, sizeof(new_value));
    
    WsConfigFacade::instance()->getShooterPresetConfig(new_value);
    
    WS_LOG_DEBUG("current turret_key_forward_voltage = %f, new turret_key_forward_voltage = %f",
                 a_currentValue.turret_key_forward_voltage,
                 new_value.turret_key_forward_voltage);
    WS_LOG_DEBUG("current flywheel_key_forward_speed = %f, new flywheel_key_forward_speed = %f",
                 a_currentValue.flywheel_key_forward_speed,
                 new_value.flywheel_key_forward_speed);
    WS_LOG_DEBUG("current turret_key_reverse_voltage = %f, new turret_key_reverse_voltage = %f",
                 a_currentValue.turret_key_reverse_voltage,
                 new_value.turret_key_reverse_voltage);
    WS_LOG_DEBUG("current flywheel_key_reverse_speed = %f, new flywheel_key_reverse_speed = %f",
                 a_currentValue.flywheel_key_reverse_speed,
                 new_value.flywheel_key_reverse_speed);
    WS_LOG_DEBUG("current turret_fender_left_voltage = %f, new turret_fender_left_voltage = %f",
                 a_currentValue.turret_fender_left_voltage,
                 new_value.turret_fender_left_voltage);
    WS_LOG_DEBUG("current flywheel_fender_left_speed = %f, new flywheel_fender_left_speed = %f",
                 a_currentValue.flywheel_fender_left_speed,
                 new_value.flywheel_fender_left_speed);
    WS_LOG_DEBUG("current turret_fender_right_voltage = %f, new turret_fender_right_voltage = %f",
                 a_currentValue.turret_fender_right_voltage,
                 new_value.turret_fender_right_voltage);
    WS_LOG_DEBUG("current flywheel_fender_right_speed = %f, new flywheel_fender_right_speed = %f",
                 a_currentValue.flywheel_fender_right_speed,
                 new_value.flywheel_fender_right_speed);
    WS_LOG_DEBUG("current turret_home_voltage = %f, new turret_auton_voltage = %f",
                 a_currentValue.turret_auton_voltage,
                 new_value.turret_auton_voltage);
    WS_LOG_DEBUG("current flywheel_home_speed = %f, new flywheel_auton_speed = %f",
                 a_currentValue.flywheel_auton_speed,
                 new_value.flywheel_auton_speed);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewPresetConfig(new_value);
    
    // Note the new value
    a_currentValue = new_value;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

