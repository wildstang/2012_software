#include "WsOutputFacade.hh"
#include "WsLogger.hh"
#include "Synchronized.h"
#include "Victor.h"
#include "WsVictor.hh"
#include "Servo.h"
#include "Solenoid.h"
#include "DigitalOutput.h"
#include "Relay.h"
#include "Solenoid.h"
#include "DoubleSolenoid.h"

#define OF_SMART_DASHBOARD_UPDATE_PERIOD 0.25
#define WS_USE_SMART_DASHBOARD false  // Change to true to use the smart dashboard

WsOutputFacade* WsOutputFacade::asp_instance = NULL;
WsOutputFacade::WsOutputFacade(void)
    : ap_mutexSemaphore(NULL)
    , ap_shooterCylinderStateElement(NULL)
    , ap_floodLightStateElement(NULL)
    , ap_flywheelSpeedElement(NULL)
    , ap_rightDriveMotorSpeedElement(NULL)
    , ap_leftDriveMotorSpeedElement(NULL)
    , ap_accumulatorMotorSpeedElement(NULL)
    , ap_turretMotorSpeedElement(NULL)
    , ap_escalatorMotorSpeedElement(NULL)
    , ap_elevatorMotorSpeedElement(NULL)
    , ap_hoodCylinderStateElement(NULL)
    , ap_accumulatorDeploymentStateElement(NULL)
    , ap_landingGearDeploymentStateElement(NULL)
    , ap_shifterStateElement(NULL)
    , ap_compressorRelayStateElement(NULL)
    , ap_robotGrabberInnerCylinderStateElement(NULL)
    , ap_robotGrabberOuterCylinderStateElement(NULL)
    , ap_pogoStickDeploymentStateElement(NULL)
    , ap_bridgeBrakeCylinderStateElement(NULL)
    // New elements above this line

    , ap_flywheelSpeedController(NULL)
    , ap_rightMotor1SpeedController(NULL)
    , ap_rightMotor2SpeedController(NULL)
    , ap_leftMotor1SpeedController(NULL)
    , ap_leftMotor2SpeedController(NULL)
    , ap_accumulatorSpeedController(NULL)
    , ap_turretSpeedController(NULL)
    , ap_escalatorSpeedController(NULL)
    , ap_elevatorSpeedController(NULL)
    , ap_hoodSolenoid(NULL)
    , ap_landingGearSolenoid(NULL)
    , ap_shifterSolenoid(NULL)
    , ap_floodLightRelay(NULL)
    , ap_compressorRelay(NULL)
    , ap_ballShooterSolenoid(NULL)
    , ap_robotGrabberInnerSolenoid(NULL)
    , ap_robotGrabberOuterSolenoid(NULL)
    , ap_accumulatorSolenoid(NULL)
    , ap_pogoStickSolenoid(NULL)
    , ap_bridgeBrakeSolenoid(NULL)
{
    // Create a mutex to protect our data
    ap_mutexSemaphore = semMCreate(SEM_Q_PRIORITY |
                                   SEM_DELETE_SAFE |
                                   SEM_INVERSION_SAFE);
                                   
                                   
    //-------------------------------------------------------------------------
    // Create All Facade Data Elements
    //-------------------------------------------------------------------------
    WsTypes::WsShooterCylinderStateType default_shooter_cyl_state =
        WsTypes::WS_SHOOTER_CYLINDER_OPEN;
    ap_shooterCylinderStateElement =
        new WsFacadeElement<WsTypes::WsShooterCylinderStateType>("ShooterCylinderState",
                                                                 sizeof(default_shooter_cyl_state),
                                                                 &default_shooter_cyl_state,
                                                                 ap_mutexSemaphore);
                                                                 
    bool default_flood_light_state = false;
    ap_floodLightStateElement =
        new WsFacadeElement<bool>("FloodLightState",
                                  sizeof(default_flood_light_state),
                                  &default_flood_light_state,
                                  ap_mutexSemaphore);
                                  
    float default_motor_speed = false;
    ap_flywheelSpeedElement =
        new WsFacadeElement<float>("FlywheelMotorSpeed",
                                   sizeof(default_motor_speed),
                                   &default_motor_speed,
                                   ap_mutexSemaphore);
                                   
    ap_rightDriveMotorSpeedElement =
        new WsFacadeElement<float>("RightDriveMotorSpeed",
                                   sizeof(default_motor_speed),
                                   &default_motor_speed,
                                   ap_mutexSemaphore);
                                   
    ap_leftDriveMotorSpeedElement =
        new WsFacadeElement<float>("LeftDriveMotorSpeed",
                                   sizeof(default_motor_speed),
                                   &default_motor_speed,
                                   ap_mutexSemaphore);
                                   
    ap_accumulatorMotorSpeedElement =
        new WsFacadeElement<float>("AccumulatorMotorSpeed",
                                   sizeof(default_motor_speed),
                                   &default_motor_speed,
                                   ap_mutexSemaphore);
                                   
    ap_turretMotorSpeedElement =
        new WsFacadeElement<float>("TurretMotorSpeed",
                                   sizeof(default_motor_speed),
                                   &default_motor_speed,
                                   ap_mutexSemaphore);
                                   
    ap_escalatorMotorSpeedElement =
        new WsFacadeElement<float>("EscalatorMotorSpeed",
                                   sizeof(default_motor_speed),
                                   &default_motor_speed,
                                   ap_mutexSemaphore);
                                   
    ap_elevatorMotorSpeedElement =
        new WsFacadeElement<float>("ElevatorMotorSpeed",
                                   sizeof(default_motor_speed),
                                   &default_motor_speed,
                                   ap_mutexSemaphore);
                                   
                                   
    WsTypes::WsPogoStickCylinderStateType default_pogo_stick_state =
        WsTypes::WS_POGO_STICK_CYLINDER_UP;
    ap_pogoStickDeploymentStateElement =
        new WsFacadeElement<WsTypes::WsPogoStickCylinderStateType>("PogoStickCylinderState",
                                                                   sizeof(default_pogo_stick_state),
                                                                   &default_pogo_stick_state,
                                                                   ap_mutexSemaphore);
                                                                   
    WsTypes::WsHoodCylinderStateType default_hood_cyl_state =
        WsTypes::WS_HOOD_CYLINDER_UP;
    ap_hoodCylinderStateElement =
        new WsFacadeElement<WsTypes::WsHoodCylinderStateType>("HoodCylinderState",
                                                              sizeof(default_hood_cyl_state),
                                                              &default_hood_cyl_state,
                                                              ap_mutexSemaphore);
                                                              
    WsTypes::WsAccumulatorStateType default_acc_deployment_state =
        WsTypes::WS_ACCUMULATOR_RETRACTED;
    ap_accumulatorDeploymentStateElement =
        new WsFacadeElement<WsTypes::WsAccumulatorStateType>("AccumulatorDeploymentState",
                                                             sizeof(default_acc_deployment_state),
                                                             &default_acc_deployment_state,
                                                             ap_mutexSemaphore);
                                                             
    WsTypes::WsLandingGearStateType default_lg_deployment_state =
        WsTypes::WS_LG_RETRACTED;
    ap_landingGearDeploymentStateElement =
        new WsFacadeElement<WsTypes::WsLandingGearStateType>("LandingGearDeploymentState",
                                                             sizeof(default_lg_deployment_state),
                                                             &default_lg_deployment_state,
                                                             ap_mutexSemaphore);
                                                             
    WsTypes::WsShifterStateType default_shifter_deployment_state =
        WsTypes::WS_S_HIGHGEAR;
    ap_shifterStateElement =
        new WsFacadeElement<WsTypes::WsShifterStateType>("ShifterState",
                                                         sizeof(default_shifter_deployment_state),
                                                         &default_shifter_deployment_state,
                                                         ap_mutexSemaphore);
                                                         
    WsTypes::WsCompressorStateType default_compressor_state =
        WsTypes::WS_COMPRESSOR_OFF;
    ap_compressorRelayStateElement =
        new WsFacadeElement<WsTypes::WsCompressorStateType>("CompressorState",
                                                            sizeof(default_compressor_state),
                                                            &default_compressor_state,
                                                            ap_mutexSemaphore);
                                                            
    WsTypes::WsRobotGrabberCylinderStateType default_rg_cylinder_state =
        WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
    ap_robotGrabberInnerCylinderStateElement =
        new WsFacadeElement<WsTypes::WsRobotGrabberCylinderStateType>("RobotGrabberInnerCylinderState",
                                                                      sizeof(default_rg_cylinder_state),
                                                                      &default_rg_cylinder_state,
                                                                      ap_mutexSemaphore);
                                                                      
    ap_robotGrabberOuterCylinderStateElement =
        new WsFacadeElement<WsTypes::WsRobotGrabberCylinderStateType>("RobotGrabberOuterCylinderState",
                                                                      sizeof(default_rg_cylinder_state),
                                                                      &default_rg_cylinder_state,
                                                                      ap_mutexSemaphore);
                                                                      
    WsTypes::WsBridgeBrakeCylinderStateType default_bridge_brake_state =
        WsTypes::WS_BRIDGE_BRAKE_CYLINDER_DISENGAGED;
    ap_bridgeBrakeCylinderStateElement =
        new WsFacadeElement<WsTypes::WsBridgeBrakeCylinderStateType>("BridgeBrakeCylinderState",
                                                                     sizeof(default_bridge_brake_state),
                                                                     &default_bridge_brake_state,
                                                                     ap_mutexSemaphore);
                                                                     
                                                                     
    //-------------------------------------------------------------------------
    // Create Output Actuator Objects
    //-------------------------------------------------------------------------
    // Create speed controllers & servos
    ap_leftMotor1SpeedController  = new WsVictor(1);
    ap_leftMotor2SpeedController  = new WsVictor(2);
    ap_rightMotor1SpeedController = new WsVictor(3);
    ap_rightMotor2SpeedController = new WsVictor(4);
    ap_flywheelSpeedController    = new WsVictor(5);
    ap_accumulatorSpeedController = new Victor(6);
    ap_escalatorSpeedController   = new Victor(7);
    ap_elevatorSpeedController    = new Victor(8);
    ap_turretSpeedController      = new Victor(9);
    
    // Set mapping files (for linearization) in WsVictor speed controller objects
    ap_leftMotor1SpeedController->setMappingFileName("/ws_drive_left_mapping.txt");
    ap_leftMotor2SpeedController->setMappingFileName("/ws_drive_left_mapping.txt");
    ap_rightMotor1SpeedController->setMappingFileName("/ws_drive_right_mapping.txt");
    ap_rightMotor2SpeedController->setMappingFileName("/ws_drive_right_mapping.txt");
    ap_flywheelSpeedController->setMappingFileName("/ws_flywheel_mapping.txt");
    
    // Create Spike Relays
    ap_compressorRelay = new Relay(1, Relay::kForwardOnly);
    ap_floodLightRelay = new Relay(2, Relay::kForwardOnly);
    
    // Create Solenoids (module 1)
    ap_landingGearSolenoid     = new Solenoid(1);
    ap_accumulatorSolenoid     = new Solenoid(3);
    ap_shifterSolenoid         = new Solenoid(4);
    ap_ballShooterSolenoid     = new Solenoid(5);
    ap_bridgeBrakeSolenoid     = new DoubleSolenoid(6, 7);
    ap_hoodSolenoid            = new Solenoid(8);
    
    // Create Solenoids (module 2)
    ap_robotGrabberInnerSolenoid    = new Solenoid(2, 1);
    ap_robotGrabberOuterSolenoid    = new Solenoid(2, 2);
    ap_pogoStickSolenoid            = new DoubleSolenoid(2, 3, 4);
}

WsOutputFacade::~WsOutputFacade(void)
{
    // Tear down our element data objects
    delete ap_shooterCylinderStateElement;            ap_shooterCylinderStateElement = NULL;
    delete ap_floodLightStateElement;                 ap_floodLightStateElement = NULL;
    delete ap_flywheelSpeedElement;                   ap_flywheelSpeedElement = NULL;
    delete ap_rightDriveMotorSpeedElement;            ap_rightDriveMotorSpeedElement = NULL;
    delete ap_leftDriveMotorSpeedElement;             ap_leftDriveMotorSpeedElement = NULL;
    delete ap_accumulatorMotorSpeedElement;           ap_accumulatorMotorSpeedElement = NULL;
    delete ap_turretMotorSpeedElement;                ap_turretMotorSpeedElement = NULL;
    delete ap_escalatorMotorSpeedElement;             ap_escalatorMotorSpeedElement = NULL;
    delete ap_elevatorMotorSpeedElement;              ap_elevatorMotorSpeedElement = NULL;
    delete ap_hoodCylinderStateElement;               ap_hoodCylinderStateElement = NULL;
    delete ap_accumulatorDeploymentStateElement;      ap_accumulatorDeploymentStateElement = NULL;
    delete ap_landingGearDeploymentStateElement;      ap_landingGearDeploymentStateElement = NULL;
    delete ap_shifterStateElement;                    ap_shifterStateElement = NULL;
    delete ap_compressorRelayStateElement;            ap_compressorRelayStateElement = NULL;
    delete ap_robotGrabberInnerCylinderStateElement;  ap_robotGrabberInnerCylinderStateElement = NULL;
    delete ap_robotGrabberOuterCylinderStateElement;  ap_robotGrabberOuterCylinderStateElement = NULL;
    delete ap_pogoStickDeploymentStateElement;        ap_pogoStickDeploymentStateElement = NULL;
    delete ap_bridgeBrakeCylinderStateElement;        ap_bridgeBrakeCylinderStateElement = NULL;
    
    // Tear down our data interface objects
    delete ap_flywheelSpeedController;       ap_flywheelSpeedController = NULL;
    delete ap_rightMotor1SpeedController;    ap_rightMotor1SpeedController = NULL;
    delete ap_rightMotor2SpeedController;    ap_rightMotor2SpeedController = NULL;
    delete ap_leftMotor1SpeedController;     ap_leftMotor1SpeedController = NULL;
    delete ap_leftMotor2SpeedController;     ap_leftMotor2SpeedController = NULL;
    delete ap_accumulatorSpeedController;    ap_accumulatorSpeedController = NULL;
    delete ap_landingGearSolenoid;           ap_landingGearSolenoid = NULL;
    delete ap_shifterSolenoid;               ap_shifterSolenoid = NULL;
    delete ap_floodLightRelay;               ap_floodLightRelay = NULL;
    delete ap_compressorRelay;               ap_compressorRelay = NULL;
    delete ap_turretSpeedController;         ap_turretSpeedController = NULL;
    delete ap_escalatorSpeedController;      ap_escalatorSpeedController = NULL;
    delete ap_elevatorSpeedController;       ap_elevatorSpeedController = NULL;
    delete ap_ballShooterSolenoid;           ap_ballShooterSolenoid = NULL;
    delete ap_hoodSolenoid;                  ap_hoodSolenoid = NULL;
    delete ap_robotGrabberInnerSolenoid;     ap_robotGrabberInnerSolenoid = NULL;
    delete ap_robotGrabberOuterSolenoid;     ap_robotGrabberOuterSolenoid = NULL;
    delete ap_accumulatorSolenoid;           ap_accumulatorSolenoid = NULL;
    delete ap_pogoStickSolenoid;             ap_pogoStickSolenoid = NULL;
    delete ap_bridgeBrakeSolenoid;           ap_bridgeBrakeSolenoid = NULL;
    
    // Let's clean up our mutex nicely
    semFlush(ap_mutexSemaphore);
    semDelete(ap_mutexSemaphore);
    ap_mutexSemaphore = NULL;
}

WsOutputFacade*
WsOutputFacade::instance(void)
{
    if (NULL == asp_instance)
    { asp_instance = new WsOutputFacade(); }
    return asp_instance;
}

void
WsOutputFacade::update(void)
{
    // Local variables
    float                                     flywheel_motor_speed = 0.0f;
    WsTypes::WsLandingGearStateType           landing_gear_deploy_state = WsTypes::WS_LG_RETRACTED;
    WsTypes::WsShifterStateType               shifter_state = WsTypes::WS_S_HIGHGEAR;
    float                                     accumulator_motor_speed = 0.0f;
    WsTypes::WsCompressorStateType            compressor_state = WsTypes::WS_COMPRESSOR_ON;
    float                                     turret_motor_speed = 0.0f;
    bool                                      flood_light_state = false;
    float                                     escalator_motor_speed = 0.0f;
    float                                     elevator_motor_speed = 0.0f;
    WsTypes::WsShooterCylinderStateType       shooter_cyl_state = WsTypes::WS_SHOOTER_CYLINDER_OPEN;
    WsTypes::WsHoodCylinderStateType          hood_cyl_state = WsTypes::WS_HOOD_CYLINDER_UP;
    WsTypes::WsRobotGrabberCylinderStateType  robot_grabber_inner_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
    WsTypes::WsRobotGrabberCylinderStateType  robot_grabber_outer_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
    WsTypes::WsAccumulatorStateType           accumulator_deployment_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
    float                                     right_drive_motor_speed = 0.0f;
    float                                     left_drive_motor_speed = 0.0f;
    WsTypes::WsPogoStickCylinderStateType     pogo_stick_state = WsTypes::WS_POGO_STICK_CYLINDER_UP;
    WsTypes::WsBridgeBrakeCylinderStateType   bridge_brake_state = WsTypes::WS_BRIDGE_BRAKE_CYLINDER_DISENGAGED;
    
    // Get this loop's output values
    this->getFlywheelMotorSpeed(flywheel_motor_speed);
    this->getLandingGearState(landing_gear_deploy_state);
    this->getShifterState(shifter_state);
    this->getAccumulatorSpeed(accumulator_motor_speed);
    this->getCompressorState(compressor_state);
    this->getTurretHeadingSpeed(turret_motor_speed);
    this->getFloodLightState(flood_light_state);
    this->getEscalatorSpeed(escalator_motor_speed);
    this->getElevatorSpeed(elevator_motor_speed);
    this->getShooterCylinderState(shooter_cyl_state);
    this->getHoodState(hood_cyl_state);
    this->getRobotGrabberInnerCylinderState(robot_grabber_inner_state);
    this->getRobotGrabberOuterCylinderState(robot_grabber_outer_state);
    this->getAccumulatorState(accumulator_deployment_state);
    this->getRightDriveMotorSpeed(right_drive_motor_speed);
    this->getLeftDriveMotorSpeed(left_drive_motor_speed);
    this->getPogoStickCylinderState(pogo_stick_state);
    this->getBridgeBrakeCylinderState(bridge_brake_state);
    
    // Pogo stick UP needs to set the solenoid to false
    DoubleSolenoid::Value pogo_val =
        (pogo_stick_state == WsTypes::WS_POGO_STICK_CYLINDER_DOWN)
        ? DoubleSolenoid::kForward
        : DoubleSolenoid::kReverse;
        
    // Pogo stick UP needs to set the solenoid to false
    DoubleSolenoid::Value bridge_brake_val =
        (bridge_brake_state == WsTypes::WS_BRIDGE_BRAKE_CYLINDER_ENGAGED)
        ? DoubleSolenoid::kForward
        : DoubleSolenoid::kReverse;
        
    // And set them in the actuator objects
    ap_flywheelSpeedController->Set(flywheel_motor_speed);
    ap_landingGearSolenoid->Set(WsTypes::WS_LG_DEPLOYED == landing_gear_deploy_state);
    ap_shifterSolenoid->Set(WsTypes::WS_S_HIGHGEAR == shifter_state);
    ap_accumulatorSpeedController->Set(accumulator_motor_speed);
    ap_compressorRelay->Set((WsTypes::WS_COMPRESSOR_ON == compressor_state) ? Relay::kOn : Relay::kOff);
    ap_turretSpeedController->Set(turret_motor_speed);
    ap_floodLightRelay->Set((true == flood_light_state) ? Relay::kOn : Relay::kOff);
    ap_escalatorSpeedController->Set(escalator_motor_speed);
    ap_elevatorSpeedController->Set(elevator_motor_speed);
    ap_ballShooterSolenoid->Set(WsTypes::WS_SHOOTER_CYLINDER_CLOSED == shooter_cyl_state);  // Normally open
    ap_hoodSolenoid->Set(WsTypes::WS_HOOD_CYLINDER_DOWN == hood_cyl_state);                 // Normally up
    ap_robotGrabberInnerSolenoid->Set(WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED == robot_grabber_inner_state);// Normally retracted
    ap_robotGrabberOuterSolenoid->Set(WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED == robot_grabber_outer_state);// Normally retracted
    ap_accumulatorSolenoid->Set(WsTypes::WS_ACCUMULATOR_EXTENDED == accumulator_deployment_state);   // Normally retracted
    ap_pogoStickSolenoid->Set(pogo_val);
    ap_bridgeBrakeSolenoid->Set(bridge_brake_val);
    
    //
    // The drive motors are physically oriented 180 degrees apart from each other.
    //  Normally, we might invert the value of the left and right motors, but
    //  for electrical debugging purposes, we chose to perform this inversion
    //  by connecting "M+" and "M-" from the Victors "backwards" for one side
    //  of the drive line
    //
    ap_rightMotor1SpeedController->Set(right_drive_motor_speed);
    ap_rightMotor2SpeedController->Set(right_drive_motor_speed);
    ap_leftMotor1SpeedController->Set(left_drive_motor_speed);
    ap_leftMotor2SpeedController->Set(left_drive_motor_speed);
}


//-----------------------------------------------------------------------------
// Accessor Methods (Get/Set)
//-----------------------------------------------------------------------------
WsSubject*
WsOutputFacade::getFlywheelMotorSpeed(float& r_speed)
{

    ap_flywheelSpeedElement->getState(r_speed);
    return ap_flywheelSpeedElement;
}

void
WsOutputFacade::setFlywheelMotorSpeed(const float& rc_speed)
{
    ap_flywheelSpeedElement->setState(rc_speed);
}

WsSubject*
WsOutputFacade::getLandingGearState(WsTypes::WsLandingGearStateType& r_state)
{
    ap_landingGearDeploymentStateElement->getState(r_state);
    return ap_landingGearDeploymentStateElement;
}

void
WsOutputFacade::setLandingGearState(const WsTypes::WsLandingGearStateType& rc_state)
{
    ap_landingGearDeploymentStateElement->setState(rc_state);
}

WsSubject*
WsOutputFacade::getShifterState(WsTypes::WsShifterStateType& r_state)
{
    ap_shifterStateElement->getState(r_state);
    return ap_shifterStateElement;
}

void
WsOutputFacade::setShifterState(const WsTypes::WsShifterStateType& rc_state)
{
    ap_shifterStateElement->setState(rc_state);
}

WsSubject*
WsOutputFacade::getRightDriveMotorSpeed(float& r_speed)
{
    ap_rightDriveMotorSpeedElement->getState(r_speed);
    return ap_rightDriveMotorSpeedElement;
}

void
WsOutputFacade::setRightDriveMotorSpeed(const float& rc_speed)
{
    ap_rightDriveMotorSpeedElement->setState(rc_speed);
}

WsSubject*
WsOutputFacade::getLeftDriveMotorSpeed(float& r_speed)
{
    ap_leftDriveMotorSpeedElement->getState(r_speed);
    return ap_leftDriveMotorSpeedElement;
}

void
WsOutputFacade::setLeftDriveMotorSpeed(const float& rc_speed)
{
    ap_leftDriveMotorSpeedElement->setState(rc_speed);
}

WsSubject*
WsOutputFacade::getTurretHeadingSpeed(float& r_speed)
{
    ap_turretMotorSpeedElement->getState(r_speed);
    return ap_turretMotorSpeedElement;
}

void
WsOutputFacade::setTurretHeadingSpeed(const float& rc_speed)
{
    ap_turretMotorSpeedElement->setState(rc_speed);
}

WsSubject*
WsOutputFacade::getAccumulatorSpeed(float& r_speed)
{
    ap_accumulatorMotorSpeedElement->getState(r_speed);
    return ap_accumulatorMotorSpeedElement;
}

void
WsOutputFacade::setAccumulatorSpeed(const float& rc_speed)
{
    ap_accumulatorMotorSpeedElement->setState(rc_speed);
}

WsSubject*
WsOutputFacade::getFloodLightState(bool& r_state)
{
    ap_floodLightStateElement->getState(r_state);
    return ap_floodLightStateElement;
}

void
WsOutputFacade::setFloodLightState(const bool& rc_state)
{
    ap_floodLightStateElement->setState(rc_state);
}

WsSubject*
WsOutputFacade::getCompressorState(WsTypes::WsCompressorStateType& r_state)
{
    ap_compressorRelayStateElement->getState(r_state);
    return ap_compressorRelayStateElement;
}

void
WsOutputFacade::setCompressorState(const WsTypes::WsCompressorStateType& rc_state)
{
    ap_compressorRelayStateElement->setState(rc_state);
}

WsSubject*
WsOutputFacade::getEscalatorSpeed(float& r_speed)
{
    ap_escalatorMotorSpeedElement->getState(r_speed);
    return ap_escalatorMotorSpeedElement;
}

void
WsOutputFacade::setEscalatorSpeed(const float& rc_speed)
{
    ap_escalatorMotorSpeedElement->setState(rc_speed);
}

WsSubject*
WsOutputFacade::getElevatorSpeed(float& r_speed)
{
    ap_elevatorMotorSpeedElement->getState(r_speed);
    return ap_elevatorMotorSpeedElement;
}

void
WsOutputFacade::setElevatorSpeed(const float& rc_speed)
{
    ap_elevatorMotorSpeedElement->setState(rc_speed);
}

WsSubject*
WsOutputFacade::getShooterCylinderState(WsTypes::WsShooterCylinderStateType& r_state)
{
    ap_shooterCylinderStateElement->getState(r_state);
    return ap_shooterCylinderStateElement;
}

void
WsOutputFacade::setShooterCylinderState(const WsTypes::WsShooterCylinderStateType& rc_state)
{
    ap_shooterCylinderStateElement->setState(rc_state);
}

WsSubject*
WsOutputFacade::getHoodState(WsTypes::WsHoodCylinderStateType& r_state)
{
    ap_hoodCylinderStateElement->getState(r_state);
    return ap_hoodCylinderStateElement;
}

void
WsOutputFacade::setHoodState(const WsTypes::WsHoodCylinderStateType& rc_state)
{
    ap_hoodCylinderStateElement->setState(rc_state);
}

WsSubject*
WsOutputFacade::getAccumulatorState(WsTypes::WsAccumulatorStateType& r_state)
{
    ap_accumulatorDeploymentStateElement->getState(r_state);
    return ap_accumulatorDeploymentStateElement;
}

void
WsOutputFacade::setAccumulatorState(const WsTypes::WsAccumulatorStateType& rc_state)
{
    ap_accumulatorDeploymentStateElement->setState(rc_state);
}

WsSubject*
WsOutputFacade::getRobotGrabberInnerCylinderState(WsTypes::WsRobotGrabberCylinderStateType& r_state)
{
    ap_robotGrabberInnerCylinderStateElement->getState(r_state);
    return ap_robotGrabberInnerCylinderStateElement;
}

void
WsOutputFacade::setRobotGrabberInnerCylinderState(const WsTypes::WsRobotGrabberCylinderStateType& rc_state)
{
    ap_robotGrabberInnerCylinderStateElement->setState(rc_state);
}

WsSubject*
WsOutputFacade::getRobotGrabberOuterCylinderState(WsTypes::WsRobotGrabberCylinderStateType& r_state)
{
    ap_robotGrabberOuterCylinderStateElement->getState(r_state);
    return ap_robotGrabberOuterCylinderStateElement;
}

void
WsOutputFacade::setRobotGrabberOuterCylinderState(const WsTypes::WsRobotGrabberCylinderStateType& rc_state)
{
    ap_robotGrabberOuterCylinderStateElement->setState(rc_state);
}

WsSubject*
WsOutputFacade::getPogoStickCylinderState(WsTypes::WsPogoStickCylinderStateType& r_state)
{
    ap_pogoStickDeploymentStateElement->getState(r_state);
    return ap_pogoStickDeploymentStateElement;
}

void
WsOutputFacade::setPogoStickCylinderState(const WsTypes::WsPogoStickCylinderStateType& rc_state)
{
    ap_pogoStickDeploymentStateElement->setState(rc_state);
}

WsSubject*
WsOutputFacade::getBridgeBrakeCylinderState(WsTypes::WsBridgeBrakeCylinderStateType& r_state)
{
    ap_bridgeBrakeCylinderStateElement->getState(r_state);
    return ap_bridgeBrakeCylinderStateElement;
}

void
WsOutputFacade::setBridgeBrakeCylinderState(const WsTypes::WsBridgeBrakeCylinderStateType& rc_state)
{
    ap_bridgeBrakeCylinderStateElement->setState(rc_state);
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

