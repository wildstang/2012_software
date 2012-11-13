#include "WsPogoStick.hh"
#include "WsOutputFacade.hh"
#include "WsPogoStickObservers.hh"
#include "WsLogger.hh"

WsPogoStick::WsPogoStick(void)
    : WsSubsystem("WsPogoStick")
    , a_pogoStickState(WsTypes::WS_POGO_STICK_CYLINDER_UP)
    , ap_pogoStickValueObserver(NULL)
{
    WS_LOG_TRACE("Begin");
    
    ap_pogoStickValueObserver = new WsPogoStickButtonStateObserver(this);
    WsOutputFacade::instance()->setPogoStickCylinderState(a_pogoStickState);
    
    WS_LOG_TRACE("End");
}

WsPogoStick::~WsPogoStick(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_pogoStickValueObserver;    ap_pogoStickValueObserver = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsPogoStick::update(void)
{
    WS_LOG_TRACE("Begin");
    
    WsOutputFacade::instance()->setPogoStickCylinderState(a_pogoStickState);
    
    WS_LOG_TRACE("End");
}

void
WsPogoStick::acceptNewPogoStickButtonState(bool oldValue, bool newValue)
{
    if ((false == oldValue) &&
        (true == newValue))
    {
        WsTypes::WsPogoStickCylinderStateType new_state =
            (WsTypes::WS_POGO_STICK_CYLINDER_UP == a_pogoStickState)
            ? WsTypes::WS_POGO_STICK_CYLINDER_DOWN
            : WsTypes::WS_POGO_STICK_CYLINDER_UP;
            
        WS_LOG_ALWAYS("Pogo Stick toggled from %d to %d", a_pogoStickState, new_state);
        a_pogoStickState = new_state;
    }
    // else do nothing
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

