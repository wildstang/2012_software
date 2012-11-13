#include "WsObserver.hh"
#include "WsLogger.hh"

WsObserver::WsObserver(std::string name)
    : a_observerName(name)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_DEBUG("WsObserver '%s' created", a_observerName.c_str());
    
    WS_LOG_TRACE("End");
}

WsObserver::~WsObserver(void)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_DEBUG("WsObserver '%s' destroyed", a_observerName.c_str());
    a_observerName.clear();
    
    WS_LOG_TRACE("End");
}

std::string
WsObserver::getName(void)
{
    return a_observerName;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

