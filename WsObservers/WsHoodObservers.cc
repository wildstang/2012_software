#include "WsHoodObservers.hh"
#include "WsInputFacade.hh"
#include "WsHood.hh"
#include "WsLogger.hh"

//-----------------------------------------------------------------------------
//class WsHoodButtonStateObserver : public WsObserver
//-----------------------------------------------------------------------------

WsHoodButtonStateObserver::WsHoodButtonStateObserver(WsHood* p_subsystem)
    : WsObserver("WsHoodButtonStateObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getHoodButtonState(a_currentState);
    p_subject->attach(this);
}

WsHoodButtonStateObserver::~WsHoodButtonStateObserver(void)
{
    bool value = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getHoodButtonState(value);
    p_subject->detach(this);
}

void
WsHoodButtonStateObserver::update(void)
{
    bool new_value = false;
    WsInputFacade::instance()->getHoodButtonState(new_value);
    
    //
    // Log this event as an error because we are seeing flakiness with the hood.
    //  We should never see these state transitions if the user is not actively
    //  pressing the buttons.
    //
    WS_LOG_DEBUG("a_currentState = %d, new_value = %d",
                 a_currentState,
                 new_value);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewHoodButtonState(a_currentState, new_value);
    
    // Note the new value
    a_currentState = new_value;
}

//-----------------------------------------------------------------------------
//class WsHoodShooterPresetObserver : public WsObserver
//-----------------------------------------------------------------------------

WsHoodShooterPresetObserver::WsHoodShooterPresetObserver(WsHood* p_subsystem)
    : WsObserver("WsHoodShooterPresetObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(WsTypes::WS_SHOOTER_PRESET_NONE)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getShooterPreset(a_currentState);
    p_subject->attach(this);
}

WsHoodShooterPresetObserver::~WsHoodShooterPresetObserver(void)
{
    WsTypes::WsShooterPresetType new_value = WsTypes::WS_SHOOTER_PRESET_NONE;
    WsSubject* p_subject =
        WsInputFacade::instance()->getShooterPreset(new_value);
    p_subject->detach(this);
}

void
WsHoodShooterPresetObserver::update(void)
{
    WsTypes::WsShooterPresetType new_value = WsTypes::WS_SHOOTER_PRESET_NONE;
    WsInputFacade::instance()->getShooterPreset(new_value);
    
    bool shooter_button_state = false;
    WsInputFacade::instance()->getBallShootButtonState(shooter_button_state);
    
    bool shooter_override_button_state = false;
    WsInputFacade::instance()->getBallShootOverrideButtonState(shooter_override_button_state);
    
    //
    // Log this event as an error because we are seeing flakiness with the hood.
    //  We should never see these state transitions if the user is not actively
    //  pressing the buttons.
    //
    WS_LOG_DEBUG("a_currentState = %d, new_value = %d",
                 a_currentState,
                 new_value);
                 
    // Push the current value into the subsystem, only if we aren't shooting.
    if ((false == shooter_override_button_state) &&
        (false == shooter_button_state))
    {
        ap_subsystem->acceptNewRequestedPreset(new_value);
    }
    
    // Note the new value
    a_currentState = new_value;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

