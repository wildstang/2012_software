#include "WsBallTransport.hh"
#include "WsLogger.hh"
#include "WsBallTransportObservers.hh"
#include "WsOutputFacade.hh"

WsBallTransport::WsBallTransport(void)
    : WsSubsystem("WsBallTransport")
    , ap_accumulatorMotorButtonObserver(NULL)
    , ap_escalatorUpButtonObserver(NULL)
    , ap_escalatorDownButtonObserver(NULL)
    , ap_shooterButtonObserver(NULL)
    , ap_shooterOverrideButtonObserver(NULL)
    , ap_flywheelSpeedObserver(NULL)
    , a_accumulatorMotorButtonState(false)
    , a_escalatorUpButtonState(false)
    , a_escalatorDownButtonState(false)
    , a_shooterButtonState(false)
    , a_shooterOverrideButtonState(false)
    , a_flywheelSpeed(0.0f)
{
    WS_LOG_TRACE("Begin");
    
    ap_accumulatorMotorButtonObserver = new WsBtAccumulatorMotorButtonObserver(this);
    ap_escalatorUpButtonObserver      = new WsBtEscalatorUpButtonObserver(this);
    ap_escalatorDownButtonObserver    = new WsBtEscalatorDownButtonObserver(this);
    ap_shooterButtonObserver          = new WsBtShooterButtonObserver(this);
    ap_shooterOverrideButtonObserver  = new WsBtShooterOverrideButtonObserver(this);
    ap_flywheelSpeedObserver          = new WsBtFlywheelSpeedObserver(this);
    
    ::memset(&a_motorSpeeds, 0x00, sizeof(a_motorSpeeds));
    
    a_motorSpeeds.elevator_motor_speed_off = 0.0;
    a_motorSpeeds.elevator_motor_speed_up = 1.0;
    a_motorSpeeds.elevator_motor_speed_down = -1.0;
    a_motorSpeeds.escalator_motor_speed_off = 0.0;
    a_motorSpeeds.escalator_motor_speed_up = -1.0;
    a_motorSpeeds.escalator_motor_speed_down = 1.0;
    a_motorSpeeds.accumulator_motor_speed_off = 0.0;
    a_motorSpeeds.accumulator_motor_speed_pullin = 1.0;
    a_motorSpeeds.accumulator_motor_speed_pushout = -1.0;
    
    WS_LOG_TRACE("End");
}

WsBallTransport::~WsBallTransport(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_accumulatorMotorButtonObserver;  ap_accumulatorMotorButtonObserver = NULL;
    delete ap_escalatorUpButtonObserver;       ap_escalatorUpButtonObserver = NULL;
    delete ap_escalatorDownButtonObserver;     ap_escalatorDownButtonObserver = NULL;
    delete ap_shooterButtonObserver;           ap_shooterButtonObserver = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsBallTransport::update(void)
{
    WS_LOG_TRACE("Begin");
    
    this->updateAccumulator();
    this->updateEscalator();
    this->updateElevator();
    this->updateShooterCylinder();
    
    WS_LOG_TRACE("End");
}

void
WsBallTransport::updateAccumulator(void)
{
    //
    // The accumulator will be controlled by both the driver and the manipulator.
    //  The driver will cause the accumulator to deploy and the motors to turn
    //  on in the "pull-in" direction.  The manipulator will cause the motors
    //  to turn on in the "pull-in" or "push-out" direction based on the state
    //  of the escalator-override button.
    //
    float acc_motor_speed = 0.0f;
    
    if (true == (a_escalatorUpButtonState ^ a_escalatorDownButtonState))
    {
        //
        // The manipulator is trying to control the state of the accumulator motor
        //  (exclusively up or down, not both) and the manipulator is always given
        //  priority on this motor...
        //
        acc_motor_speed = (true == a_escalatorUpButtonState)
                          ? a_motorSpeeds.accumulator_motor_speed_pullin
                          : a_motorSpeeds.accumulator_motor_speed_pushout;
    }
    else
    {
        //
        // The manipulator doesn't care about the state of the accumulator motor.
        //  Therefore, we should pay attention to what the driver wants the
        //  accumulator to do.
        //
        acc_motor_speed = (true == a_accumulatorMotorButtonState)
                          ? a_motorSpeeds.accumulator_motor_speed_pullin
                          : a_motorSpeeds.accumulator_motor_speed_off;
    }
    
    // Update the output facade with the real accumulator motor speed
    WsOutputFacade::instance()->setAccumulatorSpeed(acc_motor_speed);
    
    WS_LOG_TRACE("End");
}

void
WsBallTransport::updateEscalator(void)
{
    float escalator_motor_speed = a_motorSpeeds.escalator_motor_speed_off;
    if (true == this->isClearToShoot())
    {
        escalator_motor_speed = a_motorSpeeds.escalator_motor_speed_up;
    }
    else if (true == (a_escalatorUpButtonState ^ a_escalatorDownButtonState))
    {
        escalator_motor_speed = (true == a_escalatorUpButtonState)
                                ? a_motorSpeeds.escalator_motor_speed_up
                                : a_motorSpeeds.escalator_motor_speed_down;
    }
    else
    {
        // Keep the motor off
        escalator_motor_speed = a_motorSpeeds.escalator_motor_speed_off;
    }
    WsOutputFacade::instance()->setEscalatorSpeed(escalator_motor_speed);
}

void
WsBallTransport::updateElevator(void)
{
    float elevator_motor_speed = (true == this->isClearToShoot())
                                 ? a_motorSpeeds.elevator_motor_speed_up
                                 : a_motorSpeeds.elevator_motor_speed_off;
    WsOutputFacade::instance()->setElevatorSpeed(elevator_motor_speed);
}

void
WsBallTransport::updateShooterCylinder(void)
{
    WsTypes::WsShooterCylinderStateType cyl_state =
        (true == this->isClearToShoot())
        ? WsTypes::WS_SHOOTER_CYLINDER_CLOSED
        : WsTypes::WS_SHOOTER_CYLINDER_OPEN;
    WsOutputFacade::instance()->setShooterCylinderState(cyl_state);
}

bool
WsBallTransport::isClearToShoot(void)
{
    bool shoot_ok = false;
    if (true == a_shooterButtonState)
    {
        // Manipulator is requesting to shoot
        if (a_flywheelSpeed > 30.0f)
        {
            // Flywheel is spinning fast enough not to die, proceed with the shot
            shoot_ok = true;
        }
        else if (true == a_shooterOverrideButtonState)
        {
            // User is also requesting "force shoot", so proceed
            shoot_ok = true;
        }
        else
        {
            shoot_ok = false;
        }
    }
    else
    {
        // User is not requesting to shoot, open the cylinder
        shoot_ok = false;
    }
    
    return shoot_ok;
}

void
WsBallTransport::acceptNewAccumulatorMotorButtonState(const bool& rc_newButtonState)
{
    a_accumulatorMotorButtonState = rc_newButtonState;
}

void
WsBallTransport::acceptNewEscalatorDownButtonState(const bool& rc_newButtonState)
{
    a_escalatorDownButtonState = rc_newButtonState;
}

void
WsBallTransport::acceptNewEscalatorUpButtonState(const bool& rc_newButtonState)
{
    a_escalatorUpButtonState = rc_newButtonState;
}

void
WsBallTransport::acceptNewShooterButtonState(const bool& rc_newButtonState)
{
    a_shooterButtonState = rc_newButtonState;
}

void
WsBallTransport::acceptNewShooterOverrideButtonState(const bool& rc_newButtonState)
{
    a_shooterOverrideButtonState = rc_newButtonState;
}

void
WsBallTransport::acceptNewFlywheelSpeed(const float& rc_newSpeed)
{
    a_flywheelSpeed = rc_newSpeed;
}

void
WsBallTransport::acceptNewFixedMotorSpeeds(WsTypes::WsBallTransportMotorValueType& r_speeds)
{
    a_motorSpeeds = r_speeds;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

