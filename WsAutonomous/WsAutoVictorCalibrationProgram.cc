#include "WsAutoVictorCalibrationProgram.hh"
#include "WsOutputFacade.hh"
#include "WsLogger.hh"
#include "WsInputFacade.hh"

#include "Timer.h"

#define WS_CALIBRATION_SETUP_TIMESTAMP         3.0
#define WS_CALIBRATION_HOLD_FWD_TIMESTAMP      WS_CALIBRATION_SETUP_TIMESTAMP + 2.0
#define WS_CALIBRATION_WAIT_CENTER1_TIMESTAMP  WS_CALIBRATION_HOLD_FWD_TIMESTAMP + 1.0
#define WS_CALIBRATION_HOLD_REV_TIMESTAMP      WS_CALIBRATION_WAIT_CENTER1_TIMESTAMP + 2.0
#define WS_CALIBRATION_WAIT_CENTER2_TIMESTAMP  WS_CALIBRATION_HOLD_REV_TIMESTAMP + 1.0
#define WS_CALIBRATION_COMPLETED_TIMESTAMP     WS_CALIBRATION_WAIT_CENTER2_TIMESTAMP

WsAutoVictorCalibrationProgram::WsAutoVictorCalibrationProgram(void)
    : WsAutonomousProgram("WsAutoVictorCalibrationProgram"),
      ap_stateTimer(NULL)
{
    WS_LOG_TRACE("Begin");
    
    a_programName = "WsAutoVictorCalibrationProgram";
    
    ap_stateTimer = new Timer();
    ap_stateTimer->Stop();
    ap_stateTimer->Reset();
    
    WS_LOG_TRACE("End");
}

WsAutoVictorCalibrationProgram::~WsAutoVictorCalibrationProgram(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_stateTimer;
    ap_stateTimer = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsAutoVictorCalibrationProgram::init(void)
{
    WS_LOG_TRACE("Begin");
    
    float stop_motors = 0.0;
    WsOutputFacade::instance()->setRightDriveMotorSpeed(stop_motors);
    WsOutputFacade::instance()->setLeftDriveMotorSpeed(stop_motors);
    
    ap_stateTimer->Start();
    
    WS_LOG_TRACE("End");
}

bool
WsAutoVictorCalibrationProgram::run(void)
{
    WS_LOG_TRACE("Begin");
    
    double elapsed_time = ap_stateTimer->Get();
    float motor_speed = 0.0;
    
    if (elapsed_time < WS_CALIBRATION_SETUP_TIMESTAMP)
    {
        // We are in the "setup" state, set output value to 0.0
        WS_LOG_NOTICE("Setup");
        motor_speed = 0.0;
    }
    else if (elapsed_time < WS_CALIBRATION_HOLD_FWD_TIMESTAMP)
    {
        // We are in the "full-forward" state, set output to 1.0 and hold
        WS_LOG_NOTICE("Full-Forward Calibration");
        motor_speed = 1.0;
    }
    else if (elapsed_time < WS_CALIBRATION_WAIT_CENTER1_TIMESTAMP)
    {
        // Back to center...
        WS_LOG_NOTICE("Stabilizing...");
        motor_speed = 0.0;
    }
    else if (elapsed_time < WS_CALIBRATION_HOLD_REV_TIMESTAMP)
    {
        // We are in the "full-reverse" state, set output to -1.0 and hold
        WS_LOG_NOTICE("Full-Reverse Calibration");
        motor_speed = -1.0;
    }
    else if (elapsed_time < WS_CALIBRATION_WAIT_CENTER2_TIMESTAMP)
    {
        // Back to center...
        WS_LOG_NOTICE("Stabilizing...");
        motor_speed = 0.0;
    }
    else
    {
        // Test is over
        motor_speed = 0.0;
    }
    
    // Update the PWM controls
    WsOutputFacade::instance()->setRightDriveMotorSpeed(motor_speed);
    WsOutputFacade::instance()->setLeftDriveMotorSpeed(motor_speed);
    WS_LOG_TRACE("End");
    
    return true;
}

void
WsAutoVictorCalibrationProgram::clear(void)
{
    ap_stateTimer->Stop();
    ap_stateTimer->Reset();
}

bool
WsAutoVictorCalibrationProgram::isDone(void)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_TRACE("End");
    return (ap_stateTimer->Get() >= WS_CALIBRATION_COMPLETED_TIMESTAMP);
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

