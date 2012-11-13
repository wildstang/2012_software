#include "WsTurretPidOutput.hh"
#include "WsOutputFacade.hh"
#include "WsLogger.hh"
#include <math.h>

WsTurretPidOutput::WsTurretPidOutput()
    : WsPidOutput()
{
}

WsTurretPidOutput::~WsTurretPidOutput()
{
}

void
WsTurretPidOutput::pidWrite(float output)
{
    // @TODO: This needs to check the limit switches to make sure
    //that we do not overdrive the turret.
    WsOutputFacade::instance()->setTurretHeadingSpeed(output);
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

