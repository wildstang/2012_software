#include "WsPogoStickObservers.hh"
#include "WsInputFacade.hh"
#include "WsPogoStick.hh"
#include "WsLogger.hh"

//-----------------------------------------------------------------------------
//class WsPogoStickButtonStateObserver : public WsObserver
//-----------------------------------------------------------------------------

WsPogoStickButtonStateObserver::WsPogoStickButtonStateObserver(WsPogoStick* p_pogoStickSubsystem)
    : WsObserver("WsPogoStickButtonStateObserver")
    , ap_pogoStickSubsystem(p_pogoStickSubsystem)
    , a_currentPogoStickButtonState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getPogoStickDeploymentButtonState(a_currentPogoStickButtonState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_pogoStickSubsystem->acceptNewPogoStickButtonState(true, a_currentPogoStickButtonState);
}

WsPogoStickButtonStateObserver::~WsPogoStickButtonStateObserver(void)
{
    bool value = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getPogoStickDeploymentButtonState(value);
    p_subject->detach(this);
}

void
WsPogoStickButtonStateObserver::update(void)
{
    bool new_value = false;
    WsInputFacade::instance()->getPogoStickDeploymentButtonState(new_value);
    
    WS_LOG_ERROR("a_currentPogoStickButtonState = %d, new_value = %d",
                 a_currentPogoStickButtonState,
                 new_value);
                 
    // Push the current value into the subsystem
    ap_pogoStickSubsystem->acceptNewPogoStickButtonState(a_currentPogoStickButtonState, new_value);
    
    // Note the new value
    a_currentPogoStickButtonState = new_value;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

