#include "WsInputFacade.hh"
#include "WsLogger.hh"
#include "Synchronized.h"
#include "AnalogChannel.h"
#include "Joystick.h"
#include "DriverStation.h"
#include "DigitalInput.h"
#include "Encoder.h"
#include "Timer.h"
#include "WsGyro.hh"
#include "WsTypes.hh"
#include "WsFacadeElement.hh"
#include <math.h>
#include <list>

// Static variables for various turret measurements
static float turret_center_voltage = 2.5;
static float turret_cw_voltage = 5.0;
static float turret_ccw_voltage = 0.0;
static float turret_center_degrees = 0.0;
static float turret_cw_degrees = 135;
static float turret_ccw_degrees = -135;
static const unsigned int WS_FLYWHEEL_TICK_LIST_DEPTH = 4;

WsInputFacade* WsInputFacade::asp_instance = NULL;
WsInputFacade::WsInputFacade()
    : ap_mutexSemaphore(NULL)
    , ap_turretHeadingPotentiometer(NULL)
    , ap_driverJoystick(NULL)
    , ap_manipulatorJoystick(NULL)
    , ap_flywheelSpeedEncoder(NULL)
    , ap_pressureSwitchDigitalInput(NULL)
    , ap_leftDriveEncoder(NULL)
    , ap_rightDriveEncoder(NULL)
    , ap_turretCwLimitSwitchDigitalInput(NULL)
    , ap_turretCcwLimitSwitchDigitalInput(NULL)
    , ap_landingGearRetractionDigitalInput(NULL)
    , ap_pneumaticPressureTransducer(NULL)
    , ap_robotHeadingGyro(NULL)
    , a_sensorData()
    , a_flywheelSnapshotList()
    , ap_hoodButtonElement(NULL)
    , ap_flywheelRotationalSpeedElement(NULL)
    , ap_accumulatorDeployButtonElement(NULL)
    , ap_robotGrabberButtonElement(NULL)
    , ap_shooterButtonElement(NULL)
    , ap_shooterOverrideButtonElement(NULL)
    , ap_escalatorUpButtonElement(NULL)
    , ap_escalatorDownButtonElement(NULL)
    , ap_pressureSwitchElement(NULL)
    , ap_flywheelStatePotValueElement(NULL)
    , ap_flywheelManualPotElement(NULL)
    , ap_turretRequestedDirectionElement(NULL)
    , ap_turretHeadingPotValueElement(NULL)
    , ap_turretCwLimitSwitchElement(NULL)
    , ap_turretCcwLimitSwitchElement(NULL)
    , ap_turretTurboButtonElement(NULL)
    , ap_driveThrottleElement(NULL)
    , ap_driveHeadingElement(NULL)
    , ap_antiturboButtonElement(NULL)
    , ap_shifterButtonElement(NULL)
    , ap_landingGearButtonElement(NULL)
    , ap_landingGearRetractionSwitchElement(NULL)
    , ap_lockInSwitchElement(NULL)
    , ap_floodLightStateElement(NULL)
    , ap_pogoStickButtonElement(NULL)
    , ap_freshImageFlagElement(NULL)
    , ap_autonomousProgramIdElement(NULL)
    , ap_numVisibleTargetsElement(NULL)
    , ap_autoProgramSelectorElement(NULL)
    , ap_shooterPresetElement(NULL)
    , ap_leftDriveEncoderElement(NULL)
    , ap_rightDriveEncoderElement(NULL)
    , ap_pneumaticPressureReadingElement(NULL)
    , ap_driveBaseAccelerationButtonElement(NULL)
    , ap_accumulatorMotorButtonElement(NULL)
    , ap_robotHeadingGyroAngleElement(NULL)
    , ap_bridgeBrakeButtonElement(NULL)
    , ap_flywheelSpeedCompensationEnableSwitchElement(NULL)
    , ap_flywheelSpeedCompensationPotElement(NULL)
{
    // First, create a mutex to protect our input data
    ap_mutexSemaphore = semMCreate(SEM_Q_PRIORITY |
                                   SEM_DELETE_SAFE |
                                   SEM_INVERSION_SAFE);
                                   
    // Next, make sure we start with a fresh set of data
    ::memset(&a_sensorData, 0x00, sizeof(a_sensorData));
    
    // Make sure the flywheel data is cleared out
    a_flywheelSnapshotList.clear();
    
    // Now, create and initialize all data interfaces
    ap_driverJoystick = new Joystick(1);
    ap_manipulatorJoystick = new Joystick(2, 6, 2);
    ap_manipulatorJoystick->SetAxisChannel(Joystick::kXAxis, 1);
    ap_manipulatorJoystick->SetAxisChannel(Joystick::kYAxis, 2);
    ap_manipulatorJoystick->SetAxisChannel(Joystick::kZAxis, 3);
    ap_manipulatorJoystick->SetAxisChannel(Joystick::kTwistAxis, 5);
    ap_manipulatorJoystick->SetAxisChannel(Joystick::kThrottleAxis, 6);
    
    // Create Digital Inputs & Related
    ap_pressureSwitchDigitalInput = new DigitalInput(1);
    ap_leftDriveEncoder = new Encoder(2, 3, true, CounterBase::k4X);
    ap_leftDriveEncoder->Reset();
    ap_leftDriveEncoder->Start();
    ap_rightDriveEncoder = new Encoder(4, 5, false, CounterBase::k4X);
    ap_rightDriveEncoder->Reset();
    ap_rightDriveEncoder->Start();
    ap_flywheelSpeedEncoder = new Encoder(9, 10, true, CounterBase::k1X); // @TODO:  Make sure that the proper encoder gets reversed
    ap_flywheelSpeedEncoder->Reset();
    ap_flywheelSpeedEncoder->Start();
    ap_landingGearRetractionDigitalInput = new DigitalInput(8);
    // @TODO:  Turret "home" input on 14?
    ap_turretCcwLimitSwitchDigitalInput = new DigitalInput(11);
    ap_turretCwLimitSwitchDigitalInput = new DigitalInput(13);
    
    // Analog Inputs
    ap_robotHeadingGyro = new WsGyro(1);
    ap_turretHeadingPotentiometer = new AnalogChannel(2);
    ap_pneumaticPressureTransducer = new AnalogChannel(7);
    
    // New WsFacadeElement implementation
    bool default_hood_button_state = false;
    ap_hoodButtonElement =
        new WsFacadeElement<bool>("HoodButtonState",
                                  sizeof(default_hood_button_state),
                                  &default_hood_button_state,
                                  ap_mutexSemaphore);
                                  
    float default_flywheel_speed_value = 0.0f;
    ap_flywheelRotationalSpeedElement =
        new WsFacadeElement<float>("FlywheelSpeedValue",
                                   sizeof(default_flywheel_speed_value),
                                   &default_flywheel_speed_value,
                                   ap_mutexSemaphore);
                                   
    bool default_accumulator_deploy_button_state = false;
    ap_accumulatorDeployButtonElement =
        new WsFacadeElement<bool>("AccumulatorDeployButtonState",
                                  sizeof(default_accumulator_deploy_button_state),
                                  &default_accumulator_deploy_button_state,
                                  ap_mutexSemaphore);
                                  
    bool default_robot_grabber_button_state = false;
    ap_robotGrabberButtonElement =
        new WsFacadeElement<bool>("RobotGrabberButtonState",
                                  sizeof(default_robot_grabber_button_state),
                                  &default_robot_grabber_button_state,
                                  ap_mutexSemaphore);
                                  
    bool default_shooter_button_state = false;
    ap_shooterButtonElement =
        new WsFacadeElement<bool>("ShooterButtonState",
                                  sizeof(default_shooter_button_state),
                                  &default_shooter_button_state,
                                  ap_mutexSemaphore);
                                  
    bool default_shooter_override_button_state = false;
    ap_shooterOverrideButtonElement =
        new WsFacadeElement<bool>("ShooterOverrideButtonState",
                                  sizeof(default_shooter_override_button_state),
                                  &default_shooter_override_button_state,
                                  ap_mutexSemaphore);
                                  
    bool default_escalator_up_button_state = false;
    ap_escalatorUpButtonElement =
        new WsFacadeElement<bool>("EscalatorUpButtonState",
                                  sizeof(default_escalator_up_button_state),
                                  &default_escalator_up_button_state,
                                  ap_mutexSemaphore);
                                  
    bool default_escalator_down_button_state = false;
    ap_escalatorDownButtonElement =
        new WsFacadeElement<bool>("EscalatorDownButtonState",
                                  sizeof(default_escalator_down_button_state),
                                  &default_escalator_down_button_state,
                                  ap_mutexSemaphore);
                                  
    bool default_pressure_switch_state = false;
    ap_pressureSwitchElement =
        new WsFacadeElement<bool>("PressureSwitchState",
                                  sizeof(default_pressure_switch_state),
                                  &default_pressure_switch_state,
                                  ap_mutexSemaphore);
                                  
    float default_flywheel_pot_val = 0.0f;
    ap_flywheelStatePotValueElement =
        new WsFacadeElement<float>("FlywheelStatePotValue",
                                   sizeof(default_flywheel_pot_val),
                                   &default_flywheel_pot_val,
                                   ap_mutexSemaphore);
                                   
    float default_manual_flywheel_pot_val = 0.0f;
    ap_flywheelManualPotElement =
        new WsFacadeElement<float>("FlywheelManualPot",
                                   sizeof(default_manual_flywheel_pot_val),
                                   &default_manual_flywheel_pot_val,
                                   ap_mutexSemaphore);
                                   
    float default_turret_axis_value = 0.0;
    ap_turretRequestedDirectionElement =
        new WsFacadeElement<float>("TurretRequestedDirection",
                                   sizeof(default_turret_axis_value),
                                   &default_turret_axis_value,
                                   ap_mutexSemaphore);
                                   
    float default_turret_heading_value = 0.0;
    ap_turretHeadingPotValueElement =
        new WsFacadeElement<float>("TurretHeadingPotValue",
                                   sizeof(default_turret_heading_value),
                                   &default_turret_heading_value,
                                   ap_mutexSemaphore);
                                   
    bool default_turret_cw_limit_swtich_state = false;
    ap_turretCwLimitSwitchElement =
        new WsFacadeElement<bool>("TurretCwLimitSwitchState",
                                  sizeof(default_turret_cw_limit_swtich_state),
                                  &default_turret_cw_limit_swtich_state,
                                  ap_mutexSemaphore);
                                  
    bool default_turret_ccw_limit_swtich_state = false;
    ap_turretCwLimitSwitchElement =
        new WsFacadeElement<bool>("TurretCcwLimitSwitchState",
                                  sizeof(default_turret_ccw_limit_swtich_state),
                                  &default_turret_ccw_limit_swtich_state,
                                  ap_mutexSemaphore);
                                  
    bool default_turret_button_state = false;
    ap_turretTurboButtonElement =
        new WsFacadeElement<bool>("TurretTurboButtonState",
                                  sizeof(default_turret_button_state),
                                  &default_turret_button_state,
                                  ap_mutexSemaphore);
                                  
    bool default_flood_light_state = false;
    ap_floodLightStateElement =
        new WsFacadeElement<bool>("FloodLightButtonState",
                                  sizeof(default_flood_light_state),
                                  &default_flood_light_state,
                                  ap_mutexSemaphore);
                                  
    bool default_fresh_image_state = false;
    ap_freshImageFlagElement =
        new WsFacadeElement<bool>("FreshImageFlag",
                                  sizeof(default_fresh_image_state),
                                  &default_fresh_image_state,
                                  ap_mutexSemaphore);
                                  
    float default_drive_throttle_value = 0.0;
    ap_driveThrottleElement =
        new WsFacadeElement<float>("DriveThrottle",
                                   sizeof(default_drive_throttle_value),
                                   &default_drive_throttle_value,
                                   ap_mutexSemaphore);
                                   
    float default_drive_heading_value = 0.0;
    ap_driveHeadingElement =
        new WsFacadeElement<float>("DriveHeading",
                                   sizeof(default_drive_heading_value),
                                   &default_drive_heading_value,
                                   ap_mutexSemaphore);
                                   
    bool default_antiturbo_button_state = false;
    ap_antiturboButtonElement =
        new WsFacadeElement<bool>("AntiturboButtonState",
                                  sizeof(default_antiturbo_button_state),
                                  &default_antiturbo_button_state,
                                  ap_mutexSemaphore);
                                  
    bool default_shifter_button_state = false;
    ap_shifterButtonElement =
        new WsFacadeElement<bool>("ShifterButtonState",
                                  sizeof(default_shifter_button_state),
                                  &default_shifter_button_state,
                                  ap_mutexSemaphore);
                                  
    bool default_landing_gear_button_state = false;
    ap_landingGearButtonElement =
        new WsFacadeElement<bool>("LandingGearButtonState",
                                  sizeof(default_landing_gear_button_state),
                                  &default_landing_gear_button_state,
                                  ap_mutexSemaphore);
                                  
    bool default_lock_in_switch_state = false;
    ap_lockInSwitchElement =
        new WsFacadeElement<bool>("LockInSwitchState",
                                  sizeof(default_lock_in_switch_state),
                                  &default_lock_in_switch_state,
                                  ap_mutexSemaphore);
                                  
    bool default_landing_gear_retraction_switch_state = false;
    ap_landingGearRetractionSwitchElement =
        new WsFacadeElement<bool>("LandingGearRetractionSwitchState",
                                  sizeof(default_landing_gear_retraction_switch_state),
                                  &default_landing_gear_retraction_switch_state,
                                  ap_mutexSemaphore);
                                  
    WsTypes::WsAutonomousProgramIdType default_autonomous_program_id =
        WsTypes::WS_AUTO_PROGRAM_SLEEPER;
    ap_autonomousProgramIdElement =
        new WsFacadeElement<WsTypes::WsAutonomousProgramIdType>("AutonomousProgramId",
                                                                sizeof(default_autonomous_program_id),
                                                                &default_autonomous_program_id,
                                                                ap_mutexSemaphore);
                                                                
    bool default_pogo_stick_button_value = false;
    ap_pogoStickButtonElement =
        new WsFacadeElement<bool>("PogoStickButtonState",
                                  sizeof(default_pogo_stick_button_value),
                                  &default_pogo_stick_button_value,
                                  ap_mutexSemaphore);
                                  
    int default_num_vis_targets = 0;
    ap_numVisibleTargetsElement =
        new WsFacadeElement<int>("NumVisibleTargets",
                                 sizeof(default_num_vis_targets),
                                 &default_num_vis_targets,
                                 ap_mutexSemaphore);
                                 
    float default_selector_value = 0.0f;
    ap_autoProgramSelectorElement =
        new WsFacadeElement<float>("AutoProgramSelector",
                                   sizeof(default_selector_value),
                                   &default_selector_value,
                                   ap_mutexSemaphore);
                                   
    WsTypes::WsShooterPresetType default_preset = WsTypes::WS_SHOOTER_PRESET_NONE;
    ap_shooterPresetElement =
        new WsFacadeElement<WsTypes::WsShooterPresetType>("ShooterPreset",
                                                          sizeof(default_preset),
                                                          &default_preset,
                                                          ap_mutexSemaphore);
                                                          
    INT32 default_encoder_value = 0;
    ap_leftDriveEncoderElement =
        new WsFacadeElement<INT32>("LeftDriveEncoder",
                                   sizeof(default_encoder_value),
                                   &default_encoder_value,
                                   ap_mutexSemaphore);
                                   
    ap_rightDriveEncoderElement =
        new WsFacadeElement<INT32>("RightDriveEncoder",
                                   sizeof(default_encoder_value),
                                   &default_encoder_value,
                                   ap_mutexSemaphore);
                                   
    float default_pneumatic_pressure_sensor_value = 0.0f;
    ap_pneumaticPressureReadingElement =
        new WsFacadeElement<float>("PneumaticPressureReading",
                                   sizeof(default_pneumatic_pressure_sensor_value),
                                   &default_pneumatic_pressure_sensor_value,
                                   ap_mutexSemaphore);
                                   
    bool default_use_acceleration_flag = false;
    ap_driveBaseAccelerationButtonElement =
        new WsFacadeElement<bool>("DriveBaseAccelerationButton",
                                  sizeof(default_use_acceleration_flag),
                                  &default_use_acceleration_flag,
                                  ap_mutexSemaphore);
                                  
    bool default_accumulator_motor_button_state = false;
    ap_accumulatorMotorButtonElement =
        new WsFacadeElement<bool>("AccumulatorMotorButton",
                                  sizeof(default_accumulator_motor_button_state),
                                  &default_accumulator_motor_button_state,
                                  ap_mutexSemaphore);
                                  
    float default_gyro_angle_value = 0.0f;
    ap_robotHeadingGyroAngleElement =
        new WsFacadeElement<float>("RobotHeadingGyroAngleElement",
                                   sizeof(default_gyro_angle_value),
                                   &default_gyro_angle_value,
                                   ap_mutexSemaphore);
                                   
    bool default_bridge_brake_state = false;
    ap_bridgeBrakeButtonElement =
        new WsFacadeElement<bool>("BridgeBrakeButtonState",
                                  sizeof(default_bridge_brake_state),
                                  &default_bridge_brake_state,
                                  ap_mutexSemaphore);
                                  
    bool default_speed_compensation_switch_value = false;
    ap_flywheelSpeedCompensationEnableSwitchElement =
        new WsFacadeElement<bool>("FlywheelSpeedCompensationEnableSwitch",
                                  sizeof(default_speed_compensation_switch_value),
                                  &default_speed_compensation_switch_value,
                                  ap_mutexSemaphore);
                                  
    float default_speed_compensation_pot_value = 0.0f;
    ap_flywheelSpeedCompensationPotElement =
        new WsFacadeElement<float>("FlywheelSpeedCompensationPot",
                                   sizeof(default_speed_compensation_pot_value),
                                   &default_speed_compensation_pot_value,
                                   ap_mutexSemaphore);
}

WsInputFacade::~WsInputFacade()
{
    // Tear down all data interface objects
    delete ap_turretHeadingPotentiometer;          ap_turretHeadingPotentiometer = NULL;
    delete ap_driverJoystick;                      ap_driverJoystick = NULL;
    delete ap_manipulatorJoystick;                 ap_manipulatorJoystick = NULL;
    delete ap_flywheelSpeedEncoder;                ap_flywheelSpeedEncoder = NULL;
    delete ap_pressureSwitchDigitalInput;          ap_pressureSwitchDigitalInput = NULL;
    delete ap_turretCwLimitSwitchDigitalInput;     ap_turretCwLimitSwitchDigitalInput = NULL;
    delete ap_turretCcwLimitSwitchDigitalInput;    ap_turretCcwLimitSwitchDigitalInput = NULL;
    delete ap_leftDriveEncoder;                    ap_leftDriveEncoder = NULL;
    delete ap_rightDriveEncoder;                   ap_rightDriveEncoder = NULL;
    delete ap_landingGearRetractionDigitalInput;   ap_landingGearRetractionDigitalInput = NULL;
    delete ap_pneumaticPressureTransducer;         ap_pneumaticPressureTransducer = NULL;
    delete ap_robotHeadingGyro;                    ap_robotHeadingGyro = NULL;
    
    // New WsSubject implementation
    delete ap_hoodButtonElement;                   ap_hoodButtonElement = NULL;
    delete ap_flywheelRotationalSpeedElement;      ap_flywheelRotationalSpeedElement = NULL;
    delete ap_accumulatorDeployButtonElement;      ap_accumulatorDeployButtonElement = NULL;
    delete ap_robotGrabberButtonElement;           ap_robotGrabberButtonElement = NULL;
    delete ap_shooterButtonElement;                ap_shooterButtonElement = NULL;
    delete ap_shooterOverrideButtonElement;        ap_shooterOverrideButtonElement = NULL;
    delete ap_escalatorUpButtonElement;            ap_escalatorUpButtonElement = NULL;
    delete ap_escalatorDownButtonElement;          ap_escalatorDownButtonElement = NULL;
    delete ap_flywheelStatePotValueElement;        ap_flywheelStatePotValueElement = NULL;
    delete ap_flywheelManualPotElement;            ap_flywheelManualPotElement = NULL;
    delete ap_turretRequestedDirectionElement;     ap_turretRequestedDirectionElement = NULL;
    delete ap_turretHeadingPotValueElement;        ap_turretHeadingPotValueElement = NULL;
    delete ap_turretCwLimitSwitchElement;          ap_turretCwLimitSwitchElement = NULL;
    delete ap_turretCcwLimitSwitchElement;         ap_turretCcwLimitSwitchElement = NULL;
    delete ap_turretTurboButtonElement;            ap_turretTurboButtonElement = NULL;
    delete ap_floodLightStateElement;              ap_floodLightStateElement = NULL;
    delete ap_freshImageFlagElement;               ap_freshImageFlagElement = NULL;
    delete ap_driveThrottleElement;                ap_driveThrottleElement = NULL;
    delete ap_driveHeadingElement;                 ap_driveHeadingElement = NULL;
    delete ap_antiturboButtonElement;              ap_antiturboButtonElement = NULL;
    delete ap_shifterButtonElement;                ap_shifterButtonElement = NULL;
    delete ap_landingGearButtonElement;            ap_landingGearButtonElement = NULL;
    delete ap_landingGearRetractionSwitchElement;  ap_landingGearRetractionSwitchElement = NULL;
    delete ap_lockInSwitchElement;                 ap_lockInSwitchElement = NULL;
    delete ap_autonomousProgramIdElement;          ap_autonomousProgramIdElement = NULL;
    delete ap_autoProgramSelectorElement;          ap_autoProgramSelectorElement = NULL;
    delete ap_pogoStickButtonElement;              ap_pogoStickButtonElement = NULL;
    delete ap_rightDriveEncoderElement;            ap_rightDriveEncoderElement = NULL;
    delete ap_leftDriveEncoderElement;             ap_leftDriveEncoderElement = NULL;
    delete ap_pneumaticPressureReadingElement;     ap_pneumaticPressureReadingElement = NULL;
    delete ap_driveBaseAccelerationButtonElement;  ap_driveBaseAccelerationButtonElement = NULL;
    delete ap_accumulatorMotorButtonElement;       ap_accumulatorMotorButtonElement = NULL;
    delete ap_robotHeadingGyroAngleElement;        ap_robotHeadingGyroAngleElement = NULL;
    delete ap_bridgeBrakeButtonElement;            ap_bridgeBrakeButtonElement = NULL;
    delete ap_flywheelSpeedCompensationEnableSwitchElement; ap_flywheelSpeedCompensationEnableSwitchElement = NULL;
    delete ap_flywheelSpeedCompensationPotElement; ap_flywheelSpeedCompensationPotElement = NULL;
    
    // Clear out our data
    ::memset(&a_sensorData, 0x00, sizeof(a_sensorData));
    
    // Let's clean up our mutex nicely
    semFlush(ap_mutexSemaphore);
    semDelete(ap_mutexSemaphore);
    ap_mutexSemaphore = NULL;
}

WsInputFacade*
WsInputFacade::instance()
{
    if (NULL == asp_instance)
    { asp_instance = new WsInputFacade(); }
    return asp_instance;
}

void
WsInputFacade::updateSensorData(void)
{
    //
    // Use temporary stack variables to hold the values returned from the
    //  hardware.  We don't want to interface with the hardware objects
    //  in the critical region because we aren't guaranteed that those
    //  operations execute quickly.
    //
    
    float turret_heading = ap_turretHeadingPotentiometer->GetVoltage();
    INT32 left_drive_encoder = ap_leftDriveEncoder->Get();
    INT32 right_drive_encoder = ap_rightDriveEncoder->Get();
    bool raw_pressure_switch_state = (0 == ap_pressureSwitchDigitalInput->Get());
    bool raw_turret_cw_limit_switch_state = false; // (0 == ap_turretCwLimitSwitchDigitalInput->Get());
    bool raw_turret_ccw_limit_switch_state = false; // (0 == ap_turretCcwLimitSwitchDigitalInput->Get());
    bool raw_landing_gear_retraction_switch_state = ! ap_landingGearRetractionDigitalInput->Get();
    float raw_pneumatic_pressure_reading = ap_pneumaticPressureTransducer->GetVoltage();
    float raw_gyro_angle = ap_robotHeadingGyro->GetAngle();
    
    // Calculate the turret heading in degrees
    float turret_heading_degrees = 0.0;
    if (turret_heading > turret_center_voltage)
    {
        turret_heading_degrees = (turret_heading *
                                  ((turret_cw_degrees - turret_center_degrees) /
                                   (turret_cw_voltage - turret_center_voltage))
                                  + turret_ccw_degrees);
    }
    else if (turret_heading < turret_center_voltage)
    {
        turret_heading_degrees = (turret_heading *
                                  ((turret_center_degrees - turret_ccw_degrees) /
                                   (turret_center_voltage - turret_ccw_voltage))
                                  + turret_ccw_degrees);
    }
    else
    {
        turret_heading_degrees = 0.0;
    }
    
    //-------------------------------------------------------------------------
    // Calculate Flywheel Rotational Speed
    //-------------------------------------------------------------------------
    
    // Get the instantaneous tick count from the encoder & the time
    WsFlywheelSnapshotType snapshot;
    snapshot.fpga_time = Timer::GetFPGATimestamp();
    snapshot.tick_count = ap_flywheelSpeedEncoder->Get();
    
    // Get other values from the encoder...
    WS_LOG_DEBUG("GET_PERIOD: %.09f", ap_flywheelSpeedEncoder->GetPeriod());
    
    // Add the current snapshot to the list
    a_flywheelSnapshotList.push_back(snapshot);
    
    // Make sure the list is of expected size
    // @TODO:  Make this size at least a #define
    while (a_flywheelSnapshotList.size() > WS_FLYWHEEL_TICK_LIST_DEPTH)
    { a_flywheelSnapshotList.pop_front(); }
    
    // Loop through the list of ticks & times, and calculate the average speed
    float flywheel_speed = 0.0f;
    if (a_flywheelSnapshotList.size() > 1)
    {
        INT32 newest_tick_count = a_flywheelSnapshotList.back().tick_count;
        INT32 oldest_tick_count = a_flywheelSnapshotList.front().tick_count;
        double newest_timestamp = a_flywheelSnapshotList.back().fpga_time;
        double oldest_timestamp = a_flywheelSnapshotList.front().fpga_time;
        double numerator        = fabs(newest_tick_count - oldest_tick_count);
        double denominator      = fabs(newest_timestamp - oldest_timestamp);
        
        WS_LOG_DEBUG("(%d - %d) / (%f - %f) Deltas: %f / %f",
                     newest_tick_count,
                     oldest_tick_count,
                     newest_timestamp,
                     oldest_timestamp,
                     numerator,
                     denominator);
                     
        // Don't divide by zero (floating point gets close)
        if (denominator > 0.0001)
        {
            // Average the speeds & apply our conversion factor (currently,
            double ticks_per_second = numerator / denominator;
            double rpm_intermediate_axle = ticks_per_second * (60.0f / 128.0f);
            double rpm_flywheel_axle = rpm_intermediate_axle * (18.0f / 35.0f);
            flywheel_speed = (float)rpm_flywheel_axle;
            WS_LOG_DEBUG("CALC: %f, %f, %f", ticks_per_second, rpm_intermediate_axle, flywheel_speed);
        }
    }
    
    // New WsSubject/Observer implementation
    this->setFlywheelRotationalSpeed(flywheel_speed);
    this->setPressureSwitchState(raw_pressure_switch_state);
    this->setTurretCwLimitSwitchState(raw_turret_cw_limit_switch_state);
    this->setTurretCcwLimitSwitchState(raw_turret_ccw_limit_switch_state);
    this->setLandingGearRetractionSwitch(raw_landing_gear_retraction_switch_state);
    this->setTurretHeadingPotValue(turret_heading);
    this->setLeftDriveEncoderCount(left_drive_encoder);
    this->setRightDriveEncoderCount(right_drive_encoder);
    this->setPneumaticPressureReading(raw_pneumatic_pressure_reading);
    this->setRobotHeadingGyroAngle(raw_gyro_angle);
    
    WS_LOG_INFO("if_turretHeading = %.05f", (double)turret_heading);
    WS_LOG_INFO("if_turretHeadingDegrees = %.05f", (double)turret_heading_degrees);
    WS_LOG_INFO("if_pressureSwitchState = %d", raw_pressure_switch_state);
    WS_LOG_INFO("if_cwLimitSwitch = %d", raw_turret_cw_limit_switch_state);
    WS_LOG_INFO("if_ccwLimitSwitch = %d", raw_turret_ccw_limit_switch_state);
    WS_LOG_INFO("if_landGearRetractSwitch = %d", raw_landing_gear_retraction_switch_state);
    WS_LOG_INFO("if_flywheelRotationalSpeed = %.05f", flywheel_speed);
    WS_LOG_INFO("if_leftEncoderCount = %d", left_drive_encoder);
    WS_LOG_INFO("if_rightEncoderCount = %d", right_drive_encoder);
    WS_LOG_INFO("if_gyroAngle = %d", raw_gyro_angle);
}

void
WsInputFacade::updateOiData(void)
{
    //
    // Use temporary stack variables to hold the values returned from the
    //  hardware.  We don't want to interface with the hardware objects
    //  in the critical region because we aren't guaranteed that those
    //  operations execute quickly.
    //
    bool raw_shifter_button = ap_driverJoystick->GetRawButton(6);
    bool raw_antiturbo_button = ap_driverJoystick->GetRawButton(7);
    bool raw_accumulator_deploy_button = ap_driverJoystick->GetRawButton(8);
    bool raw_accumulator_motor_button = ap_driverJoystick->GetRawButton(5);
    bool raw_landing_gear_button = false;
    bool raw_robot_grabber_button = ap_driverJoystick->GetRawButton(1);
    bool raw_escalator_up_button_state = ap_manipulatorJoystick->GetRawButton(4);
    bool raw_escalator_down_button_state = ap_manipulatorJoystick->GetRawButton(2);
    bool raw_turret_turbo_button = ap_manipulatorJoystick->GetRawButton(1);
    bool raw_shooter_override_button_state = ap_manipulatorJoystick->GetRawButton(6);
    bool raw_shooter_button_state = ap_manipulatorJoystick->GetRawButton(8);
    bool raw_drive_base_accel_state = ap_driverJoystick->GetRawButton(9);
    bool raw_hood_button_state = ap_manipulatorJoystick->GetRawButton(3);
    bool raw_pogo_stick_button_state = ap_driverJoystick->GetRawButton(3);
    bool raw_bridge_brake_button_state = ap_driverJoystick->GetRawButton(2);
    
    // kXAxis, kYAxis, kZAxis, kTwistAxis, kThrottleAxis, kNumAxisTypes
    float drive_throttle = -1.0f * ap_driverJoystick->GetAxis(Joystick::kYAxis);
    float drive_heading  = ap_driverJoystick->GetAxis(Joystick::kZAxis);
    
    
    //
    // Interpret the inputs from the D-Pad on the manipulator's gamepad as 1 of 4
    //  preset shooting positions as follows:
    //
    //       +---+         1 == Shooting from the key, facing the baskets
    //       | 1 |
    //   +---+   +---+     2 == Shooting from the fender, robot on right side
    //   | 4       2 |
    //   +---+   +---+     3 == Shooting from the key, facing away from the baskets
    //       | 3 |
    //       +---+         4 == Shooting from the fender, robot on left side
    //
    
    // Positive values should come from the dpad up -> Need to invert incoming value
    float raw_d_pad_up_down      = ap_manipulatorJoystick->GetAxis(Joystick::kThrottleAxis) * -1.0f;
    float raw_d_pad_left_right   = ap_manipulatorJoystick->GetAxis(Joystick::kTwistAxis) * -1.0f;
    bool left_right_axis_pressed = (fabs(raw_d_pad_left_right) > 0.5f);
    bool up_down_axis_pressed    = (fabs(raw_d_pad_up_down) > 0.5f);
    
    WsTypes::WsShooterPresetType shooter_preset = WsTypes::WS_SHOOTER_PRESET_NONE;
    if ((true == left_right_axis_pressed) &&
        (false == up_down_axis_pressed))
    {
        // Only the left/right axis is being actuated
        shooter_preset = (raw_d_pad_left_right > 0.5f)
                         ? WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT
                         : WsTypes::WS_SHOOTER_PRESET_FENDER_RIGHT;
    }
    else if ((false == left_right_axis_pressed) &&
             (true == up_down_axis_pressed))
    {
        // Only the up/down axis is being actuated
        shooter_preset = (raw_d_pad_up_down > 0.5f)
                         ? WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD
                         : WsTypes::WS_SHOOTER_PRESET_KEY_BACK;
    }
    else
    {
        // We have a palm-smash on the D-pad, this is invalid input
        shooter_preset = WsTypes::WS_SHOOTER_PRESET_NONE;
    }
    
    
    //
    // Depending on the analog voltage level read in, set the requested flywheel
    //  state in this facade.
    // @TODO:  Make this work like the turret manual direction... just store the
    //         raw value in the facade and let the button make the decisions about what to do.
    //         Delete the enum in this class!
    //
    float flywheel_switch = DriverStation::GetInstance()->GetAnalogIn(3);
    float raw_flywheel_manual_pot = DriverStation::GetInstance()->GetAnalogIn(1);
    float auto_program_selector = DriverStation::GetInstance()->GetAnalogIn(2);
    // float raw_flywheel_speed_compensation_pot_value = DriverStation::GetInstance()->GetAnalogIn(4);
    
    // Get joystick deflection and store
    float requested_turret_direction = ap_manipulatorJoystick->GetAxis(Joystick::kXAxis);
    
    bool flood_light = DriverStation::GetInstance()->GetDigitalIn(2);
    
    bool raw_lock_in_switch = DriverStation::GetInstance()->GetDigitalIn(1);
    // bool raw_flywheel_speed_compensation_enable_switch_state = DriverStation::GetInstance()->GetDigitalIn(3);
    
    // New WsSubject/Observer implementation
    
    this->setHoodButtonState(raw_hood_button_state);
    this->setAccumulatorDeployButtonState(raw_accumulator_deploy_button);
    this->setAccumulatorMotorButtonState(raw_accumulator_motor_button);
    this->setRobotGrabberButtonState(raw_robot_grabber_button);
    this->setEscalatorUpButtonState(raw_escalator_up_button_state);
    this->setEscalatorDownButtonState(raw_escalator_down_button_state);
    this->setBallShootButtonState(raw_shooter_button_state);
    this->setBallShootOverrideButtonState(raw_shooter_override_button_state);
    this->setFlywheelStatePotValue(flywheel_switch);
    this->setFlywheelManualPot(raw_flywheel_manual_pot);
    this->setFloodLightButtonState(flood_light);
    this->setRequestedTurretDirection(requested_turret_direction);
    this->setTurretTurboButtonState(raw_turret_turbo_button);
    this->setDriverThrottleValue(drive_throttle);
    this->setDriverHeadingValue(drive_heading);
    this->setAntiTurboButtonState(raw_antiturbo_button);
    this->setDriveBaseAccelerationButtonState(raw_drive_base_accel_state);
    this->setShifterButtonState(raw_shifter_button);
    this->setLandingGearButtonState(raw_landing_gear_button);
    this->setLockInSwitchState(raw_lock_in_switch);
    this->setAutoProgramSelectorValue(auto_program_selector);
    this->setPogoStickDeploymentButtonState(raw_pogo_stick_button_state);
    this->setBridgeBrakeActivationButtonState(raw_bridge_brake_button_state);
    
    // This is a temporary workaround to use the available hardware on the OI
    this->setFlywheelSpeedCompensationEnableSwitchState(flood_light);
    this->setFlywheelSpeedCompensationPotValue(auto_program_selector);
    
    //
    // If we are in the process of shooting, we must ignore future turret
    //  position changes - so don't update the preset value.
    //
    if ((false == raw_shooter_override_button_state) &&
        (false == raw_shooter_button_state))
    {
        this->setShooterPreset(shooter_preset);
    }
    
    WS_LOG_INFO("if_driveThrottle = %.05f", (double)drive_throttle);
    WS_LOG_INFO("if_driveHeading = %.05f", (double)drive_heading);
    WS_LOG_INFO("if_requestedTurretDirection = %.05f", (double)requested_turret_direction);
    WS_LOG_INFO("if_flywheelStatePot = %.4f", flywheel_switch);
    WS_LOG_INFO("if_shifterButtonState = %d", raw_shifter_button);
    WS_LOG_INFO("if_antiTurboButtonState = %d", raw_antiturbo_button);
    WS_LOG_INFO("if_driveBaseAccelerationButtonState = %d", raw_drive_base_accel_state);
    WS_LOG_INFO("if_accumulatorDeployButtonState = %d", raw_accumulator_deploy_button);
    WS_LOG_INFO("if_accumulatorMotorButtonState = %d", raw_accumulator_motor_button);
    WS_LOG_INFO("if_landingGearButtonState = %d", raw_landing_gear_button);
    WS_LOG_INFO("if_floodlightState = %d", flood_light);
    WS_LOG_INFO("if_escalatorUpButtonState = %d", raw_escalator_up_button_state);
    WS_LOG_INFO("if_escalatorDownButtonState = %d", raw_escalator_down_button_state);
    WS_LOG_INFO("if_escalatorBallShootButtonState = %d", raw_shooter_button_state);
    WS_LOG_INFO("if_robotGrabberState = %d", raw_robot_grabber_button);
    WS_LOG_INFO("if_flywheelManualPot = %.05f", (double)raw_flywheel_manual_pot);
    WS_LOG_INFO("if_turretTurboButtonState = %d", raw_turret_turbo_button);
    WS_LOG_INFO("if_dPadUpDownValue = %f", (double)raw_d_pad_up_down);
    WS_LOG_INFO("if_rawHoodButtonState = %d", raw_hood_button_state);
    WS_LOG_INFO("if_pogoStickButtonState = %d", raw_pogo_stick_button_state);
}

WsSubject*
WsInputFacade::getDriverThrottleValue(float& r_throttle)
{
    ap_driveThrottleElement->getState(r_throttle);
    return ap_driveThrottleElement;
}

void
WsInputFacade::setDriverThrottleValue(const float& rc_throttle)
{
    ap_driveThrottleElement->setState(rc_throttle);
}

WsSubject*
WsInputFacade::getDriverHeadingValue(float& r_heading)
{
    ap_driveHeadingElement->getState(r_heading);
    return ap_driveHeadingElement;
}

void
WsInputFacade::setDriverHeadingValue(const float& rc_heading)
{
    ap_driveHeadingElement->setState(rc_heading);
}

WsSubject*
WsInputFacade::getFloodLightButtonState(bool& r_state)
{
    ap_floodLightStateElement->getState(r_state);
    return ap_floodLightStateElement;
}

void
WsInputFacade::setFloodLightButtonState(const bool& rc_state)
{
    ap_floodLightStateElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getFreshImageFlag(bool& r_flag)
{
    ap_freshImageFlagElement->getState(r_flag);
    return ap_freshImageFlagElement;
}

void
WsInputFacade::setFreshImageFlag(const bool& rc_flag)
{
    ap_freshImageFlagElement->setState(rc_flag);
}

WsSubject*
WsInputFacade::getTurretHeadingPotValue(float& r_heading)
{
    ap_turretHeadingPotValueElement->getState(r_heading);
    return ap_turretHeadingPotValueElement;
}

void
WsInputFacade::setTurretHeadingPotValue(const float& rc_heading)
{
    ap_turretHeadingPotValueElement->setState(rc_heading);
}

WsSubject*
WsInputFacade::getRequestedTurretDirection(float& r_direction)
{
    ap_turretRequestedDirectionElement->getState(r_direction);
    return ap_turretRequestedDirectionElement;
}

void
WsInputFacade::setRequestedTurretDirection(const float& rc_direction)
{
    ap_turretRequestedDirectionElement->setState(rc_direction);
}

WsSubject*
WsInputFacade::getShifterButtonState(bool& r_state)
{
    ap_shifterButtonElement->getState(r_state);
    return ap_shifterButtonElement;
}

void
WsInputFacade::setShifterButtonState(const bool& rc_state)
{
    ap_shifterButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getAntiTurboButtonState(bool& r_state)
{
    ap_antiturboButtonElement->getState(r_state);
    return ap_antiturboButtonElement;
}

void
WsInputFacade::setAntiTurboButtonState(const bool& rc_state)
{
    ap_antiturboButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getAccumulatorDeployButtonState(bool& r_state)
{
    ap_accumulatorDeployButtonElement->getState(r_state);
    return ap_accumulatorDeployButtonElement;
}

void
WsInputFacade::setAccumulatorDeployButtonState(const bool& rc_state)
{
    ap_accumulatorDeployButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getLandingGearButtonState(bool& r_state)
{
    ap_landingGearButtonElement->getState(r_state);
    return ap_landingGearButtonElement;
}

void
WsInputFacade::setLandingGearButtonState(const bool& rc_state)
{
    ap_landingGearButtonElement->setState(rc_state);
    
}

WsSubject*
WsInputFacade::getHoodButtonState(bool& r_state)
{
    ap_hoodButtonElement->getState(r_state);
    return ap_hoodButtonElement;
}

void
WsInputFacade::setHoodButtonState(const bool& rc_state)
{
    ap_hoodButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getFlywheelStatePotValue(float& r_value)
{
    ap_flywheelStatePotValueElement->getState(r_value);
    return ap_flywheelStatePotValueElement;
}

void
WsInputFacade::setFlywheelStatePotValue(const float& rc_value)
{
    ap_flywheelStatePotValueElement->setState(rc_value);
}

WsSubject*
WsInputFacade::getPressureSwitchState(bool& r_state)
{
    ap_pressureSwitchElement->getState(r_state);
    return ap_pressureSwitchElement;
    
}

void
WsInputFacade::setPressureSwitchState(const bool& rc_state)
{
    ap_pressureSwitchElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getTurretCwLimitSwitchState(bool& r_state)
{
    ap_turretCwLimitSwitchElement->getState(r_state);
    return ap_turretCwLimitSwitchElement;
}

void
WsInputFacade::setTurretCwLimitSwitchState(const bool& rc_state)
{
    ap_turretCwLimitSwitchElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getTurretCcwLimitSwitchState(bool& r_state)
{
    ap_turretCcwLimitSwitchElement->getState(r_state);
    return ap_turretCcwLimitSwitchElement;
}

void
WsInputFacade::setTurretCcwLimitSwitchState(const bool& rc_state)
{
    ap_turretCcwLimitSwitchElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getLeftDriveEncoderCount(INT32& r_count)
{
    ap_leftDriveEncoderElement->getState(r_count);
    return ap_leftDriveEncoderElement;
}

void
WsInputFacade::setLeftDriveEncoderCount(const INT32& rc_count)
{
    ap_leftDriveEncoderElement->setState(rc_count);
}

WsSubject*
WsInputFacade::getRightDriveEncoderCount(INT32& r_count)
{
    ap_rightDriveEncoderElement->getState(r_count);
    return ap_rightDriveEncoderElement;
}

void
WsInputFacade::setRightDriveEncoderCount(const INT32& rc_count)
{
    ap_rightDriveEncoderElement->setState(rc_count);
}

void
WsInputFacade::resetDriveEncoderCounts(void)
{
    INT32 zero = 0;
    
    this->setRightDriveEncoderCount(zero);
    this->setLeftDriveEncoderCount(zero);
    
    ap_leftDriveEncoder->Reset();
    ap_rightDriveEncoder->Reset();
}

WsSubject*
WsInputFacade::getEscalatorDownButtonState(bool& r_state)
{
    ap_escalatorDownButtonElement->getState(r_state);
    return ap_escalatorDownButtonElement;
}

void
WsInputFacade::setEscalatorDownButtonState(const bool& rc_state)
{
    ap_escalatorDownButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getEscalatorUpButtonState(bool& r_state)
{
    ap_escalatorUpButtonElement->getState(r_state);
    return ap_escalatorUpButtonElement;
}

void
WsInputFacade::setEscalatorUpButtonState(const bool& rc_state)
{
    ap_escalatorUpButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getBallShootButtonState(bool& r_state)
{
    ap_shooterButtonElement->getState(r_state);
    return ap_shooterButtonElement;
}

void
WsInputFacade::setBallShootButtonState(const bool& rc_state)
{
    ap_shooterButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getBallShootOverrideButtonState(bool& r_state)
{
    ap_shooterOverrideButtonElement->getState(r_state);
    return ap_shooterOverrideButtonElement;
}

void
WsInputFacade::setBallShootOverrideButtonState(const bool& rc_state)
{
    ap_shooterOverrideButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getRobotGrabberButtonState(bool& r_state)
{
    ap_robotGrabberButtonElement->getState(r_state);
    return ap_robotGrabberButtonElement;
}

void
WsInputFacade::setRobotGrabberButtonState(const bool& rc_state)
{
    ap_robotGrabberButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getLandingGearRetractionSwitch(bool& r_state)
{
    ap_landingGearRetractionSwitchElement->getState(r_state);
    return ap_landingGearRetractionSwitchElement;
    
}
void
WsInputFacade::setLandingGearRetractionSwitch(const bool& rc_state)
{
    ap_landingGearRetractionSwitchElement->setState(rc_state);
    
}

WsSubject*
WsInputFacade::getPogoStickDeploymentButtonState(bool& r_state)
{
    ap_pogoStickButtonElement->getState(r_state);
    return ap_pogoStickButtonElement;
}

void
WsInputFacade::setPogoStickDeploymentButtonState(const bool& rc_state)
{
    ap_pogoStickButtonElement->setState(rc_state);
}

void
WsInputFacade::setTargetImageInfo(const WsTypes::WsTargetBasketIdxType& rc_target_idx,
                                  WsTypes::WsTargetImageInfoType& rc_info)
{
    CRITICAL_REGION(ap_mutexSemaphore)
    {
        if ((WS_FIRST_TARGET_BASKET_IDX <= rc_target_idx) &&
            (WS_LAST_TARGET_BASKET_IDX >= rc_target_idx))
        {
            a_sensorData.TargetImageInfo[rc_target_idx] = rc_info;
        }
    }
    END_REGION
}


typedef std::pair < WsTypes::WsTargetBasketIdxType,
        WsTypes::WsTargetImageInfoType > WsImgIdxInfoPairType;

// Returns true if first should go before second, false otherwise
bool
compare_area(WsImgIdxInfoPairType a, WsImgIdxInfoPairType b)
{
    return (a.second.target_area > b.second.target_area);
}

void
WsInputFacade::getTargetImageIndecesByRank(WsTypes::WsTargetBasketIdxType& r_rank_one_idx,
                                           WsTypes::WsTargetBasketIdxType& r_rank_two_idx,
                                           WsTypes::WsTargetBasketIdxType& r_rank_three_idx,
                                           WsTypes::WsTargetBasketIdxType& r_rank_four_idx,
                                           WsTypes::WsTargetRankingCriteriaType rc_criteria)
{
    //
    // Based on the criteria, rank the image info structures and populate the
    // rank 1/2/3/4 indeces based on the sorting
    //
    WsTypes::WsTargetImageInfoType img_info_array[4];
    CRITICAL_REGION(ap_mutexSemaphore)
    {
        ::memcpy(img_info_array, a_sensorData.TargetImageInfo, sizeof(img_info_array));
    }
    END_REGION
    
    // Convert to an STL-list
    std::list<WsImgIdxInfoPairType> img_list;
    img_list.clear();
    
    // Populate the list
    WsImgIdxInfoPairType pair;
    pair.first = WsTypes::WS_TOP_BASKET_INDEX;
    pair.second = img_info_array[WsTypes::WS_TOP_BASKET_INDEX];
    img_list.push_back(pair);
    
    pair.first = WsTypes::WS_LEFT_BASKET_INDEX;
    pair.second = img_info_array[WsTypes::WS_LEFT_BASKET_INDEX];
    img_list.push_back(pair);
    
    pair.first = WsTypes::WS_RIGHT_BASKET_INDEX;
    pair.second = img_info_array[WsTypes::WS_RIGHT_BASKET_INDEX];
    img_list.push_back(pair);
    
    pair.first = WsTypes::WS_BOTTOM_BASKET_INDEX;
    pair.second = img_info_array[WsTypes::WS_BOTTOM_BASKET_INDEX];
    img_list.push_back(pair);
    
    // Sort the list depending on the ranking criteria
    switch (rc_criteria)
    {
        case WsTypes::WS_RANKING_CRITERIA_PARTICLE_AREA:
        {
            img_list.sort(compare_area);
            
            std::list<WsImgIdxInfoPairType>::iterator iter = img_list.begin();
            r_rank_one_idx = (true == (*iter).second.has_target)
                             ? (*iter).first
                             : WsTypes::WS_BASKET_INDEX_INVALID;
            iter++;
            
            r_rank_two_idx = (true == (*iter).second.has_target)
                             ? (*iter).first
                             : WsTypes::WS_BASKET_INDEX_INVALID;
            iter++;
            
            r_rank_three_idx = (true == (*iter).second.has_target)
                               ? (*iter).first
                               : WsTypes::WS_BASKET_INDEX_INVALID;
            iter++;
            
            r_rank_four_idx = (true == (*iter).second.has_target)
                              ? (*iter).first
                              : WsTypes::WS_BASKET_INDEX_INVALID;
            iter++;
            break;
        }
        
        default:
        {
            r_rank_one_idx = WsTypes::WS_BASKET_INDEX_INVALID;
            r_rank_two_idx = WsTypes::WS_BASKET_INDEX_INVALID;
            r_rank_three_idx = WsTypes::WS_BASKET_INDEX_INVALID;
            r_rank_four_idx = WsTypes::WS_BASKET_INDEX_INVALID;
            WS_LOG_ERROR("Unsupported ranking criteria: %d", rc_criteria);
            break;
        }
    }
    
    // All calculations done above here
    return;
}

void
WsInputFacade::getTargetImageInfo(const WsTypes::WsTargetBasketIdxType& rc_target_idx,
                                  WsTypes::WsTargetImageInfoType& r_info)
{
    CRITICAL_REGION(ap_mutexSemaphore)
    {
        if ((WS_FIRST_TARGET_BASKET_IDX <= rc_target_idx) &&
            (WS_LAST_TARGET_BASKET_IDX >= rc_target_idx))
        {
            r_info = a_sensorData.TargetImageInfo[rc_target_idx];
        }
    }
    END_REGION
}

WsSubject*
WsInputFacade::getNumVisibleTargets(int& r_numTargets)
{
    ap_numVisibleTargetsElement->getState(r_numTargets);
    return ap_numVisibleTargetsElement;
}

void
WsInputFacade::setNumVisibleTargets(const int& rc_numTargets)
{
    ap_numVisibleTargetsElement->setState(rc_numTargets);
}

WsSubject*
WsInputFacade::getFlywheelManualPot(float& r_value)
{
    ap_flywheelManualPotElement->getState(r_value);
    return ap_flywheelManualPotElement;
}

void
WsInputFacade::setFlywheelManualPot(const float& rc_value)
{
    ap_flywheelManualPotElement->setState(rc_value);
}

WsSubject*
WsInputFacade::getFlywheelRotationalSpeed(float& r_speed)
{
    ap_flywheelRotationalSpeedElement->getState(r_speed);
    return ap_flywheelRotationalSpeedElement;
}

void
WsInputFacade::setFlywheelRotationalSpeed(const float& rc_speed)
{
    ap_flywheelRotationalSpeedElement->setState(rc_speed);
}

WsSubject*
WsInputFacade::getTurretTurboButtonState(bool& r_state)
{
    ap_turretTurboButtonElement->getState(r_state);
    return ap_turretTurboButtonElement;
}

void
WsInputFacade::setTurretTurboButtonState(const bool& rc_state)
{
    ap_turretTurboButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getLockInSwitchState(bool& r_state)
{
    ap_lockInSwitchElement->getState(r_state);
    return ap_lockInSwitchElement;
}

void
WsInputFacade::setLockInSwitchState(const bool& rc_state)
{
    ap_lockInSwitchElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getAutonomousProgramId(WsTypes::WsAutonomousProgramIdType& r_id)
{
    ap_autonomousProgramIdElement->getState(r_id);
    return ap_autonomousProgramIdElement;
}

void
WsInputFacade::setAutonomousProgramId(const WsTypes::WsAutonomousProgramIdType& rc_id)
{
    ap_autonomousProgramIdElement->setState(rc_id);
}

WsSubject*
WsInputFacade::getAutoProgramSelectorValue(float& r_value)
{
    ap_autoProgramSelectorElement->getState(r_value);
    return ap_autoProgramSelectorElement;
}

void
WsInputFacade::setAutoProgramSelectorValue(const float& rc_value)
{
    ap_autoProgramSelectorElement->setState(rc_value);
}

WsSubject*
WsInputFacade::getShooterPreset(WsTypes::WsShooterPresetType& r_value)
{
    ap_shooterPresetElement->getState(r_value);
    return ap_shooterPresetElement;
}

void
WsInputFacade::setShooterPreset(const WsTypes::WsShooterPresetType& rc_value)
{
    ap_shooterPresetElement->setState(rc_value);
}

WsSubject*
WsInputFacade::getPneumaticPressureReading(float& r_value)
{
    ap_pneumaticPressureReadingElement->getState(r_value);
    return ap_pneumaticPressureReadingElement;
}

void
WsInputFacade::setPneumaticPressureReading(const float& rc_value)
{
    ap_pneumaticPressureReadingElement->setState(rc_value);
}

WsSubject*
WsInputFacade::getDriveBaseAccelerationButtonState(bool& r_value)
{
    ap_driveBaseAccelerationButtonElement->getState(r_value);
    return ap_driveBaseAccelerationButtonElement;
}

void
WsInputFacade::setDriveBaseAccelerationButtonState(const bool& rc_value)
{
    ap_driveBaseAccelerationButtonElement->setState(rc_value);
}

WsSubject*
WsInputFacade::getAccumulatorMotorButtonState(bool& r_state)
{
    ap_accumulatorMotorButtonElement->getState(r_state);
    return ap_accumulatorMotorButtonElement;
}

void
WsInputFacade::setAccumulatorMotorButtonState(const bool& rc_state)
{
    ap_accumulatorMotorButtonElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getRobotHeadingGyroAngle(float& r_value)
{
    ap_robotHeadingGyroAngleElement->getState(r_value);
    return ap_robotHeadingGyroAngleElement;
}

void
WsInputFacade::setRobotHeadingGyroAngle(const float& rc_value)
{
    ap_robotHeadingGyroAngleElement->setState(rc_value);
}

WsSubject*
WsInputFacade::getFlywheelSpeedCompensationPotValue(float& r_value)
{
    ap_flywheelSpeedCompensationPotElement->getState(r_value);
    return ap_flywheelSpeedCompensationPotElement;
}

void
WsInputFacade::setFlywheelSpeedCompensationPotValue(const float& rc_value)
{
    ap_flywheelSpeedCompensationPotElement->setState(rc_value);
}

WsSubject*
WsInputFacade::getFlywheelSpeedCompensationEnableSwitchState(bool& r_state)
{
    ap_flywheelSpeedCompensationEnableSwitchElement->getState(r_state);
    return ap_flywheelSpeedCompensationEnableSwitchElement;
}

void
WsInputFacade::setFlywheelSpeedCompensationEnableSwitchState(const bool& rc_state)
{
    ap_flywheelSpeedCompensationEnableSwitchElement->setState(rc_state);
}

WsSubject*
WsInputFacade::getBridgeBrakeActivationButtonState(bool& r_state)
{
    ap_bridgeBrakeButtonElement->getState(r_state);
    return ap_bridgeBrakeButtonElement;
}

void
WsInputFacade::setBridgeBrakeActivationButtonState(const bool& rc_state)
{
    ap_bridgeBrakeButtonElement->setState(rc_state);
}


//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

