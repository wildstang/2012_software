#include "WsCompressor.hh"
#include "WsLogger.hh"
#include "WsOutputFacade.hh"
#include "WsCompressorObservers.hh"

WsCompressor::WsCompressor(void)
    : WsSubsystem("WsCompressor")
    , a_compressorState(false)
    , ap_pressureSwitchObserver(NULL)
{
    WS_LOG_TRACE("Begin");
    ap_pressureSwitchObserver = new WsPressureSwitchStateObserver(this);
    WS_LOG_TRACE("End");
}

WsCompressor::~WsCompressor(void)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_TRACE("End");
}

void
WsCompressor::update(void)
{
    WS_LOG_TRACE("Begin");
    
    WsTypes::WsCompressorStateType compressor =
        (true == a_compressorState)
        ? WsTypes::WS_COMPRESSOR_ON
        : WsTypes::WS_COMPRESSOR_OFF;
    WsOutputFacade::instance()->setCompressorState(compressor);
    
    WS_LOG_TRACE("End");
}

void
WsCompressor::setCompressorState(const bool& compressor_state)
{
    WS_LOG_TRACE("Begin");
    
    a_compressorState = compressor_state;
    
    WS_LOG_TRACE("End");
}

void
WsCompressor::getCompressorState(bool& compressor_state)
{
    WS_LOG_TRACE("Begin");
    compressor_state = a_compressorState;
    WS_LOG_TRACE("End");
}

void
WsCompressor::acceptNewPressureSwitchState(bool sensorState)
{
    a_compressorState = sensorState;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

