#include "WsAppendageHandler.hh"
#include "WsLogger.hh"
#include "WsOutputFacade.hh"
#include "WsInputFacade.hh"
#include "WsAppendageHandlerObservers.hh"
#include "Timer.h"
#include <math.h>
#include "WsTypes.hh"

#define ROBOT_GRABBER_RETRACT_TIME 0.30
#define ACCUMULATOR_RETRACT_TIME 0.50

#define FIRST_DEPLOY_TIME 0.25
#define SECOND_DEPLOY_TIME 0.25

WsAppendageHandler::WsAppendageHandler(void)
    : WsSubsystem("WsAppendageHandler")
    , a_currentState(WS_APPENDAGE_WAIT_STATE)
    , a_robotGrabberProcessedButtonState(false)
    , a_accumulatorProcessedButtonState(false)
    , ap_accumulatorDeployButtonObserver(NULL)
    , ap_robotGrabberButtonObserver(NULL)
    , ap_retractTimer(NULL)
{
    WS_LOG_TRACE("Begin");
    
    ap_retractTimer = new Timer();
    ap_retractTimer->Start();
    ap_retractTimer->Reset();
    
    //
    // The observers must be the last objects created here...
    //
    ap_accumulatorDeployButtonObserver = new WsAccumulatorDeployButtonObserver(this);
    ap_robotGrabberButtonObserver = new WsRobotGrabberButtonObserver(this);
    
    WS_LOG_TRACE("End");
}

WsAppendageHandler::~WsAppendageHandler(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_retractTimer;                     ap_retractTimer = NULL;
    delete ap_accumulatorDeployButtonObserver;  ap_accumulatorDeployButtonObserver = NULL;
    delete ap_robotGrabberButtonObserver;       ap_robotGrabberButtonObserver = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsAppendageHandler::acceptNewAccumulatorDeployButtonState(bool oldButtonState,
                                                          bool newButtonState)
{
    // Toggle the processed "sticky" button state on raw transition
    if ((false == oldButtonState) && (true == newButtonState))
    {
        a_accumulatorProcessedButtonState = !a_accumulatorProcessedButtonState;
    }
}

void
WsAppendageHandler::acceptNewRobotGrabberButtonState(bool oldButtonState,
                                                     bool newButtonState)
{
    // Toggle the processed "sticky" button state on raw transition
    if ((false == oldButtonState) && (true == newButtonState))
    {
        a_robotGrabberProcessedButtonState = !a_robotGrabberProcessedButtonState;
    }
}

void
WsAppendageHandler::update(void)
{
    WS_LOG_TRACE("Begin");
    
    WsTypes::WsAccumulatorStateType accumulator_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
    WsTypes::WsRobotGrabberCylinderStateType robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
    WsTypes::WsRobotGrabberCylinderStateType robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
    
    // Get a new state so we don't mess with a_currentState
    WsAppendageControlStateType new_state = a_currentState;
    
    // Perform state transitions as appropriate
    switch (a_currentState)
    {
        case WS_ROBOT_GRABBER_FIRST_DEPLOY_STATE:
        {
            accumulator_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
            robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED;
            robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            
            if (ap_retractTimer->HasPeriodPassed(FIRST_DEPLOY_TIME))
            {
                ap_retractTimer->Reset();
                new_state = WS_ROBOT_GRABBER_SECOND_DEPLOY_STATE;
            }
            
            if ((true == a_accumulatorProcessedButtonState) ||
                (false == a_robotGrabberProcessedButtonState))
            {
                // First, rectract the robot grabber
                robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
                robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
                
                // If the accumulator requests deployment, we could end up in a
                //  state where both are true. So, we will toggle the robot grabber
                //  button state to false
                a_robotGrabberProcessedButtonState = false;
                
                //
                // If the accumulator requests deployment while the robot grabber
                //  is deployed, we will be in a situation where both "sticky"
                //  buttons show as "true".  To avoid this, we are internally
                //  "un-sticking" the button associated with the current appendage,
                //  in this case, the accumulator.
                //
                // In the case where the accumulator sticky button is already
                //  showing as "false", this statement will have no effect.
                //
                a_robotGrabberProcessedButtonState = false;
                
                // Set our timer
                ap_retractTimer->Reset();
                
                // Advance to the next state...
                new_state = WS_ROBOT_GRABBER_RETRACTING_STATE;
            }
        } break;
        case WS_ROBOT_GRABBER_SECOND_DEPLOY_STATE:
        {
            accumulator_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
            robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED;
            robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED;
            
            if (ap_retractTimer->HasPeriodPassed(SECOND_DEPLOY_TIME))
            {
                ap_retractTimer->Reset();
                new_state = WS_ROBOT_GRABBER_DEPLOYED_STATE;
            }
            
            if ((true == a_accumulatorProcessedButtonState) ||
                (false == a_robotGrabberProcessedButtonState))
            {
                // First, rectract the robot grabber
                robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
                robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
                
                // If the accumulator requests deployment, we could end up in a
                //  state where both are true. So, we will toggle the robot grabber
                //  button state to false
                a_robotGrabberProcessedButtonState = false;
                
                //
                // If the accumulator requests deployment while the robot grabber
                //  is deployed, we will be in a situation where both "sticky"
                //  buttons show as "true".  To avoid this, we are internally
                //  "un-sticking" the button associated with the current appendage,
                //  in this case, the accumulator.
                //
                // In the case where the accumulator sticky button is already
                //  showing as "false", this statement will have no effect.
                //
                a_robotGrabberProcessedButtonState = false;
                
                // Set our timer
                ap_retractTimer->Reset();
                
                // Advance to the next state...
                new_state = WS_ROBOT_GRABBER_RETRACTING_STATE;
            }
        } break;
        case WS_ROBOT_GRABBER_DEPLOYED_STATE:
        {
            //
            // Currently, the robot grabber is deployed, we will retract the robot
            //  grabber if... 1. the driver requests accumulator deployment or 2. the
            //  driver requests the robot grabber retraction.
            //
            accumulator_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
            robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED;
            
            if ((true == a_accumulatorProcessedButtonState) ||
                (false == a_robotGrabberProcessedButtonState))
            {
                // First, rectract the robot grabber
                robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
                robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
                
                // If the accumulator requests deployment, we could end up in a
                //  state where both are true. So, we will toggle the robot grabber
                //  button state to false
                a_robotGrabberProcessedButtonState = false;
                
                //
                // If the accumulator requests deployment while the robot grabber
                //  is deployed, we will be in a situation where both "sticky"
                //  buttons show as "true".  To avoid this, we are internally
                //  "un-sticking" the button associated with the current appendage,
                //  in this case, the accumulator.
                //
                // In the case where the accumulator sticky button is already
                //  showing as "false", this statement will have no effect.
                //
                a_robotGrabberProcessedButtonState = false;
                
                // Set our timer
                ap_retractTimer->Reset();
                
                // Advance to the next state...
                new_state = WS_ROBOT_GRABBER_RETRACTING_STATE;
            }
        } break;
        
        case WS_ROBOT_GRABBER_RETRACTING_STATE:
        {
            //
            // Currently, the robot grabber on its way back in.  We will not
            //  acknowledge any human input during this time.  We must wait for
            //  the robot grabber to fully retract before processing additional
            //  commands.
            //
            accumulator_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
            robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            
            // Wait for the retracting command to finish
            if (true == ap_retractTimer->HasPeriodPassed(ROBOT_GRABBER_RETRACT_TIME))
            {
                new_state = WS_ROBOT_GRABBER_RETRACTED_STATE;
            }
        } break;
        
        case WS_APPENDAGE_WAIT_STATE:
        case WS_ACCUMULATOR_RETRACTED_STATE:
        case WS_ROBOT_GRABBER_RETRACTED_STATE:
        {
            //
            // Currently, all appendages are in their default "retracted" state.
            //  It is okay to process any/all human input regarding the desired
            //  position of these appendages.
            //
            accumulator_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
            robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            
            if ((true == a_accumulatorProcessedButtonState) &&
                (false == a_robotGrabberProcessedButtonState))
            {
                // It's safe to deploy the accumulator
                accumulator_state = WsTypes::WS_ACCUMULATOR_EXTENDED;
                
                // Advance to the next state...
                new_state = WS_ACCUMULATOR_DEPLOYED_STATE;
            }
            else if ((false == a_accumulatorProcessedButtonState) &&
                     (true == a_robotGrabberProcessedButtonState))
            {
                // It's safe to deploy the robot grabber
                robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED;
                
                // Advance to the next state...
                new_state = WS_ROBOT_GRABBER_FIRST_DEPLOY_STATE;
                ap_retractTimer->Reset();
            }
            else
            {
                // Invalid button config... stay right here...
                new_state = WS_APPENDAGE_WAIT_STATE;
            }
        } break;
        
        case WS_ACCUMULATOR_DEPLOYED_STATE:
        {
            //
            // Currently, the accumulator is deployed... we will retract the
            //  accumulator bar if... 1. the driver requests accumulator retraction
            //  or 2. the driver requests the robot grabber deployment.
            //
            accumulator_state = WsTypes::WS_ACCUMULATOR_EXTENDED;
            robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            
            if ((false == a_accumulatorProcessedButtonState) ||
                (true == a_robotGrabberProcessedButtonState))
            {
                // First, retract the accumulator
                accumulator_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
                
                //
                // If the robot grabber requests deployment while the accumulator
                //  is deployed, we will be in a situation where both "sticky"
                //  buttons show as "true".  To avoid this, we are internally
                //  "un-sticking" the button associated with the current appendage,
                //  in this case, the accumulator.
                //
                // In the case where the accumulator sticky button is already
                //  showing as "false", this statement will have no effect.
                //
                a_accumulatorProcessedButtonState = false;
                
                // Start our timer
                ap_retractTimer->Reset();
                
                // Advance to the next state...
                new_state = WS_ACCUMULATOR_RETRACTING_STATE;
            }
        } break;
        
        case WS_ACCUMULATOR_RETRACTING_STATE:
        {
            //
            // Currently, the accumulator on its way back in.  We will not
            //  acknowledge any human input during this time.  We must wait for
            //  the accumulator to fully retract before processing additional
            //  commands.
            //
            accumulator_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
            robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            
            // Wait for the retracting command to finish
            if (true == ap_retractTimer->HasPeriodPassed(ACCUMULATOR_RETRACT_TIME))
            {
                new_state = WS_ACCUMULATOR_RETRACTED_STATE;
            }
        } break;
        
        case WS_APPENDAGE_STATE_NUM:
        default:
        {
            //
            // This is unfortunate...
            //  We never expect this to happen, but if it does, let's be safe!
            //
            WS_LOG_ERROR("Unexpected state %d, valid range [%d,%d]",
                         a_currentState,
                         (int)WS_ROBOT_GRABBER_DEPLOYED_STATE,
                         (int)WS_ACCUMULATOR_RETRACTED_STATE);
                         
            // Let's make sure all subsystems are retracted, and go to a good state
            accumulator_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
            robot_grabber_first_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            robot_grabber_second_stage_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
            
            new_state = WS_ACCUMULATOR_RETRACTING_STATE;
        } break;
    } // end switch(a_currentState)
    
    // Update the actuators accordingly
    WsOutputFacade::instance()->setRobotGrabberOuterCylinderState(robot_grabber_first_stage_state);
    WsOutputFacade::instance()->setRobotGrabberInnerCylinderState(robot_grabber_second_stage_state);
    WsOutputFacade::instance()->setAccumulatorState(accumulator_state);
    
    // Don't forget to update the new state
    a_currentState = new_state;
    
    WS_LOG_TRACE("End");
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

