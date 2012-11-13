#include "WsSubsystem.hh"
#include "WsLogger.hh"

WsSubsystem::WsSubsystem(const char* pc_name)
    : a_subsystemName(pc_name)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_TRACE("End");
}

WsSubsystem::~WsSubsystem(void)
{
    WS_LOG_TRACE("Begin");
    
    a_subsystemName.clear();
    
    WS_LOG_TRACE("End");
}

std::string
WsSubsystem::GetName(void)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_TRACE("End");
    return a_subsystemName;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

