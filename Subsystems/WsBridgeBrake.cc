#include "WsBridgeBrake.hh"
#include "WsOutputFacade.hh"
#include "WsBridgeBrakeObservers.hh"
#include "WsLogger.hh"

WsBridgeBrake::WsBridgeBrake(void)
    : WsSubsystem("WsBridgeBrake")
    , a_bridgeBrakeState(WsTypes::WS_BRIDGE_BRAKE_CYLINDER_DISENGAGED)
    , ap_bridgeBrakeValueObserver(NULL)
{
    WS_LOG_TRACE("Begin");
    
    ap_bridgeBrakeValueObserver = new WsBridgeBrakeButtonStateObserver(this);
    WsOutputFacade::instance()->setBridgeBrakeCylinderState(a_bridgeBrakeState);
    
    WS_LOG_TRACE("End");
}

WsBridgeBrake::~WsBridgeBrake(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_bridgeBrakeValueObserver;    ap_bridgeBrakeValueObserver = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsBridgeBrake::update(void)
{
    WS_LOG_TRACE("Begin");
    
    WsOutputFacade::instance()->setBridgeBrakeCylinderState(a_bridgeBrakeState);
    WS_LOG_DEBUG("Brake State (ss): %d", a_bridgeBrakeState);
    
    WS_LOG_TRACE("End");
}

void
WsBridgeBrake::acceptNewBridgeBrakeButtonState(bool oldValue, bool newValue)
{
    if ((false == oldValue) &&
        (true == newValue))
    {
        WsTypes::WsBridgeBrakeCylinderStateType new_state =
            (WsTypes::WS_BRIDGE_BRAKE_CYLINDER_DISENGAGED == a_bridgeBrakeState)
            ? WsTypes::WS_BRIDGE_BRAKE_CYLINDER_ENGAGED
            : WsTypes::WS_BRIDGE_BRAKE_CYLINDER_DISENGAGED;
            
        WS_LOG_DEBUG("Bridge Brake toggled from %d to %d", a_bridgeBrakeState, new_state);
        a_bridgeBrakeState = new_state;
    }
    // else do nothing
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

