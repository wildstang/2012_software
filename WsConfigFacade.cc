#include "WsConfigFacade.hh"
#include "WsSubject.hh"
#include "Synchronized.h"
#include "WsTypes.hh"
#include "WsLogger.hh"

#include <sstream>

WsConfigFacade* WsConfigFacade::asp_instance = NULL;
WsConfigFacade* WsConfigFacade::instance(void)
{
    if (NULL == asp_instance)
    { asp_instance = new WsConfigFacade(); }
    return asp_instance;
}

WsConfigFacade::WsConfigFacade(void)
    : ap_turretPidConfigElement(NULL)
    , ap_flywheelPidConfigElement(NULL)
    , ap_flywheelLocationSpeedMappingElement(NULL)
    , ap_autoShoot6ConfigElement(NULL)
    , ap_cameraProcessingImaqData(NULL)
    , ap_ballTransportMotorValsElement(NULL)
    , ap_autoTipShootConfigElement(NULL)
    , ap_autoShoot3ConfigElement(NULL)
    , ap_autoShootTipConfigElement(NULL)
    , ap_shooterPresetConfigElement(NULL)
    , ap_autoDriveProgramConfigElement(NULL)
    , ap_autoFenderRightProgramConfigElement(NULL)
    , ap_autoFenderLeftProgramConfigElement(NULL)
    , a_configurationDatabase()
{
    ap_mutexSemaphore = semMCreate(SEM_Q_PRIORITY |
                                   SEM_DELETE_SAFE |
                                   SEM_INVERSION_SAFE);
                                   
    //
    // Create the database with default values for all expected parameters.
    //
    a_configurationDatabase.clear();
    
    // Logger Config
    a_configurationDatabase["ws.logger.fatal"]   = "true";
    a_configurationDatabase["ws.logger.error"]   = "true";
    a_configurationDatabase["ws.logger.warning"] = "true";
    a_configurationDatabase["ws.logger.notice"]  = "false";
    a_configurationDatabase["ws.logger.info"]    = "false";
    a_configurationDatabase["ws.logger.debug"]   = "false";
    a_configurationDatabase["ws.logger.trace"]   = "false";
    
    // Turret PID Config
    a_configurationDatabase["ws.turret.pid.kp"]                        = "8.0";
    a_configurationDatabase["ws.turret.pid.ki"]                        = "1.0";
    a_configurationDatabase["ws.turret.pid.kd"]                        = "0.0";
    a_configurationDatabase["ws.turret.pid.error.epsilon"]             = "0.05";
    a_configurationDatabase["ws.turret.pid.static.epsilon"]            = "0.05";
    a_configurationDatabase["ws.turret.pid.max.integral"]              = "0.60";
    a_configurationDatabase["ws.turret.pid.error.increment"]           = "0.05";
    a_configurationDatabase["ws.turret.pid.integral.error.threshold"]  = "2.5";
    a_configurationDatabase["ws.turret.pid.differentiator.band.limit"] = "0.1";
    a_configurationDatabase["ws.turret.pid.max.output"]                = "1.0";
    a_configurationDatabase["ws.turret.pid.min.output"]                = "-1.0";
    a_configurationDatabase["ws.turret.pid.max.input"]                 = "5.0";
    a_configurationDatabase["ws.turret.pid.min.input"]                 = "0.0";
    a_configurationDatabase["ws.turret.pid.stabilization.time"]        = "0.2";
    
    // Flywheel PID Config
    a_configurationDatabase["ws.flywheel.pid.kp"]                        = "0.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.ki"]                        = "0.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.kd"]                        = "0.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.error.epsilon"]             = "0.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.static.epsilon"]            = "0.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.max.integral"]              = "0.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.error.increment"]           = "0.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.integral.error.threshold"]  = "0.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.differentiator.band.limit"] = "0.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.max.output"]                = "1.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.min.output"]                = "-1.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.max.input"]                 = "5000.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.min.input"]                 = "0.0";  // @TODO:  Get a good default
    a_configurationDatabase["ws.flywheel.pid.stabilization.time"]        = "0.2";  // @TODO:  Get a good default
    
    // Ball Transport Config
    a_configurationDatabase["ws.ball.transport.elevator.off"]     = "0.0";
    a_configurationDatabase["ws.ball.transport.elevator.up"]      = "0.4";
    a_configurationDatabase["ws.ball.transport.elevator.down"]    = "-0.4";
    a_configurationDatabase["ws.ball.transport.escalator.off"]    = "0.0";
    a_configurationDatabase["ws.ball.transport.escalator.up"]     = "-0.75";
    a_configurationDatabase["ws.ball.transport.escalator.down"]   = "0.75";
    a_configurationDatabase["ws.ball.transport.accumulator.off"]  = "0.0";
    a_configurationDatabase["ws.ball.transport.accumulator.pull"] = "1.0";
    a_configurationDatabase["ws.ball.transport.accumulator.push"] = "-1.0";
    
    // Flywheel Location Speed Mapping Config
    a_configurationDatabase["ws.flywheel.speed.mapping.fender.distance"]     =  "45.0";
    a_configurationDatabase["ws.flywheel.speed.mapping.fender.speed"]        = "110.0";
    a_configurationDatabase["ws.flywheel.speed.mapping.key.bottom.distance"] = "116.0";
    a_configurationDatabase["ws.flywheel.speed.mapping.key.bottom.speed"]    = "170.0";
    a_configurationDatabase["ws.flywheel.speed.mapping.key.middle.distance"] = "153.0";
    a_configurationDatabase["ws.flywheel.speed.mapping.key.middle.speed"]    = "190.0";
    a_configurationDatabase["ws.flywheel.speed.mapping.key.top.distance"]    = "188.0";
    a_configurationDatabase["ws.flywheel.speed.mapping.key.top.speed"]       = "210.0";
    
    // Auto Program Config:  Shoot 3
    a_configurationDatabase["ws.auto.shoot3.state.prime1.fly.time"] = "1.25";
    a_configurationDatabase["ws.auto.shoot3.state.spin1.fly.time"]  = "0.75";
    a_configurationDatabase["ws.auto.shoot3.state.shoot1.time"]     = "1.15";
    a_configurationDatabase["ws.auto.shoot3.state.prime2.fly.time"] = "0.40";
    a_configurationDatabase["ws.auto.shoot3.state.spin2.fly.time"]  = "0.77";
    a_configurationDatabase["ws.auto.shoot3.state.shoot2.time"]     = "1.15";
    
    // Auto Program Config:  Shoot 6
    a_configurationDatabase["ws.auto.shoot6.state.prime1.fly.time"] = "1.25";
    a_configurationDatabase["ws.auto.shoot6.state.spin1.fly.time"]  = "0.75";
    a_configurationDatabase["ws.auto.shoot6.state.shoot1.time"]     = "1.15";
    a_configurationDatabase["ws.auto.shoot6.state.prime2.fly.time"] = "0.40";
    a_configurationDatabase["ws.auto.shoot6.state.spin2.fly.time"]  = "0.77";
    a_configurationDatabase["ws.auto.shoot6.state.shoot2.time"]     = "1.15";
    
    // Auto Program:  Tip Shoot
    a_configurationDatabase["ws.auto.tipshoot.coop.bridge.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.tipshoot.coop.bridge.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.coop.bridge.heading"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.coop.bridge.timeout"] = "5.00";
    a_configurationDatabase["ws.auto.tipshoot.wait.for.coast.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.put.down.coop.bridge.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.drive.onto.coop.bridge.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.tipshoot.drive.onto.coop.bridge.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.drive.onto.coop.bridge.heading"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.drive.onto.coop.bridge.timeout"] = "5.00";
    a_configurationDatabase["ws.auto.tipshoot.accumulate.balls.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shooting.position.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.tipshoot.shooting.position.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shooting.position.heading"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shooting.position.timeout"] = "5.00";
    a_configurationDatabase["ws.auto.tipshoot.wait.for.angle.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.angle.between.facing.and.not.facing"] = "185.0";
    a_configurationDatabase["ws.auto.tipshoot.second.flywheel.setpoint"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.wait.for.turret.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shoot.state.spin.fly.time.1"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shoot.state.shoot.1.time"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shoot.state.spin.fly.time.2"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shoot.state.shoot.2.time"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shoot.state.spin.fly.time.3"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shoot.state.shoot.3.time"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shoot.state.spin.fly.time.4"] = "0.0";
    a_configurationDatabase["ws.auto.tipshoot.shoot.state.shoot.4.time"] = "0.0";
    
    // Auto Program:  Shoot Tip
    a_configurationDatabase["ws.auto.shoottip.coop.bridge.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.shoottip.coop.bridge.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.coop.bridge.heading"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.coop.bridge.timeout"] = "5.00";
    a_configurationDatabase["ws.auto.shoottip.wait.for.coast.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.put.down.coop.bridge.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.drive.onto.coop.bridge.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.shoottip.drive.onto.coop.bridge.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.drive.onto.coop.bridge.heading"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.drive.onto.coop.bridge.timeout"] = "5.00";
    a_configurationDatabase["ws.auto.shoottip.accumulate.balls.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.shooting.position.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.shoottip.shooting.position.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.shooting.position.heading"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.shooting.position.timeout"] = "5.00";
    a_configurationDatabase["ws.auto.shoottip.shoot.state.spin.fly.time.1"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.shoot.state.shoot.1.time"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.shoot.state.spin.fly.time.2"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.shoot.state.shoot.2.time"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.shoot.state.spin.fly.time.3"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.shoot.state.shoot.3.time"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.shoot.state.spin.fly.time.4"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.shoot.state.shoot.4.time"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.wait.for.angle.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.angle.between.facing.and.not.facing"] = "185.0";
    a_configurationDatabase["ws.auto.shoottip.second.flywheel.setpoint"] = "0.0";
    a_configurationDatabase["ws.auto.shoottip.wait.for.turret.timeout"] = "0.0";
    
    
    // Auto Program:  Uber Drive tip 2 shoot 2
    a_configurationDatabase["ws.auto.drive.coop.bridge.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.drive.coop.bridge.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.drive.coop.bridge.heading"] = "0.0";
    a_configurationDatabase["ws.auto.drive.coop.bridge.timeout"] = "5.00";
    a_configurationDatabase["ws.auto.drive.wait.for.coast.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.drive.put.down.coop.bridge.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.drive.shooting.position.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.drive.shooting.position.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.drive.shooting.position.heading"] = "0.0";
    a_configurationDatabase["ws.auto.drive.shooting.position.timeout"] = "5.00";
    a_configurationDatabase["ws.auto.drive.wait.for.angle.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.drive.angle.between.facing.and.not.facing"] = "185.0";
    a_configurationDatabase["ws.auto.drive.second.flywheel.setpoint"] = "0.0";
    a_configurationDatabase["ws.auto.drive.wait.for.turret.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.drive.shoot.state.spin.fly.time.1"] = "0.0";
    a_configurationDatabase["ws.auto.drive.shoot.state.shoot.1.time"] = "0.0";
    a_configurationDatabase["ws.auto.drive.shoot.state.spin.fly.time.2"] = "0.0";
    a_configurationDatabase["ws.auto.drive.shoot.state.shoot.2.time"] = "0.0";
    a_configurationDatabase["ws.auto.drive.shoot.state.spin.fly.time.3"] = "0.0";
    a_configurationDatabase["ws.auto.drive.shoot.state.shoot.3.time"] = "0.0";
    
    
    // Camera IMAQ Config
    a_configurationDatabase["ws.camera.imaq.lumen.min.threshold"] = "154";
    a_configurationDatabase["ws.camera.imaq.lumen.max.threshold"] = "215";
    
    // Shooter Preset Config
    a_configurationDatabase["ws.shooter.preset.facing.turret.heading.setpoint"]         = "3.80";
    a_configurationDatabase["ws.shooter.preset.facing.flywheel.speed.setpoint"]         = "85.0";
    a_configurationDatabase["ws.shooter.preset.facing.flywheel.raw.motor.output"]       = "0.85";
    a_configurationDatabase["ws.shooter.preset.reverse.turret.heading.setpoint"]        = "1.20";
    a_configurationDatabase["ws.shooter.preset.reverse.flywheel.speed.setpoint"]        = "80.0";
    a_configurationDatabase["ws.shooter.preset.reverse.flywheel.raw.motor.output"]      = "0.80";
    a_configurationDatabase["ws.shooter.preset.fender.left.turret.heading.setpoint"]    = "3.60";
    a_configurationDatabase["ws.shooter.preset.fender.left.flywheel.speed.setpoint"]    = "29.0";
    a_configurationDatabase["ws.shooter.preset.fender.left.flywheel.raw.motor.output"]  = "0.60";
    a_configurationDatabase["ws.shooter.preset.fender.right.turret.heading.setpoint"]   = "4.0";
    a_configurationDatabase["ws.shooter.preset.fender.right.flywheel.speed.setpoint"]   = "0.31";
    a_configurationDatabase["ws.shooter.preset.fender.right.flywheel.raw.motor.output"] = "0.60";
    a_configurationDatabase["ws.shooter.preset.auto.turret.heading.setpoint"]           = "1.2";
    a_configurationDatabase["ws.shooter.preset.auto.flywheel.speed.setpoint"]           = "0.0";
    a_configurationDatabase["ws.shooter.preset.auto.flywheel.raw.motor.output"]         = "0.0";
    
    // Auto Fender Right Config
    a_configurationDatabase["ws.auto.fender.right.drive.fender.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.drive.fender.heading"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.drive.fender.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.fender.right.drive.fender.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.square.up.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.square.up.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.coast.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.shoot.first.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.wait.for.flywheel.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.shoot.second.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.back.up.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.back.up.heading"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.back.up.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.fender.right.back.up.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.turn.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.turn.heading"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.turn.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.fender.right.turn.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.drive.alliance.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.wait.for.turn.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.drive.alliance.heading"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.drive.alliance.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.fender.right.drive.alliance.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.coast.alliance.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.right.dump.alliance.timeout"] = "0.0";
    
    // Auto Fender Left Config
    a_configurationDatabase["ws.auto.fender.left.drive.fender.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.drive.fender.heading"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.drive.fender.encoder.target"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.drive.fender.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.square.up.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.square.up.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.coast.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.shoot.first.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.wait.for.flywheel.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.shoot.second.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.back.up.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.back.up.heading"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.back.up.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.fender.left.back.up.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.turn.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.turn.heading"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.turn.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.fender.left.turn.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.drive.barrier.throttle"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.wait.for.turn.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.drive.barrier.heading"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.drive.barrier.encoder.target"] = "0";
    a_configurationDatabase["ws.auto.fender.left.drive.barrier.timeout"] = "0.0";
    a_configurationDatabase["ws.auto.fender.left.coast.barrier.timeout"] = "0.0";
    
    //-------------------------------------------------------------------------
    // Create default structures out of the values in the configuration DB.
    //-------------------------------------------------------------------------
    // Turret PID Config
    WsTypes::WsPidConfigurationDataT default_turret_pid_values;
    ::memset(&default_turret_pid_values, 0x00, sizeof(default_turret_pid_values));
    this->extractTurretPidConfig(default_turret_pid_values);
    
    // Flywheel PID Config
    WsTypes::WsPidConfigurationDataT default_flywheel_pid_values;
    ::memset(&default_flywheel_pid_values, 0x00, sizeof(default_flywheel_pid_values));
    this->extractFlywheelPidConfig(default_flywheel_pid_values);
    
    // Ball Transport Config
    WsTypes::WsBallTransportMotorValueType default_bt_motor_vals;
    ::memset(&default_bt_motor_vals, 0x00, sizeof(default_bt_motor_vals));
    this->extractBallTransportMotorValsConfig(default_bt_motor_vals);
    
    // Flywheel Location Speed Mapping Config
    WsTypes::WsLocationSpeedMappingType default_location_mapping;
    ::memset(&default_location_mapping, 0x00, sizeof(default_location_mapping));
    this->extractFlywheelLocationSpeedMapping(default_location_mapping);
    
    // Auto Program Shoot 3 Config
    WsTypes::WsAutoShootProgramConfigType default_auto_shoot3_config;
    ::memset(&default_auto_shoot3_config, 0x00, sizeof(default_auto_shoot3_config));
    this->extractAutoShoot3Config(default_auto_shoot3_config);
    
    // Auto Program Shoot 6 Config
    WsTypes::WsAutoShootProgramConfigType default_auto_shoot6_config;
    ::memset(&default_auto_shoot6_config, 0x00, sizeof(default_auto_shoot6_config));
    this->extractAutoShoot6Config(default_auto_shoot6_config);
    
    // Auto Program Tip Shoot
    WsTypes::WsAutoTipShootProgramConfigType default_tip_shoot_auto_config;
    ::memset(&default_tip_shoot_auto_config, 0x00, sizeof(default_tip_shoot_auto_config));
    this->extractAutoTipShootConfig(default_tip_shoot_auto_config);
    
    // Auto Program Shoot Tip
    WsTypes::WsAutoShootTipProgramConfigType default_shoot_tip_auto_config;
    ::memset(&default_shoot_tip_auto_config, 0x00, sizeof(default_shoot_tip_auto_config));
    this->extractAutoShootTipConfig(default_shoot_tip_auto_config);
    
    // Camera IMAQ Config
    WsTypes::WsCameraProcessingImaqData default_camera_imaq_data;
    ::memset(&default_camera_imaq_data, 0x00, sizeof(default_camera_imaq_data));
    this->extractCameraProcessingImaqData(default_camera_imaq_data);
    
    // Shooter Preset Config
    WsTypes::WsShooterPresetConfigT default_shooter_preset_config;
    ::memset(&default_shooter_preset_config, 0x00, sizeof(default_shooter_preset_config));
    this->extractShooterPresetConfig(default_shooter_preset_config);
    
    // Stuffs
    WsTypes::WsAutoDriveProgramConfigType default_auto_drive_config;
    ::memset(&default_auto_drive_config, 0x00, sizeof(default_auto_drive_config));
    this->extractAutoDriveProgramConfig(default_auto_drive_config);
    
    WsTypes::WsAutoFenderRightProgramConfigType default_auto_fender_right_config;
    ::memset(&default_auto_fender_right_config, 0x00, sizeof(default_auto_fender_right_config));
    this->extractAutoFenderRightProgramConfig(default_auto_fender_right_config);
    
    WsTypes::WsAutoFenderLeftProgramConfigType default_auto_fender_left_config;
    ::memset(&default_auto_fender_left_config, 0x00, sizeof(default_auto_fender_left_config));
    this->extractAutoFenderLeftProgramConfig(default_auto_fender_left_config);
    
    //-------------------------------------------------------------------------
    // Create the configuration database elements
    //-------------------------------------------------------------------------
    ap_turretPidConfigElement =
        new WsFacadeElement<WsTypes::WsPidConfigurationDataT>("TurretPidConfig",
                                                              sizeof(default_turret_pid_values),
                                                              &default_turret_pid_values,
                                                              ap_mutexSemaphore);
                                                              
    ap_flywheelPidConfigElement =
        new WsFacadeElement<WsTypes::WsPidConfigurationDataT>("FlywheelPidConfig",
                                                              sizeof(default_flywheel_pid_values),
                                                              &default_flywheel_pid_values,
                                                              ap_mutexSemaphore);
                                                              
    ap_flywheelLocationSpeedMappingElement =
        new WsFacadeElement<WsTypes::WsLocationSpeedMappingType>("LocaitonSpeedMapping",
                                                                 sizeof(default_location_mapping),
                                                                 &default_location_mapping,
                                                                 ap_mutexSemaphore);
                                                                 
    ap_ballTransportMotorValsElement =
        new WsFacadeElement<WsTypes::WsBallTransportMotorValueType>("BallTransportMotorVals",
                                                                    sizeof(default_bt_motor_vals),
                                                                    &default_bt_motor_vals,
                                                                    ap_mutexSemaphore);
                                                                    
    ap_autoShoot3ConfigElement =
        new WsFacadeElement<WsTypes::WsAutoShootProgramConfigType>("AutoShoot3Config",
                                                                   sizeof(default_auto_shoot3_config),
                                                                   &default_auto_shoot3_config,
                                                                   ap_mutexSemaphore);
                                                                   
    ap_autoShoot6ConfigElement =
        new WsFacadeElement<WsTypes::WsAutoShootProgramConfigType>("AutoShoot6Config",
                                                                   sizeof(default_auto_shoot6_config),
                                                                   &default_auto_shoot6_config,
                                                                   ap_mutexSemaphore);
                                                                   
    ap_cameraProcessingImaqData =
        new WsFacadeElement<WsTypes::WsCameraProcessingImaqData>("CameraProcessingImaqData",
                                                                 sizeof(default_camera_imaq_data),
                                                                 &default_camera_imaq_data,
                                                                 ap_mutexSemaphore);
                                                                 
    ap_autoTipShootConfigElement =
        new WsFacadeElement<WsTypes::WsAutoTipShootProgramConfigType>("AutoTipShootConfig",
                                                                      sizeof(default_tip_shoot_auto_config),
                                                                      &default_tip_shoot_auto_config,
                                                                      ap_mutexSemaphore);
                                                                      
    ap_autoShootTipConfigElement =
        new WsFacadeElement<WsTypes::WsAutoShootTipProgramConfigType>("AutoShootTipConfig",
                                                                      sizeof(default_shoot_tip_auto_config),
                                                                      &default_shoot_tip_auto_config,
                                                                      ap_mutexSemaphore);
                                                                      
    ap_shooterPresetConfigElement =
        new WsFacadeElement<WsTypes::WsShooterPresetConfigT>("ShooterPresetConfig",
                                                             sizeof(default_shooter_preset_config),
                                                             &default_shooter_preset_config,
                                                             ap_mutexSemaphore);
                                                             
    ap_autoDriveProgramConfigElement =
        new WsFacadeElement<WsTypes::WsAutoDriveProgramConfigType>("AutoDriveProgramConfig",
                                                                   sizeof(default_auto_drive_config),
                                                                   &default_auto_drive_config,
                                                                   ap_mutexSemaphore);
                                                                   
    ap_autoFenderRightProgramConfigElement =
        new WsFacadeElement<WsTypes::WsAutoFenderRightProgramConfigType>("AutoFenderRightProgramConfig",
                                                                         sizeof(default_auto_fender_right_config),
                                                                         &default_auto_fender_right_config,
                                                                         ap_mutexSemaphore);
                                                                         
    ap_autoFenderLeftProgramConfigElement =
        new WsFacadeElement<WsTypes::WsAutoFenderLeftProgramConfigType>("AutoFenderLeftProgramConfig",
                                                                        sizeof(default_auto_fender_left_config),
                                                                        &default_auto_fender_left_config,
                                                                        ap_mutexSemaphore);
}

WsConfigFacade::~WsConfigFacade(void)
{
    asp_instance = NULL;
    
    delete ap_turretPidConfigElement;               ap_turretPidConfigElement = NULL;
    delete ap_flywheelPidConfigElement;             ap_flywheelPidConfigElement = NULL;
    delete ap_flywheelLocationSpeedMappingElement;  ap_flywheelLocationSpeedMappingElement = NULL;
    delete ap_autoShoot3ConfigElement;              ap_autoShoot3ConfigElement = NULL;
    delete ap_cameraProcessingImaqData;             ap_cameraProcessingImaqData = NULL;
    delete ap_ballTransportMotorValsElement;        ap_ballTransportMotorValsElement = NULL;
    delete ap_autoTipShootConfigElement;            ap_autoTipShootConfigElement = NULL;
    delete ap_autoShoot6ConfigElement;              ap_autoShoot6ConfigElement = NULL;
    delete ap_autoShootTipConfigElement;            ap_autoShootTipConfigElement = NULL;
    delete ap_autoDriveProgramConfigElement;        ap_autoDriveProgramConfigElement = NULL;
    delete ap_autoFenderRightProgramConfigElement;  ap_autoFenderRightProgramConfigElement = NULL;
    delete ap_autoFenderLeftProgramConfigElement;   ap_autoFenderLeftProgramConfigElement = NULL;
    
    // Delete the mutex
    semFlush(ap_mutexSemaphore);
    semDelete(ap_mutexSemaphore);
    ap_mutexSemaphore = NULL;
}

void
WsConfigFacade::updateCache(void)
{
    //In this function, reload cache from the files
    WS_LOG_TRACE("Begin");
    
    // Open the file so we can read the contents
    const int LINE_BUFFER_SIZE = 256;
    const int KEY_BUFFER_SIZE = 256;
    const int VALUE_BUFFER_SIZE = 256;
    const char* WS_CONFIG_FILENAME = "/ws_config.txt";
    FILE* p_file = NULL;
    p_file = ::fopen(WS_CONFIG_FILENAME, "r");
    
    // Check for a valid file handle
    if (NULL != p_file)
    {
        //
        // File opened successfully, read in the contents...
        //  The file format is as follows:
        //  # This is a comment...
        //  Blank lines are allowed, and ignored
        //  Non-blank lines are expected to be in key=value pairs.
        //
        char line_buffer[LINE_BUFFER_SIZE];
        while (0 == feof(p_file))
        {
            ::memset(line_buffer, 0x00, sizeof(line_buffer));
            ::fgets(line_buffer, sizeof(line_buffer), p_file);
            
            if (('#' == line_buffer[0]) ||
                ('\n' == line_buffer[0]) ||
                ('\r' == line_buffer[0]) ||
                (strlen(line_buffer) < 2))
            {
                // Ignore, this is a blank or comment line
            }
            else
            {
                char key_buffer[KEY_BUFFER_SIZE];
                char value_buffer[VALUE_BUFFER_SIZE];
                ::memset(key_buffer, 0x00, sizeof(key_buffer));
                ::memset(value_buffer, 0x00, sizeof(value_buffer));
                
                // Does this line conform to the key=value pair?
                const char* pc_equal_sign = strchr(line_buffer, '=');
                const char* pc_end_of_line = strchr(line_buffer, '\n');
                if ((NULL != pc_equal_sign) &&
                    (NULL != pc_end_of_line))
                {
                    // Find out how many characters are in the 'key' descriptor
                    const int key_len = pc_equal_sign - line_buffer;
                    const int value_len = pc_end_of_line - (pc_equal_sign + 1);
                    
                    // Copy the key and value strings into their respective buffers
                    ::strncpy(key_buffer, line_buffer, key_len);
                    ::strncpy(value_buffer, pc_equal_sign + 1, value_len);
                    
                    //
                    // Yes, it does!
                    //  We will insert this key/value pair into our configuration
                    //  database.  But, we want to know if that key/value pair
                    //  exists first.  This will let us know if we forgot to code
                    //  a default value for any "added" parameters.
                    //
                    std::string key(key_buffer);
                    std::string value(value_buffer);
                    WsConfigMapType::iterator iter = a_configurationDatabase.find(key);
                    WsConfigMapType::iterator end_iter = a_configurationDatabase.end();
                    
                    if (iter != end_iter)
                    {
                        // This is an update to an existing parameter - expected case
                        //  insert(it,pair) doesn't seem to work... so we will brute-force it
                        a_configurationDatabase.erase(iter);
                        a_configurationDatabase.insert(std::pair<std::string, std::string>(key, value));
                    }
                    else
                    {
                        // This is a new parameter...
                        //  We really should have a default value for this!
                        WS_LOG_ERROR("Parameter Does Not Have Default: %s", key_buffer);
                        a_configurationDatabase[key] = value;
                    }
                }
                else
                {
                    // Some other line, we can't parse it
                    WS_LOG_ERROR("Unrecognized line: '%s'", line_buffer);
                }
            }
        } // end while()
        
        // Update Logger Configuration
        // @TODO This should probably be an observer like everything else...
        if (true == this->stringToBoolean(a_configurationDatabase["ws.logger.fatal"], true))
        { WsLogger::instance()->enableLoggingForLevel(WsLogger::LEVEL_FATAL); }
        else
        { WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_FATAL); }
        
        if (true == this->stringToBoolean(a_configurationDatabase["ws.logger.error"], true))
        { WsLogger::instance()->enableLoggingForLevel(WsLogger::LEVEL_ERROR); }
        else
        { WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_ERROR); }
        
        if (true == this->stringToBoolean(a_configurationDatabase["ws.logger.warning"], true))
        { WsLogger::instance()->enableLoggingForLevel(WsLogger::LEVEL_WARNING); }
        else
        { WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_WARNING); }
        
        if (true == this->stringToBoolean(a_configurationDatabase["ws.logger.notice"], true))
        { WsLogger::instance()->enableLoggingForLevel(WsLogger::LEVEL_NOTICE); }
        else
        { WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_NOTICE); }
        
        if (true == this->stringToBoolean(a_configurationDatabase["ws.logger.info"], false))
        { WsLogger::instance()->enableLoggingForLevel(WsLogger::LEVEL_INFO); }
        else
        { WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_INFO); }
        
        if (true == this->stringToBoolean(a_configurationDatabase["ws.logger.debug"], false))
        { WsLogger::instance()->enableLoggingForLevel(WsLogger::LEVEL_DEBUG); }
        else
        { WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_DEBUG); }
        
        if (true == this->stringToBoolean(a_configurationDatabase["ws.logger.trace"], false))
        { WsLogger::instance()->enableLoggingForLevel(WsLogger::LEVEL_TRACE); }
        else
        { WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_TRACE); }
        
        //----------------------------------------------------------------------
        // Push new data into the subjects
        //----------------------------------------------------------------------
        // Turret PID Config
        WsTypes::WsPidConfigurationDataT turret_pid_values;
        ::memset(&turret_pid_values, 0x00, sizeof(turret_pid_values));
        this->extractTurretPidConfig(turret_pid_values);
        this->setTurretPidConfig(turret_pid_values);
        
        // Flywheel PID Config
        WsTypes::WsPidConfigurationDataT flywheel_pid_values;
        ::memset(&flywheel_pid_values, 0x00, sizeof(flywheel_pid_values));
        this->extractFlywheelPidConfig(flywheel_pid_values);
        this->setFlywheelPidConfig(flywheel_pid_values);
        
        // Ball Transport Config
        WsTypes::WsBallTransportMotorValueType bt_motor_vals;
        ::memset(&bt_motor_vals, 0x00, sizeof(bt_motor_vals));
        this->extractBallTransportMotorValsConfig(bt_motor_vals);
        this->setBallTransportMotorValsConfig(bt_motor_vals);
        
        // Flywheel Location Speed Mapping Config
        WsTypes::WsLocationSpeedMappingType location_mapping;
        ::memset(&location_mapping, 0x00, sizeof(location_mapping));
        this->extractFlywheelLocationSpeedMapping(location_mapping);
        this->setFlywheelLocationSpeedMapping(location_mapping);
        
        // Auto Program Shoot 3 Config
        WsTypes::WsAutoShootProgramConfigType auto_shoot3_config;
        ::memset(&auto_shoot3_config, 0x00, sizeof(auto_shoot3_config));
        this->extractAutoShoot3Config(auto_shoot3_config);
        this->setAutoShoot3Config(auto_shoot3_config);
        
        // Auto Program Shoot 6 Config
        WsTypes::WsAutoShootProgramConfigType auto_shoot6_config;
        ::memset(&auto_shoot6_config, 0x00, sizeof(auto_shoot6_config));
        this->extractAutoShoot6Config(auto_shoot6_config);
        this->setAutoShoot6Config(auto_shoot6_config);
        
        // Auto Program Tip Shoot
        WsTypes::WsAutoTipShootProgramConfigType tip_shoot_auto_config;
        ::memset(&tip_shoot_auto_config, 0x00, sizeof(tip_shoot_auto_config));
        this->extractAutoTipShootConfig(tip_shoot_auto_config);
        this->setAutoTipShootConfig(tip_shoot_auto_config);
        
        // Auto Program Shoot Tip
        WsTypes::WsAutoShootTipProgramConfigType shoot_tip_auto_config;
        ::memset(&shoot_tip_auto_config, 0x00, sizeof(shoot_tip_auto_config));
        this->extractAutoShootTipConfig(shoot_tip_auto_config);
        this->setAutoShootTipConfig(shoot_tip_auto_config);
        
        // Camera IMAQ Config
        WsTypes::WsCameraProcessingImaqData camera_imaq_data;
        ::memset(&camera_imaq_data, 0x00, sizeof(camera_imaq_data));
        this->extractCameraProcessingImaqData(camera_imaq_data);
        this->setCameraProcessingImaqData(camera_imaq_data);
        
        // Shooter Preset Config
        WsTypes::WsShooterPresetConfigT shooter_preset_config;
        ::memset(&shooter_preset_config, 0x00, sizeof(shooter_preset_config));
        this->extractShooterPresetConfig(shooter_preset_config);
        this->setShooterPresetConfig(shooter_preset_config);
        
        // Auto Drive Config
        WsTypes::WsAutoDriveProgramConfigType auto_drive_config;
        ::memset(&auto_drive_config, 0x00, sizeof(auto_drive_config));
        this->extractAutoDriveProgramConfig(auto_drive_config);
        this->setAutoDriveConfig(auto_drive_config);
        
        WsTypes::WsAutoFenderRightProgramConfigType auto_fender_right_config;
        ::memset(&auto_fender_right_config, 0x00, sizeof(auto_fender_right_config));
        this->extractAutoFenderRightProgramConfig(auto_fender_right_config);
        this->setAutoFenderRightConfig(auto_fender_right_config);
        
        WsTypes::WsAutoFenderLeftProgramConfigType auto_fender_left_config;
        ::memset(&auto_fender_left_config, 0x00, sizeof(auto_fender_left_config));
        this->extractAutoFenderLeftProgramConfig(auto_fender_left_config);
        this->setAutoFenderLeftConfig(auto_fender_left_config);
    }
    else
    {
        // This is really bad...
        WS_LOG_ERROR("Unable to open config file '%s', errno(%d): '%s'",
                     WS_CONFIG_FILENAME,
                     errno,
                     ::strerror(errno));
    }
    
    // Don't forget to close the file
    if (NULL != p_file)
    { ::fclose(p_file); }
    
    // For debugging, dump the new database contents
    this->dumpDatabaseToLog();
    
    WS_LOG_TRACE("End");
}

WsSubject*
WsConfigFacade::getTurretPidConfig(WsTypes::WsPidConfigurationDataT& r_config)
{
    ap_turretPidConfigElement->getState(r_config);
    return ap_turretPidConfigElement;
}

void
WsConfigFacade::setTurretPidConfig(const WsTypes::WsPidConfigurationDataT& rc_config)
{
    ap_turretPidConfigElement->setState(rc_config);
}

WsSubject*
WsConfigFacade::getFlywheelPidConfig(WsTypes::WsPidConfigurationDataT& r_config)
{
    ap_flywheelPidConfigElement->getState(r_config);
    return ap_flywheelPidConfigElement;
}

void
WsConfigFacade::setFlywheelPidConfig(const WsTypes::WsPidConfigurationDataT& rc_config)
{
    ap_flywheelPidConfigElement->setState(rc_config);
}

WsSubject*
WsConfigFacade::getFlywheelLocationSpeedMapping(WsTypes::WsLocationSpeedMappingType& r_mappingData)
{
    ap_flywheelLocationSpeedMappingElement->getState(r_mappingData);
    return ap_flywheelLocationSpeedMappingElement;
}

void
WsConfigFacade::setFlywheelLocationSpeedMapping(const WsTypes::WsLocationSpeedMappingType& rc_mappingData)
{
    ap_flywheelLocationSpeedMappingElement->setState(rc_mappingData);
}

WsSubject*
WsConfigFacade::getAutoShoot6Config(WsTypes::WsAutoShootProgramConfigType& r_config)
{
    ap_autoShoot6ConfigElement->getState(r_config);
    return ap_autoShoot6ConfigElement;
}

void
WsConfigFacade::setAutoShoot6Config(const WsTypes::WsAutoShootProgramConfigType& rc_config)
{
    ap_autoShoot6ConfigElement->setState(rc_config);
}

WsSubject*
WsConfigFacade::getCameraProcessingImaqData(WsTypes::WsCameraProcessingImaqData& r_imaqData)
{
    ap_cameraProcessingImaqData->getState(r_imaqData);
    return ap_cameraProcessingImaqData;
}

void
WsConfigFacade::setCameraProcessingImaqData(const WsTypes::WsCameraProcessingImaqData& rc_imaqData)
{
    ap_cameraProcessingImaqData->setState(rc_imaqData);
}

WsSubject*
WsConfigFacade::getBallTransportMotorValsConfig(WsTypes::WsBallTransportMotorValueType& r_bt_vals)
{
    ap_ballTransportMotorValsElement->getState(r_bt_vals);
    return ap_ballTransportMotorValsElement;
}

void
WsConfigFacade::setBallTransportMotorValsConfig(const WsTypes::WsBallTransportMotorValueType& rc_bt_vals)
{
    ap_ballTransportMotorValsElement->setState(rc_bt_vals);
}

WsSubject*
WsConfigFacade::getAutoTipShootConfig(WsTypes::WsAutoTipShootProgramConfigType& r_config)
{
    ap_autoTipShootConfigElement->getState(r_config);
    return ap_autoTipShootConfigElement;
}

void
WsConfigFacade::setAutoTipShootConfig(const WsTypes::WsAutoTipShootProgramConfigType& rc_config)
{
    ap_autoTipShootConfigElement->setState(rc_config);
}

WsSubject*
WsConfigFacade::getAutoShoot3Config(WsTypes::WsAutoShootProgramConfigType& r_config)
{
    ap_autoShoot3ConfigElement->getState(r_config);
    return ap_autoShoot3ConfigElement;
}

void
WsConfigFacade::setAutoShoot3Config(const WsTypes::WsAutoShootProgramConfigType& rc_config)
{
    ap_autoShoot3ConfigElement->setState(rc_config);
}

WsSubject*
WsConfigFacade::getAutoShootTipConfig(WsTypes::WsAutoShootTipProgramConfigType& r_config)
{
    ap_autoShootTipConfigElement->getState(r_config);
    return ap_autoShootTipConfigElement;
}

void
WsConfigFacade::setAutoShootTipConfig(const WsTypes::WsAutoShootTipProgramConfigType& rc_config)
{
    ap_autoShootTipConfigElement->setState(rc_config);
}

WsSubject*
WsConfigFacade::getShooterPresetConfig(WsTypes::WsShooterPresetConfigT& r_config)
{
    ap_shooterPresetConfigElement->getState(r_config);
    return ap_shooterPresetConfigElement;
}

void
WsConfigFacade::setShooterPresetConfig(const WsTypes::WsShooterPresetConfigT& rc_config)
{
    ap_shooterPresetConfigElement->setState(rc_config);
}

WsSubject*
WsConfigFacade::getAutoDriveConfig(WsTypes::WsAutoDriveProgramConfigType& r_config)
{
    ap_autoDriveProgramConfigElement->getState(r_config);
    return ap_autoDriveProgramConfigElement;
}

void
WsConfigFacade::setAutoDriveConfig(const WsTypes::WsAutoDriveProgramConfigType& rc_config)
{
    ap_autoDriveProgramConfigElement->setState(rc_config);
}

float
WsConfigFacade::stringToFloat(const std::string& rc_string, float defaultValue)
{
    float result = 0.0;
    if (1 == sscanf(rc_string.c_str(), "%f", &result))
    {
        return result;
    }
    else
    {
        WS_LOG_ERROR("Failed to convert '%s', using defaultValue: %f", rc_string.c_str(), defaultValue);
        return defaultValue;
    }
}

bool
WsConfigFacade::stringToBoolean(const std::string& rc_string, bool defaultValue)
{
    if (0 == strncmp("true", rc_string.c_str(), strlen("true")))
    {
        return true;
    }
    else if (0 == strncmp("false", rc_string.c_str(), strlen("false")))
    {
        return false;
    }
    else
    {
        WS_LOG_ERROR("Failed to convert '%s', using defaultValue: %d", rc_string.c_str(), defaultValue);
        return defaultValue;
    }
}

int
WsConfigFacade::stringToInt(const std::string& rc_string, int defaultValue)
{
    int result = 0;
    if (1 == sscanf(rc_string.c_str(), "%d", &result))
    {
        return result;
    }
    else
    {
        WS_LOG_ERROR("Failed to convert '%s', using defaultValue: %d", rc_string.c_str(), defaultValue);
        return defaultValue;
    }
}

void
WsConfigFacade::extractTurretPidConfig(WsTypes::WsPidConfigurationDataT& r_config)
{
    r_config.k_p                       = this->stringToFloat(a_configurationDatabase["ws.turret.pid.kp"]);
    r_config.k_i                       = this->stringToFloat(a_configurationDatabase["ws.turret.pid.ki"]);
    r_config.k_d                       = this->stringToFloat(a_configurationDatabase["ws.turret.pid.kd"]);
    r_config.error_epsilon             = this->stringToFloat(a_configurationDatabase["ws.turret.pid.error.epsilon"]);
    r_config.static_epsilon            = this->stringToFloat(a_configurationDatabase["ws.turret.pid.static.epsilon"]);
    r_config.max_integral              = this->stringToFloat(a_configurationDatabase["ws.turret.pid.max.integral"]);
    r_config.error_increment           = this->stringToFloat(a_configurationDatabase["ws.turret.pid.error.increment"]);
    r_config.integral_error_thresh     = this->stringToFloat(a_configurationDatabase["ws.turret.pid.integral.error.threshold"]);
    r_config.differentiator_band_limit = this->stringToFloat(a_configurationDatabase["ws.turret.pid.differentiator.band.limit"]);
    r_config.max_output                = this->stringToFloat(a_configurationDatabase["ws.turret.pid.max.output"]);
    r_config.min_output                = this->stringToFloat(a_configurationDatabase["ws.turret.pid.min.output"]);
    r_config.max_input                 = this->stringToFloat(a_configurationDatabase["ws.turret.pid.max.input"]);
    r_config.min_input                 = this->stringToFloat(a_configurationDatabase["ws.turret.pid.min.input"]);
    r_config.min_on_target_time        = this->stringToFloat(a_configurationDatabase["ws.turret.pid.stabilization.time"]);
}

void
WsConfigFacade::extractFlywheelPidConfig(WsTypes::WsPidConfigurationDataT& r_config)
{
    r_config.k_p                       = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.kp"]);
    r_config.k_i                       = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.ki"]);
    r_config.k_d                       = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.kd"]);
    r_config.error_epsilon             = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.error.epsilon"]);
    r_config.static_epsilon            = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.static.epsilon"]);
    r_config.max_integral              = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.max.integral"]);
    r_config.error_increment           = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.error.increment"]);
    r_config.integral_error_thresh     = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.integral.error.threshold"]);
    r_config.differentiator_band_limit = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.differentiator.band.limit"]);
    r_config.max_output                = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.max.output"]);
    r_config.min_output                = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.min.output"]);
    r_config.max_input                 = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.max.input"]);
    r_config.min_input                 = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.min.input"]);
    r_config.min_on_target_time        = this->stringToFloat(a_configurationDatabase["ws.flywheel.pid.stabilization.time"]);
}

void
WsConfigFacade::extractFlywheelLocationSpeedMapping(WsTypes::WsLocationSpeedMappingType& r_config)
{
    r_config.fender_distance     = this->stringToFloat(a_configurationDatabase["ws.flywheel.speed.mapping.fender.distance"]);
    r_config.fender_fws          = this->stringToFloat(a_configurationDatabase["ws.flywheel.speed.mapping.fender.speed"]);
    r_config.key_bottom_distance = this->stringToFloat(a_configurationDatabase["ws.flywheel.speed.mapping.key.bottom.distance"]);
    r_config.key_bottom_fws      = this->stringToFloat(a_configurationDatabase["ws.flywheel.speed.mapping.key.bottom.speed"]);
    r_config.key_middle_distance = this->stringToFloat(a_configurationDatabase["ws.flywheel.speed.mapping.key.middle.distance"]);
    r_config.key_middle_fws      = this->stringToFloat(a_configurationDatabase["ws.flywheel.speed.mapping.key.middle.speed"]);
    r_config.key_top_distance    = this->stringToFloat(a_configurationDatabase["ws.flywheel.speed.mapping.key.top.distance"]);
    r_config.key_top_fws         = this->stringToFloat(a_configurationDatabase["ws.flywheel.speed.mapping.key.top.speed"]);
}

void
WsConfigFacade::extractAutoShoot6Config(WsTypes::WsAutoShootProgramConfigType& r_config)
{
    r_config.shoot_state_prime_fly_1_time = this->stringToFloat(a_configurationDatabase["ws.auto.shoot6.state.prime1.fly.time"]);
    r_config.shoot_state_spin_fly_time_1  = this->stringToFloat(a_configurationDatabase["ws.auto.shoot6.state.spin1.fly.time"]);
    r_config.shoot_state_shoot_1_time     = this->stringToFloat(a_configurationDatabase["ws.auto.shoot6.state.shoot1.time"]);
    r_config.shoot_state_prime_fly_2_time = this->stringToFloat(a_configurationDatabase["ws.auto.shoot6.state.prime2.fly.time"]);
    r_config.shoot_state_spin_fly_time_2  = this->stringToFloat(a_configurationDatabase["ws.auto.shoot6.state.spin2.fly.time"]);
    r_config.shoot_state_shoot_2_time     = this->stringToFloat(a_configurationDatabase["ws.auto.shoot6.state.shoot2.time"]);
}

void
WsConfigFacade::extractCameraProcessingImaqData(WsTypes::WsCameraProcessingImaqData& r_config)
{
    r_config.lumen_min_threshold = this->stringToInt(a_configurationDatabase["ws.camera.imaq.lumen.min.threshold"]);
    r_config.lumen_max_threshold = this->stringToInt(a_configurationDatabase["ws.camera.imaq.lumen.max.threshold"]);
}

void
WsConfigFacade::extractBallTransportMotorValsConfig(WsTypes::WsBallTransportMotorValueType& r_config)
{
    r_config.elevator_motor_speed_off        = this->stringToFloat(a_configurationDatabase["ws.ball.transport.elevator.off"]);
    r_config.elevator_motor_speed_up         = this->stringToFloat(a_configurationDatabase["ws.ball.transport.elevator.up"]);
    r_config.elevator_motor_speed_down       = this->stringToFloat(a_configurationDatabase["ws.ball.transport.elevator.down"]);
    r_config.escalator_motor_speed_off       = this->stringToFloat(a_configurationDatabase["ws.ball.transport.escalator.off"]);
    r_config.escalator_motor_speed_up        = this->stringToFloat(a_configurationDatabase["ws.ball.transport.escalator.up"]);
    r_config.escalator_motor_speed_down      = this->stringToFloat(a_configurationDatabase["ws.ball.transport.escalator.down"]);
    r_config.accumulator_motor_speed_off     = this->stringToFloat(a_configurationDatabase["ws.ball.transport.accumulator.off"]);
    r_config.accumulator_motor_speed_pullin  = this->stringToFloat(a_configurationDatabase["ws.ball.transport.accumulator.pull"]);
    r_config.accumulator_motor_speed_pushout = this->stringToFloat(a_configurationDatabase["ws.ball.transport.accumulator.push"]);
}

void
WsConfigFacade::extractAutoTipShootConfig(WsTypes::WsAutoTipShootProgramConfigType& r_config)
{
    r_config.ws_auto_tipshoot_coop_bridge_encoder_target                           = this->stringToInt(a_configurationDatabase["ws.auto.tipshoot.coop.bridge.encoder.target"]);
    r_config.ws_auto_tipshoot_coop_bridge_throttle                                 = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.coop.bridge.throttle"]);
    r_config.ws_auto_tipshoot_coop_bridge_heading                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.tipshoot.coop.bridge.heading" ]);
    r_config.ws_auto_tipshoot_coop_bridge_timeout                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.tipshoot.coop.bridge.timeout" ]);
    r_config.ws_auto_tipshoot_wait_for_coast_timeout                               = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.wait.for.coast.timeout"]);
    r_config.ws_auto_tipshoot_put_down_coop_bridge_timeout                         = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.put.down.coop.bridge.timeout"]);
    r_config.ws_auto_tipshoot_drive_onto_coop_bridge_encoder_target               = this->stringToInt(a_configurationDatabase["ws.auto.tipshoot.drive.onto.coop.bridge.encoder.target"]);
    r_config.ws_auto_tipshoot_drive_onto_coop_bridge_throttle                                 = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.drive.onto.coop.bridge.throttle"]);
    r_config.ws_auto_tipshoot_drive_onto_coop_bridge_heading                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.tipshoot.drive.onto.coop.bridge.heading" ]);
    r_config.ws_auto_tipshoot_drive_onto_coop_bridge_timeout                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.tipshoot.drive.onto.coop.bridge.timeout" ]);
    r_config.ws_auto_tipshoot_accumulate_balls_timeout                         = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.accumulate.balls.timeout"]);
    r_config.ws_auto_tipshoot_shooting_position_encoder_target               = this->stringToInt(a_configurationDatabase["ws.auto.tipshoot.shooting.position.encoder.target"]);
    r_config.ws_auto_tipshoot_shooting_position_throttle                                 = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.shooting.position.throttle"]);
    r_config.ws_auto_tipshoot_shooting_position_heading                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.tipshoot.shooting.position.heading" ]);
    r_config.ws_auto_tipshoot_shooting_position_timeout                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.tipshoot.shooting.position.timeout" ]);
    r_config.ws_auto_tipshoot_wait_for_angle_timeout                                        = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.wait.for.angle.timeout"]);
    r_config.ws_auto_tipshoot_angle_between_facing_and_not_facing                                        = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.angle.between.facing.and.not.facing"]);
    r_config.ws_auto_tipshoot_second_flywheel_setpoint                                        = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.second.flywheel.setpoint"]);
    r_config.ws_auto_tipshoot_wait_for_turret_timeout                           = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.wait.for.turret.timeout"]);
    r_config.shoot_state_spin_fly_time_1                                        = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.shoot.state.spin.fly.time.1"]);
    r_config.shoot_state_shoot_1_time                                           = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.shoot.state.shoot.1.time"]);
    r_config.shoot_state_spin_fly_time_2                                        = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.shoot.state.spin.fly.time.2"]);
    r_config.shoot_state_shoot_2_time                                           = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.shoot.state.shoot.2.time"]);
    r_config.shoot_state_spin_fly_time_3                                        = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.shoot.state.spin.fly.time.3"]);
    r_config.shoot_state_shoot_3_time                                           = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.shoot.state.shoot.3.time"]);
    r_config.shoot_state_spin_fly_time_4                                        = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.shoot.state.spin.fly.time.4"]);
    r_config.shoot_state_shoot_4_time                                           = this->stringToFloat(a_configurationDatabase["ws.auto.tipshoot.shoot.state.shoot.4.time"]);
}

void
WsConfigFacade::extractAutoShoot3Config(WsTypes::WsAutoShootProgramConfigType& r_config)
{
    r_config.shoot_state_prime_fly_1_time = this->stringToFloat(a_configurationDatabase["ws.auto.shoot3.state.prime1.fly.time"]);
    r_config.shoot_state_spin_fly_time_1  = this->stringToFloat(a_configurationDatabase["ws.auto.shoot3.state.spin1.fly.time"]);
    r_config.shoot_state_shoot_1_time     = this->stringToFloat(a_configurationDatabase["ws.auto.shoot3.state.shoot1.time"]);
    r_config.shoot_state_prime_fly_2_time = this->stringToFloat(a_configurationDatabase["ws.auto.shoot3.state.prime2.fly.time"]);
    r_config.shoot_state_spin_fly_time_2  = this->stringToFloat(a_configurationDatabase["ws.auto.shoot3.state.spin2.fly.time"]);
    r_config.shoot_state_shoot_2_time     = this->stringToFloat(a_configurationDatabase["ws.auto.shoot3.state.shoot2.time"]);
}

void
WsConfigFacade::extractAutoShootTipConfig(WsTypes::WsAutoShootTipProgramConfigType& r_config)
{
    r_config.ws_auto_shoottip_coop_bridge_encoder_target                           = this->stringToInt(a_configurationDatabase["ws.auto.shoottip.coop.bridge.encoder.target"]);
    r_config.ws_auto_shoottip_coop_bridge_throttle                                 = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.coop.bridge.throttle"]);
    r_config.ws_auto_shoottip_coop_bridge_heading                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.shoottip.coop.bridge.heading" ]);
    r_config.ws_auto_shoottip_coop_bridge_timeout                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.shoottip.coop.bridge.timeout" ]);
    r_config.ws_auto_shoottip_wait_for_coast_timeout                               = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.wait.for.coast.timeout"]);
    r_config.ws_auto_shoottip_put_down_coop_bridge_timeout                         = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.put.down.coop.bridge.timeout"]);
    r_config.ws_auto_shoottip_drive_onto_coop_bridge_encoder_target               = this->stringToInt(a_configurationDatabase["ws.auto.shoottip.drive.onto.coop.bridge.encoder.target"]);
    r_config.ws_auto_shoottip_drive_onto_coop_bridge_throttle                                 = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.drive.onto.coop.bridge.throttle"]);
    r_config.ws_auto_shoottip_drive_onto_coop_bridge_heading                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.shoottip.drive.onto.coop.bridge.heading" ]);
    r_config.ws_auto_shoottip_drive_onto_coop_bridge_timeout                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.shoottip.drive.onto.coop.bridge.timeout" ]);
    r_config.ws_auto_shoottip_accumulate_balls_timeout                         = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.accumulate.balls.timeout"]);
    r_config.ws_auto_shoottip_shooting_position_encoder_target               = this->stringToInt(a_configurationDatabase["ws.auto.shoottip.shooting.position.encoder.target"]);
    r_config.ws_auto_shoottip_shooting_position_throttle                                 = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.shooting.position.throttle"]);
    r_config.ws_auto_shoottip_shooting_position_heading                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.shoottip.shooting.position.heading" ]);
    r_config.ws_auto_shoottip_shooting_position_timeout                            = this->stringToFloat(a_configurationDatabase[ "ws.auto.shoottip.shooting.position.timeout" ]);
    r_config.ws_auto_shoottip_wait_for_angle_timeout                                        = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.wait.for.angle.timeout"]);
    r_config.ws_auto_shoottip_angle_between_facing_and_not_facing                                        = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.angle.between.facing.and.not.facing"]);
    r_config.ws_auto_shoottip_second_flywheel_setpoint                                        = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.second.flywheel.setpoint"]);
    r_config.ws_auto_shoottip_wait_for_turret_timeout                           = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.wait.for.turret.timeout"]);
    r_config.shoot_state_spin_fly_time_1                                        = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.shoot.state.spin.fly.time.1"]);
    r_config.shoot_state_shoot_1_time                                           = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.shoot.state.shoot.1.time"]);
    r_config.shoot_state_spin_fly_time_2                                        = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.shoot.state.spin.fly.time.2"]);
    r_config.shoot_state_shoot_2_time                                           = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.shoot.state.shoot.2.time"]);
    r_config.shoot_state_spin_fly_time_3                                        = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.shoot.state.spin.fly.time.3"]);
    r_config.shoot_state_shoot_3_time                                           = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.shoot.state.shoot.3.time"]);
    r_config.shoot_state_spin_fly_time_4                                        = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.shoot.state.spin.fly.time.4"]);
    r_config.shoot_state_shoot_4_time                                           = this->stringToFloat(a_configurationDatabase["ws.auto.shoottip.shoot.state.shoot.4.time"]);
    
}

void
WsConfigFacade::extractShooterPresetConfig(WsTypes::WsShooterPresetConfigT& r_config)
{
    r_config.turret_key_forward_voltage      = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.facing.turret.heading.setpoint"]);
    r_config.flywheel_key_forward_speed      = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.facing.flywheel.speed.setpoint"]);
    r_config.flywheel_key_forward_motor_val  = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.facing.flywheel.raw.motor.output"]);
    r_config.turret_key_reverse_voltage      = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.reverse.turret.heading.setpoint"]);
    r_config.flywheel_key_reverse_speed      = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.reverse.flywheel.speed.setpoint"]);
    r_config.flywheel_key_reverse_motor_val  = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.reverse.flywheel.raw.motor.output"]);
    r_config.turret_fender_left_voltage      = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.fender.left.turret.heading.setpoint"]);
    r_config.flywheel_fender_left_speed      = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.fender.left.flywheel.speed.setpoint"]);
    r_config.flywheel_fender_left_motor_val  = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.fender.left.flywheel.raw.motor.output"]);
    r_config.turret_fender_right_voltage     = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.fender.right.turret.heading.setpoint"]);
    r_config.flywheel_fender_right_speed     = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.fender.right.flywheel.speed.setpoint"]);
    r_config.flywheel_fender_right_motor_val = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.fender.right.flywheel.raw.motor.output"]);
    r_config.turret_auton_voltage            = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.auto.turret.heading.setpoint"]);
    r_config.flywheel_auton_speed            = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.auto.flywheel.speed.setpoint"]);
    r_config.flywheel_auton_motor_val        = this->stringToFloat(a_configurationDatabase["ws.shooter.preset.auto.flywheel.raw.motor.output"]);
}

void WsConfigFacade::extractAutoDriveProgramConfig(WsTypes::WsAutoDriveProgramConfigType& r_config)
{
    r_config.ws_auto_drive_coop_bridge_encoder_target                 = this->stringToInt(a_configurationDatabase["ws.auto.drive.coop.bridge.encoder.target"]);
    r_config.ws_auto_drive_coop_bridge_throttle                       = this->stringToFloat(a_configurationDatabase["ws.auto.drive.coop.bridge.throttle"]);
    r_config.ws_auto_drive_coop_bridge_heading                        = this->stringToFloat(a_configurationDatabase[ "ws.auto.drive.coop.bridge.heading" ]);
    r_config.ws_auto_drive_coop_bridge_timeout                        = this->stringToFloat(a_configurationDatabase[ "ws.auto.drive.coop.bridge.timeout" ]);
    r_config.ws_auto_drive_wait_for_coast_timeout                     = this->stringToFloat(a_configurationDatabase["ws.auto.drive.wait.for.coast.timeout"]);
    r_config.ws_auto_drive_put_down_coop_bridge_timeout               = this->stringToFloat(a_configurationDatabase["ws.auto.drive.put.down.coop.bridge.timeout"]);
    r_config.ws_auto_drive_shooting_position_encoder_target           = this->stringToInt(a_configurationDatabase["ws.auto.drive.shooting.position.encoder.target"]);
    r_config.ws_auto_drive_shooting_position_throttle                 = this->stringToFloat(a_configurationDatabase["ws.auto.drive.shooting.position.throttle"]);
    r_config.ws_auto_drive_shooting_position_heading                  = this->stringToFloat(a_configurationDatabase[ "ws.auto.drive.shooting.position.heading" ]);
    r_config.ws_auto_drive_shooting_position_timeout                  = this->stringToFloat(a_configurationDatabase[ "ws.auto.drive.shooting.position.timeout" ]);
    r_config.ws_auto_drive_wait_for_angle_timeout                                        = this->stringToFloat(a_configurationDatabase["ws.auto.drive.wait.for.angle.timeout"]);
    r_config.ws_auto_drive_angle_between_facing_and_not_facing                                        = this->stringToFloat(a_configurationDatabase["ws.auto.drive.angle.between.facing.and.not.facing"]);
    r_config.ws_auto_drive_second_flywheel_setpoint                                        = this->stringToFloat(a_configurationDatabase["ws.auto.drive.second.flywheel.setpoint"]);
    
    r_config.ws_auto_drive_wait_for_turret_timeout                           = this->stringToFloat(a_configurationDatabase["ws.auto.drive.wait.for.turret.timeout"]);
    r_config.shoot_state_spin_fly_time_1                              = this->stringToFloat(a_configurationDatabase["ws.auto.drive.shoot.state.spin.fly.time.1"]);
    r_config.shoot_state_shoot_1_time                                 = this->stringToFloat(a_configurationDatabase["ws.auto.drive.shoot.state.shoot.1.time"]);
    r_config.shoot_state_spin_fly_time_2                              = this->stringToFloat(a_configurationDatabase["ws.auto.drive.shoot.state.spin.fly.time.2"]);
    r_config.shoot_state_shoot_2_time                                 = this->stringToFloat(a_configurationDatabase["ws.auto.drive.shoot.state.shoot.2.time"]);
    r_config.shoot_state_spin_fly_time_3                              = this->stringToFloat(a_configurationDatabase["ws.auto.drive.shoot.state.spin.fly.time.3"]);
    r_config.shoot_state_shoot_3_time                                 = this->stringToFloat(a_configurationDatabase["ws.auto.drive.shoot.state.shoot.3.time"]);
}

void
WsConfigFacade::dumpDatabaseToLog(void)
{
    WS_LOG_NOTICE("BEGIN CONFIG DATABASE DUMP");
    WsConfigMapType::iterator iter = a_configurationDatabase.begin();
    WsConfigMapType::iterator end_iter = a_configurationDatabase.end();
    
    std::list<std::string> key_list;
    for (; iter != end_iter; iter++)
    {
        key_list.push_back(iter->first);
    }
    key_list.sort();
    
    // Print out all key -> value pairs
    std::list<std::string>::iterator key_list_iter = key_list.begin();
    std::list<std::string>::iterator key_list_end_iter = key_list.end();
    for (; key_list_iter != key_list_end_iter; key_list_iter++)
    {
        std::string key = *key_list_iter;
        std::string value = a_configurationDatabase[key];
        WS_LOG_NOTICE("'%s' --> '%s'", key.c_str(), value.c_str());
    }
    WS_LOG_NOTICE("END CONFIG DATABASE DUMP");
}

WsSubject*
WsConfigFacade::getAutoFenderRightConfig(WsTypes::WsAutoFenderRightProgramConfigType& r_config)
{
    ap_autoFenderRightProgramConfigElement->getState(r_config);
    return ap_autoFenderRightProgramConfigElement;
}

void
WsConfigFacade::setAutoFenderRightConfig(const WsTypes::WsAutoFenderRightProgramConfigType& rc_config)
{
    ap_autoFenderRightProgramConfigElement->setState(rc_config);
}

void
WsConfigFacade::extractAutoFenderRightProgramConfig(WsTypes::WsAutoFenderRightProgramConfigType& r_config)
{
    r_config.ws_auto_fender_right_drive_fender_throttle               = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.drive.fender.throttle"]);
    r_config.ws_auto_fender_right_drive_fender_heading                = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.drive.fender.heading"]);
    r_config.ws_auto_fender_right_drive_fender_encoder_target         = this->stringToInt(a_configurationDatabase["ws.auto.fender.right.drive.fender.encoder.target"]);
    r_config.ws_auto_fender_right_drive_fender_timeout                = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.drive.fender.timeout"]);
    r_config.ws_auto_fender_right_square_up_timeout                   = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.square.up.timeout"]);
    r_config.ws_auto_fender_right_square_up_throttle                  = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.square.up.throttle"]);
    r_config.ws_auto_fender_right_coast_timeout                       = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.coast.timeout"]);
    r_config.ws_auto_fender_right_shoot_first_timeout                 = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.shoot.first.timeout"]);
    r_config.ws_auto_fender_right_wait_for_flywheel_timeout           = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.wait.for.flywheel.timeout"]);
    r_config.ws_auto_fender_right_shoot_second_timeout                = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.shoot.second.timeout"]);
    r_config.ws_auto_fender_right_back_up_throttle                    = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.back.up.throttle"]);
    r_config.ws_auto_fender_right_back_up_heading                     = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.back.up.heading"]);
    r_config.ws_auto_fender_right_back_up_encoder_target              = this->stringToInt(a_configurationDatabase["ws.auto.fender.right.back.up.encoder.target"]);
    r_config.ws_auto_fender_right_back_up_timeout                     = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.back.up.timeout"]);
    r_config.ws_auto_fender_right_turn_throttle                       = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.turn.throttle"]);
    r_config.ws_auto_fender_right_turn_heading                        = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.turn.heading"]);
    r_config.ws_auto_fender_right_turn_encoder_target                 = this->stringToInt(a_configurationDatabase["ws.auto.fender.right.turn.encoder.target"]);
    r_config.ws_auto_fender_right_turn_timeout                        = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.turn.timeout"]);
    r_config.ws_auto_fender_right_wait_for_turn_timeout               = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.wait.for.turn.timeout"]);
    r_config.ws_auto_fender_right_drive_alliance_throttle             = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.drive.alliance.throttle"]);
    r_config.ws_auto_fender_right_drive_alliance_heading              = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.drive.alliance.heading"]);
    r_config.ws_auto_fender_right_drive_alliance_encoder_target       = this->stringToInt(a_configurationDatabase["ws.auto.fender.right.drive.alliance.encoder.target"]);
    r_config.ws_auto_fender_right_drive_alliance_timeout              = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.drive.alliance.timeout"]);
    r_config.ws_auto_fender_right_coast_alliance_timeout              = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.coast.alliance.timeout"]);
    r_config.ws_auto_fender_right_dump_alliance_timeout               = this->stringToFloat(a_configurationDatabase["ws.auto.fender.right.dump.alliance.timeout"]);
}

void
WsConfigFacade::extractAutoFenderLeftProgramConfig(WsTypes::WsAutoFenderLeftProgramConfigType& r_config)
{
    r_config.ws_auto_fender_left_drive_fender_throttle           = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.drive.fender.throttle"]);
    r_config.ws_auto_fender_left_drive_fender_heading            = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.drive.fender.heading"]);
    r_config.ws_auto_fender_left_drive_fender_encoder_target     = this->stringToInt(a_configurationDatabase["ws.auto.fender.left.drive.fender.encoder.target"]);
    r_config.ws_auto_fender_left_drive_fender_timeout            = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.drive.fender.timeout"]);
    r_config.ws_auto_fender_left_square_up_timeout                   = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.square.up.timeout"]);
    r_config.ws_auto_fender_left_square_up_throttle                  = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.square.up.throttle"]);
    r_config.ws_auto_fender_left_coast_timeout                   = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.coast.timeout"]);
    r_config.ws_auto_fender_left_shoot_first_timeout             = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.shoot.first.timeout"]);
    r_config.ws_auto_fender_left_wait_for_flywheel_timeout       = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.wait.for.flywheel.timeout"]);
    r_config.ws_auto_fender_left_shoot_second_timeout            = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.shoot.second.timeout"]);
    
    r_config.ws_auto_fender_left_back_up_throttle                    = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.back.up.throttle"]);
    r_config.ws_auto_fender_left_back_up_heading                     = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.back.up.heading"]);
    r_config.ws_auto_fender_left_back_up_encoder_target              = this->stringToInt(a_configurationDatabase["ws.auto.fender.left.back.up.encoder.target"]);
    r_config.ws_auto_fender_left_back_up_timeout                     = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.back.up.timeout"]);
    r_config.ws_auto_fender_left_turn_throttle                       = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.turn.throttle"]);
    r_config.ws_auto_fender_left_turn_heading                        = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.turn.heading"]);
    r_config.ws_auto_fender_left_turn_encoder_target                 = this->stringToInt(a_configurationDatabase["ws.auto.fender.left.turn.encoder.target"]);
    r_config.ws_auto_fender_left_turn_timeout                        = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.turn.timeout"]);
    r_config.ws_auto_fender_left_wait_for_turn_timeout               = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.wait.for.turn.timeout"]);
    r_config.ws_auto_fender_left_drive_barrier_throttle             = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.drive.barrier.throttle"]);
    r_config.ws_auto_fender_left_drive_barrier_heading              = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.drive.barrier.heading"]);
    r_config.ws_auto_fender_left_drive_barrier_encoder_target       = this->stringToInt(a_configurationDatabase["ws.auto.fender.left.drive.barrier.encoder.target"]);
    r_config.ws_auto_fender_left_drive_barrier_timeout              = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.drive.barrier.timeout"]);
    r_config.ws_auto_fender_left_coast_barrier_timeout              = this->stringToFloat(a_configurationDatabase["ws.auto.fender.left.coast.barrier.timeout"]);
    
    
    
}

WsSubject*
WsConfigFacade::getAutoFenderLeftConfig(WsTypes::WsAutoFenderLeftProgramConfigType& r_config)
{
    ap_autoFenderLeftProgramConfigElement->getState(r_config);
    return ap_autoFenderLeftProgramConfigElement;
}

void
WsConfigFacade::setAutoFenderLeftConfig(const WsTypes::WsAutoFenderLeftProgramConfigType& rc_config)
{
    ap_autoFenderLeftProgramConfigElement->setState(rc_config);
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

