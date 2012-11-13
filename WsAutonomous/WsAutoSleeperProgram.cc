#include "WsAutoSleeperProgram.hh"
#include "WsLogger.hh"
#include "WsAutonomousProgram.hh"

WsAutoSleeperProgram::WsAutoSleeperProgram(void)
    : WsAutonomousProgram("WsAutoSleeperProgram")
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_TRACE("End");
}

WsAutoSleeperProgram::~WsAutoSleeperProgram(void)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_TRACE("End");
}

void WsAutoSleeperProgram::init(void)
{
    WsAutonomousProgram::init();
    //Do not need to init this program
    WS_LOG_ERROR("SLEEPER!");
}

bool
WsAutoSleeperProgram::run(void)
{
    WS_LOG_TRACE("Begin");
    
    // This is the sleeper program. We don't do anything.
    
    WS_LOG_TRACE("End");
    return true;
}

void
WsAutoSleeperProgram::clear(void)
{
    //There is nothing to clear in this program
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

