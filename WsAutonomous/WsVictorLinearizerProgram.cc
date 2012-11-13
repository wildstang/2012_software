#include "WsVictorLinearizerProgram.hh"
#include "WsDriveBase.hh"
#include "WsInputFacade.hh"
#include "WsLogger.hh"
#include "WsOutputFacade.hh"
#include "Timer.h"
#include "math.h"
#include "WsTypes.hh"

#define VICTOR_TEST_TIME_STABILIZE 0.50f
#define VICTOR_TEST_TIME_TEST      1.00f

#define TEST_RESOLUTION  100.0f
#define VICTOR_TEST_STEP -1

WsVictorLinearizerProgram::WsVictorLinearizerProgram(void)
    : WsAutonomousProgram("WsVictorLinearizerProgram"),
      a_testIteration(0),
      a_testingState(VICTOR_STATE_SETUP),
      ap_victorTimer(NULL),
      a_output(0.0),
      a_finished(false)
{
    WS_LOG_TRACE("Begin");
    
    a_programName = "WsVictorLinearizerProgram";
    
    ap_victorTimer = new Timer();
    ap_victorTimer->Stop();
    ap_victorTimer->Reset();
    
    WS_LOG_TRACE("End");
}

WsVictorLinearizerProgram::~WsVictorLinearizerProgram(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_victorTimer;
    ap_victorTimer = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsVictorLinearizerProgram::init(void)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_ALWAYS("MotorOutput, Left Speed, Right Speed");
    WsInputFacade::instance()->resetDriveEncoderCounts();
    
    float motors_off = 0.0f;
    WsOutputFacade::instance()->setLeftDriveMotorSpeed(motors_off);
    WsOutputFacade::instance()->setRightDriveMotorSpeed(motors_off);
    
    WsTypes::WsShifterStateType low_gear = WsTypes::WS_S_LOWGEAR;
    WsOutputFacade::instance()->setShifterState(low_gear);
    
    WS_LOG_TRACE("End");
}

bool
WsVictorLinearizerProgram::run(void)
{
    WS_LOG_TRACE("Begin");
    
    //Victor Linearizer: Runs every loop if enabled
    
    VictorLinearizerStateT newState = a_testingState;
    
    switch (a_testingState)
    {
        case VICTOR_STATE_SETUP:
            a_output = (float)(a_testIteration / TEST_RESOLUTION);
            
            ap_victorTimer->Reset();
            ap_victorTimer->Start();
            
            // TRANSITION
            
            newState = VICTOR_STATE_STABILIZE;
            break;
            
        case VICTOR_STATE_STABILIZE:
        
            // This state is waiting for the motor to reach the
            // Desired speed before beginning to measure
            // In other words, nothing changes
            
            // TRANSITION
            
            if (ap_victorTimer->Get() >= VICTOR_TEST_TIME_STABILIZE)
            {
                WsInputFacade::instance()->resetDriveEncoderCounts();
                
                ap_victorTimer->Stop();
                ap_victorTimer->Reset();
                ap_victorTimer->Start();
                
                newState = VICTOR_STATE_TEST;
            }
            break;
            
        case VICTOR_STATE_TEST:
        
            // This state is waiting for the encoder to count
            // Valid test data
            // Again, this is a wait state. Nothing happens.
            
            // TRANSITION
            
            if (ap_victorTimer->Get() >= VICTOR_TEST_TIME_TEST)
            {
                //This is sort of a mini-state
                //Read the encoders
                INT32 left_enc_count = 0;
                INT32 right_enc_count = 0;
                WsInputFacade::instance()->getLeftDriveEncoderCount(left_enc_count);
                WsInputFacade::instance()->getRightDriveEncoderCount(right_enc_count);
                WS_LOG_ALWAYS("%.5f, %d, %d", a_output, left_enc_count, right_enc_count);
                
                ap_victorTimer->Stop();
                ap_victorTimer->Reset();
                WsInputFacade::instance()->resetDriveEncoderCounts();
                
                a_testIteration += VICTOR_TEST_STEP;
                newState = VICTOR_STATE_SETUP;
                
                if (TEST_RESOLUTION < abs(a_testIteration))
                {
                    a_finished = true;
                }
            }
            break;
            
        case VICTOR_STATE_NUM:
        default:
            //If you are here you were a bad boy
            WS_LOG_ERROR("Invalid State");
            
            //Disable the output for safety reasons
            a_output = 0.0;
            
            break;
            
    }
    
    //Set Outputs
    WsOutputFacade::instance()->setRightDriveMotorSpeed(a_output);
    WsOutputFacade::instance()->setLeftDriveMotorSpeed(a_output);
    
    a_testingState = newState;
    
    WS_LOG_TRACE("End");
    return true;
}

void
WsVictorLinearizerProgram::clear(void)
{
    a_testIteration = 0;
    a_testingState = VICTOR_STATE_SETUP;
    a_output = 0.0;
    a_finished = false;
    
    ap_victorTimer->Stop();
    ap_victorTimer->Reset();
}

bool
WsVictorLinearizerProgram::isDone(void)
{
    WS_LOG_TRACE("Begin");
    
    WS_LOG_TRACE("End");
    return a_finished;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

