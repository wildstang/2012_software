#include "WsFlywheelSpeedPidOutput.hh"
#include "WsOutputFacade.hh"
#include "WsDataIndicationFacade.hh"
#include "WsLogger.hh"
#include <math.h>

WsFlywheelSpeedPidOutput::WsFlywheelSpeedPidOutput()
    : WsPidOutput()
    , a_cumulativeOutput(0.0f)
{
}

WsFlywheelSpeedPidOutput::~WsFlywheelSpeedPidOutput()
{
}

// Writes data from the PID class to the output facade
void
WsFlywheelSpeedPidOutput::pidWrite(float output)
{
    //
    // Since we are controlling a speed, we can't just set the current output
    //  to the motors - the motor will never reach the set point because the
    //  error term will decrease as we approach our setpoint, and that term
    //  would be fed to the motors.  To overcome this, we add the PID output
    //  to a cumulative term.  When the PID output is zero, we add 0 to the
    //  output and the motor value remains as it is.
    //
    a_cumulativeOutput += output;
    a_cumulativeOutput = (a_cumulativeOutput >= 1.0) ? 1.0 : a_cumulativeOutput;
    a_cumulativeOutput = (a_cumulativeOutput <= 0.0) ? 0.0 : a_cumulativeOutput;
    
    // Flip the flywheel output to match electrical wiring
    float real_output = a_cumulativeOutput * -1.0f;
    WsOutputFacade::instance()->setFlywheelMotorSpeed(real_output);
    
    // Indicate the current PID output
    WsDataIndicationFacade::instance()->setFlywheelPidControllerOutput(output);
    
//    WS_LOG_NOTICE("a_cumulativeOutput = %.05f, output = %.05f", a_cumulativeOutput, output);
}

void
WsFlywheelSpeedPidOutput::overwriteCumulativeOutput(const float& rc_new_val)
{
    a_cumulativeOutput = rc_new_val;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

