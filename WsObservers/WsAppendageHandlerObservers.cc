#include "WsAppendageHandlerObservers.hh"
#include "WsInputFacade.hh"
#include "WsAppendageHandler.hh"
#include "WsLogger.hh"

//-----------------------------------------------------------------------------
// WsAccumulatorDeployButtonObserver
//-----------------------------------------------------------------------------
WsAccumulatorDeployButtonObserver::
WsAccumulatorDeployButtonObserver(WsAppendageHandler* p_appendageHandler)
    : WsObserver("WsAccumulatorDeployButtonObserver")
    , ap_appendageHandlerSubsystem(p_appendageHandler)
    , a_accumulatorDeployButtonState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getAccumulatorDeployButtonState(a_accumulatorDeployButtonState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_appendageHandlerSubsystem->acceptNewAccumulatorDeployButtonState(a_accumulatorDeployButtonState, a_accumulatorDeployButtonState);
}

WsAccumulatorDeployButtonObserver::
~WsAccumulatorDeployButtonObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getAccumulatorDeployButtonState(new_btn_state);
    p_subject->detach(this);
}

void
WsAccumulatorDeployButtonObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getAccumulatorDeployButtonState(new_btn_state);
    WS_LOG_DEBUG("a_accumulatorButtonState = %d, new_btn_state = %d",
                 a_accumulatorDeployButtonState,
                 new_btn_state);
                 
    // Push the current value into the subsystem
    ap_appendageHandlerSubsystem->acceptNewAccumulatorDeployButtonState(a_accumulatorDeployButtonState, new_btn_state);
    
    // Note the new value
    a_accumulatorDeployButtonState = new_btn_state;
}

//-----------------------------------------------------------------------------
// WsRobotGrabberButtonObserver
//-----------------------------------------------------------------------------
WsRobotGrabberButtonObserver::
WsRobotGrabberButtonObserver(WsAppendageHandler* p_appendageHandler)
    : WsObserver("WsRobotGrabberButtonObserver")
    , ap_appendageHandlerSubsystem(p_appendageHandler)
    , a_robotGrabberButtonState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getRobotGrabberButtonState(a_robotGrabberButtonState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_appendageHandlerSubsystem->acceptNewRobotGrabberButtonState(a_robotGrabberButtonState, a_robotGrabberButtonState);
}

WsRobotGrabberButtonObserver::
~WsRobotGrabberButtonObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getRobotGrabberButtonState(new_btn_state);
    p_subject->detach(this);
}

void
WsRobotGrabberButtonObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getRobotGrabberButtonState(new_btn_state);
    WS_LOG_DEBUG("a_robotGrabberButtonState = %d, new_btn_state = %d",
                 a_robotGrabberButtonState,
                 new_btn_state);
                 
    // Push the current value into the subsystem
    ap_appendageHandlerSubsystem->acceptNewRobotGrabberButtonState(a_robotGrabberButtonState, new_btn_state);
    
    // Note the new value
    a_robotGrabberButtonState = new_btn_state;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

