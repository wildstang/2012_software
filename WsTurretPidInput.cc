#include "WsTurretPidInput.hh"
#include "WsInputFacade.hh"
#include "WsLogger.hh"

#include <string.h>

static const int IMAGE_WIDTH         = 320;
static const int IMAGE_HEIGHT        = 240;
static const int TURRET_IMAGE_CENTER = IMAGE_WIDTH / 2;

WsTurretPidInput::WsTurretPidInput(void)
    : WsPidInput()
{
    //Nothing happens
}

WsTurretPidInput::~WsTurretPidInput(void)
{
    //Nothing Happens
}

float
WsTurretPidInput::pidRead(void)
{
    float pot_val = 0.0f;
    WsInputFacade::instance()->getTurretHeadingPotValue(pot_val);
    return pot_val;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

