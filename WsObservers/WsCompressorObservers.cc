#include "WsCompressorObservers.hh"
#include "WsInputFacade.hh"
#include "WsCompressor.hh"
#include "WsLogger.hh"

//-----------------------------------------------------------------------------
//class WsPressureSwitchStateObserver : public WsObserver
//-----------------------------------------------------------------------------

WsPressureSwitchStateObserver::WsPressureSwitchStateObserver(WsCompressor* p_compressorSubsystem)
    : WsObserver("WsPressureSwitchStateObserver")
    , ap_compressorSubsystem(p_compressorSubsystem)
    , a_currentPressureSwitchState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getPressureSwitchState(a_currentPressureSwitchState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_compressorSubsystem->acceptNewPressureSwitchState(a_currentPressureSwitchState);
}

WsPressureSwitchStateObserver::~WsPressureSwitchStateObserver(void)
{
    bool value = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getPressureSwitchState(value);
    p_subject->detach(this);
}

void
WsPressureSwitchStateObserver::update(void)
{
    bool new_value = false;
    WsInputFacade::instance()->getPressureSwitchState(new_value);
    
    WS_LOG_DEBUG("a_currentPressureSwitchState = %d, new_value = %d",
                 a_currentPressureSwitchState,
                 new_value);
                 
    // Push the current value into the subsystem
    ap_compressorSubsystem->acceptNewPressureSwitchState(new_value);
    
    // Note the new value
    a_currentPressureSwitchState = new_value;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

