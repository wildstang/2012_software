#include "WsFlywheelSpeedPidInput.hh"
#include "WsInputFacade.hh"
#include "WsLogger.hh"

#include <string.h>

WsFlywheelSpeedPidInput::WsFlywheelSpeedPidInput(void)
{
    // Nothing happens
}

WsFlywheelSpeedPidInput::~WsFlywheelSpeedPidInput(void)
{
    // Nothing Happens
}

// Gets the flywheel velocity from the Input facade
float
WsFlywheelSpeedPidInput::pidRead(void)
{
    float speed = 0.0f;
    WsInputFacade::instance()->getFlywheelRotationalSpeed(speed);
    WS_LOG_DEBUG("Flywheel Speed = %.05f", speed);
    return speed;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

