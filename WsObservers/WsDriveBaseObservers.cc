#include "WsDriveBaseObservers.hh"
#include "WsInputFacade.hh"
#include "WsDriveBase.hh"
#include "WsLogger.hh"

//-----------------------------------------------------------------------------
// WsAntiturboButtonObserver
//-----------------------------------------------------------------------------
WsAntiturboButtonObserver::
WsAntiturboButtonObserver(WsDriveBase* p_subsystem)
    : WsObserver("WsAntiturboButtonObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_antiturbo_button_subject =
        WsInputFacade::instance()->getAntiTurboButtonState(a_currentState);
    p_antiturbo_button_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewAntiturboButtonState(a_currentState, a_currentState);
}

WsAntiturboButtonObserver::
~WsAntiturboButtonObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_shifter_button_subject =
        WsInputFacade::instance()->getAntiTurboButtonState(new_btn_state);
    p_shifter_button_subject->detach(this);
}

void
WsAntiturboButtonObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getAntiTurboButtonState(new_btn_state);
    WS_LOG_DEBUG("a_currentState = %d, new_btn_state = %d",
                 a_currentState,
                 new_btn_state);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewAntiturboButtonState(a_currentState, new_btn_state);
    
    // Note the new value
    a_currentState = new_btn_state;
}

//-----------------------------------------------------------------------------
// WsLandingGearButtonObserver
//-----------------------------------------------------------------------------
WsLandingGearButtonObserver::
WsLandingGearButtonObserver(WsDriveBase* p_subsystem)
    : WsObserver("WsLandingGearButtonObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_landing_gear_button_subject =
        WsInputFacade::instance()->getLandingGearButtonState(a_currentState);
    p_landing_gear_button_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->notifyLandingGearInputChange();
}

WsLandingGearButtonObserver::
~WsLandingGearButtonObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_landing_gear_button_subject =
        WsInputFacade::instance()->getLandingGearButtonState(new_btn_state);
    p_landing_gear_button_subject->detach(this);
}

void
WsLandingGearButtonObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getLandingGearButtonState(new_btn_state);
    WS_LOG_DEBUG("a_currentState = %d, new_btn_state = %d",
                 a_currentState,
                 new_btn_state);
                 
    // Notify the drivebase that an input has changed
    //The subsystem will read the values itself because of the dependency of the button and switch
    ap_subsystem->notifyLandingGearInputChange();
    
    // Note the new value
    a_currentState = new_btn_state;
}

//-----------------------------------------------------------------------------
// WsLandingGearRetractionSwitchObserver
//-----------------------------------------------------------------------------
WsLandingGearRetractionSwitchObserver::
WsLandingGearRetractionSwitchObserver(WsDriveBase* p_subsystem)
    : WsObserver("WsLandingGearRetractionSwitchObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_landing_gear_retraction_switch_subject =
        WsInputFacade::instance()->getLandingGearRetractionSwitch(a_currentState);
    p_landing_gear_retraction_switch_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->notifyLandingGearInputChange();
}

WsLandingGearRetractionSwitchObserver::
~WsLandingGearRetractionSwitchObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_landing_gear_retraction_switch_subject =
        WsInputFacade::instance()->getLandingGearRetractionSwitch(new_btn_state);
    p_landing_gear_retraction_switch_subject->detach(this);
}

void
WsLandingGearRetractionSwitchObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getLandingGearRetractionSwitch(new_btn_state);
    WS_LOG_DEBUG("a_currentState = %d, new_btn_state = %d",
                 a_currentState,
                 new_btn_state);
                 
    // Notify the drivebase that an input has changed
    //The subsystem will read the values itself because of the dependency of the button and switch
    ap_subsystem->notifyLandingGearInputChange();
    
    // Note the new value
    a_currentState = new_btn_state;
}

//-----------------------------------------------------------------------------
// WsShifterButtonObserver
//-----------------------------------------------------------------------------
WsShifterButtonObserver::
WsShifterButtonObserver(WsDriveBase* p_subsystem)
    : WsObserver("WsShifterButtonObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_shifter_button_subject =
        WsInputFacade::instance()->getShifterButtonState(a_currentState);
    p_shifter_button_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewShifterButtonState(a_currentState, a_currentState);
}

WsShifterButtonObserver::
~WsShifterButtonObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_shifter_button_subject =
        WsInputFacade::instance()->getShifterButtonState(new_btn_state);
    p_shifter_button_subject->detach(this);
}

void
WsShifterButtonObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getShifterButtonState(new_btn_state);
    WS_LOG_DEBUG("a_currentState = %d, new_btn_state = %d",
                 a_currentState,
                 new_btn_state);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewShifterButtonState(a_currentState, new_btn_state);
    
    // Note the new value
    a_currentState = new_btn_state;
}

//-----------------------------------------------------------------------------
// WsAccelerationEnableFlagObserver
//-----------------------------------------------------------------------------
WsAccelerationEnableFlagObserver::
WsAccelerationEnableFlagObserver(WsDriveBase* p_subsystem)
    : WsObserver("WsAccelerationEnableFlagObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_shifter_button_subject =
        WsInputFacade::instance()->getDriveBaseAccelerationButtonState(a_currentState);
    p_shifter_button_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_subsystem->acceptNewAccelerationEnableFlag(a_currentState, a_currentState);
}

WsAccelerationEnableFlagObserver::
~WsAccelerationEnableFlagObserver(void)
{
    bool new_state = false;
    WsSubject* p_shifter_button_subject =
        WsInputFacade::instance()->getDriveBaseAccelerationButtonState(new_state);
    p_shifter_button_subject->detach(this);
}

void
WsAccelerationEnableFlagObserver::update(void)
{
    bool new_state = false;
    WsInputFacade::instance()->getDriveBaseAccelerationButtonState(new_state);
    WS_LOG_DEBUG("a_currentState = %d, new_state = %d",
                 a_currentState,
                 new_state);
                 
    // Push the current value into the subsystem
    ap_subsystem->acceptNewAccelerationEnableFlag(a_currentState, new_state);
    
    // Note the new value
    a_currentState = new_state;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

