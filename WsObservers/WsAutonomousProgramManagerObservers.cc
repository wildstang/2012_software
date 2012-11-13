#include "WsAutonomousProgramManagerObservers.hh"
#include "WsInputFacade.hh"
#include "WsAutonomousProgramManager.hh"
#include "WsLogger.hh"

//-----------------------------------------------------------------------------
//class WsAutoLockInSwitchObserver : public WsObserver
//-----------------------------------------------------------------------------

WsAutoLockInSwitchObserver::WsAutoLockInSwitchObserver(WsAutonomousProgramManager* p_autoProgramMan)
    : WsObserver("WsAutoLockInSwitchObserver")
    , ap_autoProgramMan(p_autoProgramMan)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getLockInSwitchState(a_currentState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_autoProgramMan->acceptNewLockInState(a_currentState, a_currentState);
}

WsAutoLockInSwitchObserver::~WsAutoLockInSwitchObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getLockInSwitchState(a_currentState);
    p_subject->detach(this);
}

void
WsAutoLockInSwitchObserver::update(void)
{
    bool new_state = false;
    WsInputFacade::instance()->getLockInSwitchState(new_state);
    
    WS_LOG_DEBUG("update");
    
    // Push the current value into the subsystem
    ap_autoProgramMan->acceptNewLockInState(new_state, a_currentState);
    
    a_currentState = new_state;
}

//-----------------------------------------------------------------------------
//class WsAutoProgramSelectorObserver : public WsObserver
//-----------------------------------------------------------------------------

WsAutoProgramSelectorObserver::WsAutoProgramSelectorObserver(WsAutonomousProgramManager* p_autoProgramMan)
    : WsObserver("WsAutoProgramSelectorObserver")
    , ap_autoProgramMan(p_autoProgramMan)
    , a_currentValue(false)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getAutoProgramSelectorValue(a_currentValue);
    p_subject->attach(this);
    ap_autoProgramMan->acceptNewProgramSelectorValue(a_currentValue);
}

WsAutoProgramSelectorObserver::~WsAutoProgramSelectorObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getAutoProgramSelectorValue(a_currentValue);
    p_subject->detach(this);
}

void
WsAutoProgramSelectorObserver::update(void)
{
    WsInputFacade::instance()->getAutoProgramSelectorValue(a_currentValue);
    ap_autoProgramMan->acceptNewProgramSelectorValue(a_currentValue);
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

