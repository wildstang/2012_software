#include "WsFlywheelLinearizerProgram.hh"
#include "WsDriveBase.hh"
#include "WsInputFacade.hh"
#include "WsLogger.hh"
#include "WsOutputFacade.hh"
#include "Timer.h"
#include "math.h"

#define VICTOR_TEST_TIME_STABILIZE 0.50f
#define VICTOR_TEST_TIME_TEST      1.00f

#define TEST_RESOLUTION  100.0f
#define VICTOR_TEST_STEP -1

WsFlywheelLinearizerProgram::WsFlywheelLinearizerProgram(void)
    : WsAutonomousProgram("WsFlywheelLinearizerProgram"),
      a_testIteration(0),
      a_testingState(VICTOR_STATE_SETUP),
      ap_victorTimer(NULL),
      a_output(0.0),
      a_finished(false)
{
    WS_LOG_TRACE("Begin");
    
    a_programName = "WsFlywheelLinearizerProgram";
    
    ap_victorTimer = new Timer();
    ap_victorTimer->Stop();
    ap_victorTimer->Reset();
    
    WS_LOG_TRACE("End");
}

WsFlywheelLinearizerProgram::~WsFlywheelLinearizerProgram(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_victorTimer; ap_victorTimer = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsFlywheelLinearizerProgram::init(void)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_ALWAYS("MotorOutput, Flywheel Speed");
    
    WS_LOG_TRACE("End");
}

bool
WsFlywheelLinearizerProgram::run(void)
{
    WS_LOG_TRACE("Begin");
    
    //Victor Linearizer: Runs every loop if enabled
    
    VictorLinearizerStateT newState = a_testingState;
    
    switch (a_testingState)
    {
        case VICTOR_STATE_SETUP:
        {
            a_output = (float)(a_testIteration / TEST_RESOLUTION);
            
            ap_victorTimer->Reset();
            ap_victorTimer->Start();
            
            // TRANSITION
            
            newState = VICTOR_STATE_STABILIZE;
            break;
        }
        
        case VICTOR_STATE_STABILIZE:
        {
            // This state is waiting for the motor to reach the
            // Desired speed before beginning to measure
            // In other words, nothing changes
            
            // TRANSITION
            
            if (ap_victorTimer->Get() >= VICTOR_TEST_TIME_STABILIZE)
            {
                ap_victorTimer->Stop();
                ap_victorTimer->Reset();
                ap_victorTimer->Start();
                newState = VICTOR_STATE_TEST;
            }
            break;
        }
        
        case VICTOR_STATE_TEST:
        {
            // This state is waiting for the encoder to count
            // Valid test data
            // Again, this is a wait state. Nothing happens.
            
            // TRANSITION
            
            if (ap_victorTimer->Get() >= VICTOR_TEST_TIME_TEST)
            {
                //This is sort of a mini-state
                //Read the encoders
                float flywheel_speed = 0.0;
                WsInputFacade::instance()->getFlywheelRotationalSpeed(flywheel_speed);
                WS_LOG_ALWAYS("%.5f, %0.5f", a_output, flywheel_speed);
                
                ap_victorTimer->Stop();
                ap_victorTimer->Reset();
                
                a_testIteration += VICTOR_TEST_STEP;
                newState = VICTOR_STATE_SETUP;
                
                if (TEST_RESOLUTION < abs(a_testIteration))
                {
                    a_finished = true;
                }
            }
            break;
        }
        
        case VICTOR_STATE_NUM:
        default:
        {
            // Invalid state... disable the output for safety reasons
            WS_LOG_ERROR("Invalid State");
            a_output = 0.0;
            break;
        }
    } // end switch
    
    // Set Outputs
    WsOutputFacade::instance()->setFlywheelMotorSpeed(a_output);
    
    // Update to the new state
    a_testingState = newState;
    
    WS_LOG_TRACE("End");
    return true;
}

void WsFlywheelLinearizerProgram::clear(void)
{
    a_testIteration = 0;
    a_testingState = VICTOR_STATE_SETUP;
    a_output = 0.0;
    a_finished = false;
    
    ap_victorTimer->Stop();
    ap_victorTimer->Reset();
}

bool
WsFlywheelLinearizerProgram::isDone(void)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_TRACE("End");
    return a_finished;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

