#include "WsBridgeBrakeObservers.hh"
#include "WsInputFacade.hh"
#include "WsBridgeBrake.hh"
#include "WsLogger.hh"

//-----------------------------------------------------------------------------
// WsBridgeBrakeButtonStateObserver
//-----------------------------------------------------------------------------

WsBridgeBrakeButtonStateObserver::WsBridgeBrakeButtonStateObserver(WsBridgeBrake* p_bridgeBrakeSubsystem)
    : WsObserver("WsBridgeBrakeButtonStateObserver")
    , ap_bridgeBrakeSubsystem(p_bridgeBrakeSubsystem)
    , a_currentBridgeBrakeButtonState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getBridgeBrakeActivationButtonState(a_currentBridgeBrakeButtonState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_bridgeBrakeSubsystem->acceptNewBridgeBrakeButtonState(true, a_currentBridgeBrakeButtonState);
}

WsBridgeBrakeButtonStateObserver::~WsBridgeBrakeButtonStateObserver(void)
{
    bool value = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getBridgeBrakeActivationButtonState(value);
    p_subject->detach(this);
}

void
WsBridgeBrakeButtonStateObserver::update(void)
{
    bool new_value = false;
    WsInputFacade::instance()->getBridgeBrakeActivationButtonState(new_value);
    
    WS_LOG_ERROR("a_currentBridgeBrakeButtonState = %d, new_value = %d",
                 a_currentBridgeBrakeButtonState,
                 new_value);
                 
    // Push the current value into the subsystem
    ap_bridgeBrakeSubsystem->acceptNewBridgeBrakeButtonState(a_currentBridgeBrakeButtonState, new_value);
    
    // Note the new value
    a_currentBridgeBrakeButtonState = new_value;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

