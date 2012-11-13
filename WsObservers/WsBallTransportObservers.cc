#include "WsBallTransportObservers.hh"
#include "WsInputFacade.hh"
#include "WsBallTransport.hh"
#include "WsLogger.hh"

//-----------------------------------------------------------------------------
// WsBtAccumulatorMotorButtonObserver
//-----------------------------------------------------------------------------
WsBtAccumulatorMotorButtonObserver::
WsBtAccumulatorMotorButtonObserver(WsBallTransport* p_ballTransport)
    : WsObserver("WsBtAccumulatorMotorButtonObserver")
    , ap_ballTransportSubsystem(p_ballTransport)
    , a_currentButtonState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getAccumulatorMotorButtonState(a_currentButtonState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_ballTransportSubsystem->acceptNewAccumulatorMotorButtonState(a_currentButtonState);
}

WsBtAccumulatorMotorButtonObserver::
~WsBtAccumulatorMotorButtonObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getAccumulatorMotorButtonState(new_btn_state);
    p_subject->detach(this);
}

void
WsBtAccumulatorMotorButtonObserver::update(void)
{
    // Find out what the new button state is
    bool new_btn_state = false;
    WsInputFacade::instance()->getAccumulatorMotorButtonState(new_btn_state);
    WS_LOG_DEBUG("a_currentButtonState = %d, new_btn_state = %d",
                 a_currentButtonState,
                 new_btn_state);
                 
    // Push the current value into the subsystem
    ap_ballTransportSubsystem->acceptNewAccumulatorMotorButtonState(new_btn_state);
    
    // Note the new value
    a_currentButtonState = new_btn_state;
}

//-----------------------------------------------------------------------------
// WsBtEscalatorDownButtonObserver
//-----------------------------------------------------------------------------
WsBtEscalatorDownButtonObserver::
WsBtEscalatorDownButtonObserver(WsBallTransport* p_ballTransport)
    : WsObserver("WsBtEscalatorDownButtonObserver")
    , ap_ballTransportSubsystem(p_ballTransport)
    , a_currentButtonState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getEscalatorDownButtonState(a_currentButtonState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_ballTransportSubsystem->acceptNewEscalatorDownButtonState(a_currentButtonState);
}

WsBtEscalatorDownButtonObserver::
~WsBtEscalatorDownButtonObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getEscalatorDownButtonState(new_btn_state);
    p_subject->detach(this);
}

void
WsBtEscalatorDownButtonObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getEscalatorDownButtonState(new_btn_state);
    WS_LOG_DEBUG("a_currentButtonState = %d, new_btn_state = %d",
                 a_currentButtonState,
                 new_btn_state);
                 
    // Push the new value into the subsystem
    ap_ballTransportSubsystem->acceptNewEscalatorDownButtonState(new_btn_state);
    
    // Note the new value
    a_currentButtonState = new_btn_state;
}

//-----------------------------------------------------------------------------
// WsBtEscalatorUpButtonObserver
//-----------------------------------------------------------------------------
WsBtEscalatorUpButtonObserver::
WsBtEscalatorUpButtonObserver(WsBallTransport* p_ballTransport)
    : WsObserver("WsBtEscalatorUpButtonObserver")
    , ap_ballTransportSubsystem(p_ballTransport)
    , a_currentButtonState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getEscalatorUpButtonState(a_currentButtonState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_ballTransportSubsystem->acceptNewEscalatorUpButtonState(a_currentButtonState);
}

WsBtEscalatorUpButtonObserver::
~WsBtEscalatorUpButtonObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getEscalatorUpButtonState(new_btn_state);
    p_subject->detach(this);
}

void
WsBtEscalatorUpButtonObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getEscalatorUpButtonState(new_btn_state);
    WS_LOG_DEBUG("a_currentButtonState = %d, new_btn_state = %d",
                 a_currentButtonState,
                 new_btn_state);
                 
    // Push the new value into the subsystem
    ap_ballTransportSubsystem->acceptNewEscalatorUpButtonState(new_btn_state);
    
    // Note the new value
    a_currentButtonState = new_btn_state;
}

//-----------------------------------------------------------------------------
// WsBtFlywheelSpeedObserver
//-----------------------------------------------------------------------------
WsBtFlywheelSpeedObserver::
WsBtFlywheelSpeedObserver(WsBallTransport* p_ballTransport)
    : WsObserver("WsBtFlywheelSpeedObserver")
    , ap_ballTransportSubsystem(p_ballTransport)
    , a_currentSpeed(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelRotationalSpeed(a_currentSpeed);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_ballTransportSubsystem->acceptNewFlywheelSpeed(a_currentSpeed);
}

WsBtFlywheelSpeedObserver::
~WsBtFlywheelSpeedObserver(void)
{
    float new_speed = 0.0f;
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelRotationalSpeed(new_speed);
    p_subject->detach(this);
}

void
WsBtFlywheelSpeedObserver::update(void)
{
    float new_speed = 0.0f;
    WsInputFacade::instance()->getFlywheelRotationalSpeed(new_speed);
    WS_LOG_DEBUG("a_currentSpeed = %.05f, new_speed = %.05f",
                 a_currentSpeed,
                 new_speed);
                 
    // Push the current value into the subsystem
    ap_ballTransportSubsystem->acceptNewFlywheelSpeed(new_speed);
    
    // Note the new value
    a_currentSpeed = new_speed;
}

//-----------------------------------------------------------------------------
// WsBtShooterButtonObserver
//-----------------------------------------------------------------------------
WsBtShooterButtonObserver::
WsBtShooterButtonObserver(WsBallTransport* p_ballTransport)
    : WsObserver("WsBtShooterButtonObserver")
    , ap_ballTransportSubsystem(p_ballTransport)
    , a_currentButtonState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getBallShootButtonState(a_currentButtonState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_ballTransportSubsystem->acceptNewShooterButtonState(a_currentButtonState);
}

WsBtShooterButtonObserver::
~WsBtShooterButtonObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getBallShootButtonState(new_btn_state);
    p_subject->detach(this);
}

void
WsBtShooterButtonObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getBallShootButtonState(new_btn_state);
    WS_LOG_DEBUG("a_currentButtonState = %d, new_btn_state = %d",
                 a_currentButtonState,
                 new_btn_state);
                 
    // Push the current value into the subsystem
    ap_ballTransportSubsystem->acceptNewShooterButtonState(new_btn_state);
    
    // Note the new value
    a_currentButtonState = new_btn_state;
}

//-----------------------------------------------------------------------------
// WsBtShooterOverrideButtonObserver
//-----------------------------------------------------------------------------
WsBtShooterOverrideButtonObserver::
WsBtShooterOverrideButtonObserver(WsBallTransport* p_ballTransport)
    : WsObserver("WsBtShooterOverrideButtonObserver")
    , ap_ballTransportSubsystem(p_ballTransport)
    , a_currentButtonState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getBallShootOverrideButtonState(a_currentButtonState);
    p_subject->attach(this);
    
    // Push the current value into the subsystem
    ap_ballTransportSubsystem->acceptNewShooterOverrideButtonState(a_currentButtonState);
}

WsBtShooterOverrideButtonObserver::~WsBtShooterOverrideButtonObserver(void)
{
    bool new_btn_state = false;
    WsSubject* p_subject =
        WsInputFacade::instance()->getBallShootOverrideButtonState(new_btn_state);
    p_subject->detach(this);
}

void
WsBtShooterOverrideButtonObserver::update(void)
{
    bool new_btn_state = false;
    WsInputFacade::instance()->getBallShootOverrideButtonState(new_btn_state);
    WS_LOG_DEBUG("a_currentButtonState = %d, new_btn_state = %d",
                 a_currentButtonState,
                 new_btn_state);
                 
    // Push the current value into the subsystem
    ap_ballTransportSubsystem->acceptNewShooterOverrideButtonState(new_btn_state);
    
    // Note the new value
    a_currentButtonState = new_btn_state;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

