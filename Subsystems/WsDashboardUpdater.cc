#include "WsDashboardUpdater.hh"
#include "WsDashboardUpdaterObservers.hh"
#include "WsLogger.hh"
#include "WsInputFacade.hh"

#include "Dashboard.h"
#include "DriverStation.h"
#include "Timer.h"

WsDashboardUpdater::WsDashboardUpdater(void)
    : WsSubsystem("WsDashboardUpdater")

    // Turret
    , a_turretAtPositionFlag(false)
    , a_turretNearPositionFlag(false)
    , a_turretMotorOutput(0.0f)
    , a_turretHeadingAngle(0.0f)
    , a_turretHeadingPotValue(0.0f)
    , a_turretPidSetpoint(0.0f)
    , a_turretPidEnabledFlag(false)
    , a_turretPidConfig()
    , a_robotHeadingGyroAngle(0.0f)

    // Autonomous
    , a_automonousLockInState(false)
    , a_autonomousProgramId(0)
    , a_lockedAutonomousProgramId(0)

    // Flywheel
    , a_flywheelAtSpeedFlag(false)
    , a_flywheelNearSpeedFlag(false)
    , a_flywheelRotationalSpeed(0.0f)
    , a_flywheelPidSetpoint(0.0f)
    , a_flywheelPidEnabledFlag(false)
    , a_flywheelMotorOutput(0.0f)
    , a_flywheelPidControllerOutput(0.0f)
    , a_flywheelFloodLightState(false)
    , a_flywheelFreshImageFlag(false)
    , a_flywheelLocationSpeedMapping()
    , a_flywheelManualSpeedPotValue(0.0f)
    , a_flywheelControlStatePotValue(0.0f)
    , a_flywheelPidConfig()
    , a_flywheelCompensationSwitchState(false)
    , a_flywheelCompensationPotValue(0.0f)

    // Vision
    , a_topTargetImageInfo()
    , a_leftTargetImageInfo()
    , a_rightTargetImageInfo()
    , a_bottomTargetImageInfo()
    , a_numVisibleTargets(0)

    // Drive Base
    , a_leftEncoderCount(0)
    , a_rightEncoderCount(0)
    , a_leftDriveMotorOutput(0.0f)
    , a_rightDriveMotorOutput(0.0f)
    , a_shifterState(WsTypes::WS_S_LOWGEAR)
    , a_pogoState(WsTypes::WS_POGO_STICK_CYLINDER_UP)
    , a_brakeState(WsTypes::WS_BRIDGE_BRAKE_CYLINDER_DISENGAGED)
    , a_robotGrabberState(WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED)
    , a_antiTurboEnabled(false)
    , a_driverThrottleValue(0.0f)
    , a_driverHeadingValue(0.0f)
    , a_driverAntiTurboButtonState(false)
    , a_driverShifterButtonState(false)
    , a_driveBaseAccelerationButtonState(false)
    , a_driveBaseAccelerationEnabledFlag(true)

    // Compressor
    , a_compressorState(WsTypes::WS_COMPRESSOR_OFF)
    , a_pneumaticPressureReading(0.0f)

    // Shooter Preset
    , a_shooterPresetConfig()

    // Turret Observers
    , ap_turretAtPositionObserver(NULL)
    , ap_turretNearPositionObserver(NULL)
    , ap_turretMotorOutputObserver(NULL)
    , ap_turretHeadingAngleObserver(NULL)
    , ap_turretHeadingPotValueObserver(NULL)
    , ap_turretPidSetpointObserver(NULL)
    , ap_turretPidEnabledObserver(NULL)
    , ap_turretPidConfigObserver(NULL)
    , ap_robotHeadingGyroAngleObserver(NULL)

    // Autonomous Observers
    , ap_autonomousLockInStateObserver(NULL)
    , ap_autonomousProgramIdObserver(NULL)
    , ap_lockedAutonomousProgramIdObserver(NULL)

    // Flywheel Observers
    , ap_flywheelAtSpeedObserver(NULL)
    , ap_flywheelNearSpeedObserver(NULL)
    , ap_flywheelRotationalSpeedObserver(NULL)
    , ap_flywheelPidSetpointObserver(NULL)
    , ap_flywheelPidEnabledObserver(NULL)
    , ap_flywheelMotorOutputObserver(NULL)
    , ap_flywheelPidControllerOutputObserver(NULL)
    , ap_flywheelFloodLightStateObserver(NULL)
    , ap_flywheelFreshImageFlagObserver(NULL)
    , ap_flywheelLocationSpeedMappingObserver(NULL)
    , ap_flywheelManualSpeedPotValueObserver(NULL)
    , ap_flywheelControlPotValueObserver(NULL)
    , ap_flywheelPidConfigObserver(NULL)
    , ap_flywheelCompensationSwitchStateObserver(NULL)
    , ap_flywheelCompensationPotValueObserver(NULL)

    // Vision Observers
    , ap_visionNumVisibleTargetsObserver(NULL)
    // @TODO Top Target Observer
    // @TODO Left Target Observer
    // @TODO Right Target Observer
    // @TODO Bottom Target Observer

    // DriveBase
    , ap_leftDriveEncoderObserver(NULL)
    , ap_rightDriveEncoderObserver(NULL)
    , ap_leftDriveMotorOutputObserver(NULL)
    , ap_rightDriveMotorOutputObserver(NULL)
    , ap_shifterStateObserver(NULL)
    , ap_antiTurboEnabledObserver(NULL)
    , ap_driverThrottleValueObserver(NULL)
    , ap_driverHeadingValueObserver(NULL)
    , ap_driverAntiTurboButtonStateObserver(NULL)
    , ap_driverShifterButtonStateObserver(NULL)
    , ap_driveBaseAccelerationButtonStateObserver(NULL)
    , ap_driveBaseAccelerationEnabledObserver(NULL)
    , ap_pogoStickStateObserver(NULL)
    , ap_bridgeBrakeStateObserver(NULL)
    , ap_robotGrabberStateObserver(NULL)

    // Compressor
    , ap_compressorStateObserver(NULL)
    , ap_pneumaticPressureReadingObserver(NULL)

    // Shooter Preset
    , ap_shooterPresetConfigObserver(NULL)

    // Timer
    , ap_dataSendTimer(NULL)
{
    WS_LOG_TRACE("Begin");
    
    ap_dataSendTimer = new Timer();
    ap_dataSendTimer->Reset();
    ap_dataSendTimer->Start();
    
    // Observers must be the last objects created in the ctor()...
    
    // Turret
    ap_turretAtPositionObserver                 = new WsDashboardUpdaterTurretAtPositionObserver(this);
    ap_turretNearPositionObserver               = new WsDashboardUpdaterTurretNearPositionObserver(this);
    ap_turretMotorOutputObserver                = new WsDashboardUpdaterTurretMotorOutputObserver(this);
    ap_turretHeadingAngleObserver               = new WsDashboardUpdaterTurretHeadingAngleObserver(this);
    ap_turretHeadingPotValueObserver            = new WsDashboardUpdaterTurretHeadingPotValueObserver(this);
    ap_turretPidSetpointObserver                = new WsDashboardUpdaterTurretPidSetpointObserver(this);
    ap_turretPidEnabledObserver                 = new WsDashboardUpdaterTurretPidEnabledObserver(this);
    ap_turretPidConfigObserver                  = new WsDashboardUpdaterTurretPidConfigObserver(this);
    ap_robotHeadingGyroAngleObserver            = new WsDashboardUpdaterRobotHeadingGyroAngleObserver(this);
    
    // Autonomous
    ap_autonomousLockInStateObserver            = new WsDashboardUpdaterAutoProgramLockInObserver(this);
    ap_autonomousProgramIdObserver              = new WsDashboardUpdaterAutoProgramObserver(this);
    ap_lockedAutonomousProgramIdObserver        = new WsDashboardUpdaterLockedAutoProgramIdObserver(this);
    
    // Flywheel
    ap_flywheelAtSpeedObserver                  = new WsDashboardUpdaterFlywheelAtSpeedObserver(this);
    ap_flywheelNearSpeedObserver                = new WsDashboardUpdaterFlywheelNearSpeedObserver(this);
    ap_flywheelRotationalSpeedObserver          = new WsDashboardUpdaterFlywheelRotationalSpeedObserver(this);
    ap_flywheelPidSetpointObserver              = new WsDashboardUpdaterFlywheelPidSetpointObserver(this);
    ap_flywheelPidEnabledObserver               = new WsDashboardUpdaterFlywheelPidEnabledObserver(this);
    ap_flywheelMotorOutputObserver              = new WsDashboardUpdaterFlywheelMotorOutputObserver(this);
    ap_flywheelPidControllerOutputObserver      = new WsDashboardUpdaterFlywheelPidControllerOutputObserver(this);
    ap_flywheelFloodLightStateObserver          = new WsDashboardUpdaterFloodLightStateObserver(this);
    ap_flywheelFreshImageFlagObserver           = new WsDashboardUpdaterFreshImageObserver(this);
    ap_flywheelLocationSpeedMappingObserver     = new WsDashboardUpdaterLocationSpeedMappingObserver(this);
    ap_flywheelManualSpeedPotValueObserver      = new WsDashboardUpdaterFlywheelManualPotObserver(this);
    ap_flywheelControlPotValueObserver          = new WsDashboardUpdaterFlywheelStatePotObserver(this);
    ap_flywheelPidConfigObserver                = new WsDashboardUpdaterFlywheelPidConfigObserver(this);
    ap_flywheelCompensationSwitchStateObserver  = new WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver(this);
    ap_flywheelCompensationPotValueObserver     = new WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver(this);
    
    // Vision
    ap_visionNumVisibleTargetsObserver          = new WsDashboardUpdaterVisionNumVisibleTargetsObserver(this);
    
    // Drive Base
    ap_leftDriveEncoderObserver                 = new WsDashboardUpdaterDriveBaseLeftEncoderObserver(this);
    ap_rightDriveEncoderObserver                = new WsDashboardUpdaterDriveBaseRightEncoderObserver(this);
    ap_leftDriveMotorOutputObserver             = new WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver(this);
    ap_rightDriveMotorOutputObserver            = new WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver(this);
    ap_shifterStateObserver                     = new WsDashboardUpdaterDriveBaseShifterStateObserver(this);
    ap_antiTurboEnabledObserver                 = new WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver(this);
    ap_driverThrottleValueObserver              = new WsDashboardUpdaterDriveBaseDriverThrottleValueObserver(this);
    ap_driverHeadingValueObserver               = new WsDashboardUpdaterDriveBaseDriverHeadingValueObserver(this);
    ap_driverAntiTurboButtonStateObserver       = new WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver(this);
    ap_driverShifterButtonStateObserver         = new WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver(this);
    ap_driveBaseAccelerationButtonStateObserver = new WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver(this);
    ap_driveBaseAccelerationEnabledObserver     = new WsDashboardUpdaterDriveBaseAccelerationEnabledObserver(this);
    ap_pogoStickStateObserver                   = new WsDashboardUpdaterDriveBasePogoEnabledObserver(this);
    ap_bridgeBrakeStateObserver                 = new WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver(this);
    ap_robotGrabberStateObserver                = new WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver(this);
    
    // Compressor
    ap_compressorStateObserver                  = new WsDashboardUpdaterCompressorStateObserver(this);
    ap_pneumaticPressureReadingObserver         = new WsDashboardUpdaterPneumaticPressureReadingObserver(this);
    
    // Shooter Preset
    ap_shooterPresetConfigObserver              = new WsDashboardUpdaterShooterPresetConfigObserver(this);
}

WsDashboardUpdater::~WsDashboardUpdater(void)
{
    WS_LOG_TRACE("Begin");
    
    // Turret
    delete ap_turretAtPositionObserver;                  ap_turretAtPositionObserver = NULL;
    delete ap_turretNearPositionObserver;                ap_turretNearPositionObserver = NULL;
    delete ap_turretMotorOutputObserver;                 ap_turretMotorOutputObserver = NULL;
    delete ap_turretHeadingAngleObserver;                ap_turretHeadingAngleObserver = NULL;
    delete ap_turretHeadingPotValueObserver;             ap_turretHeadingPotValueObserver = NULL;
    delete ap_turretPidSetpointObserver;                 ap_turretPidSetpointObserver = NULL;
    delete ap_turretPidEnabledObserver;                  ap_turretPidEnabledObserver = NULL;
    delete ap_turretPidConfigObserver;                   ap_turretPidConfigObserver = NULL;
    delete ap_robotHeadingGyroAngleObserver;             ap_robotHeadingGyroAngleObserver = NULL;
    
    // Autonomous
    delete ap_autonomousLockInStateObserver;             ap_autonomousLockInStateObserver = NULL;
    delete ap_autonomousProgramIdObserver;               ap_autonomousProgramIdObserver = NULL;
    delete ap_lockedAutonomousProgramIdObserver;         ap_lockedAutonomousProgramIdObserver = NULL;
    
    // Flywheel
    delete ap_flywheelAtSpeedObserver;                   ap_flywheelAtSpeedObserver = NULL;
    delete ap_flywheelNearSpeedObserver;                 ap_flywheelNearSpeedObserver = NULL;
    delete ap_flywheelRotationalSpeedObserver;           ap_flywheelRotationalSpeedObserver = NULL;
    delete ap_flywheelPidSetpointObserver;               ap_flywheelPidSetpointObserver = NULL;
    delete ap_flywheelPidEnabledObserver;                ap_flywheelPidEnabledObserver = NULL;
    delete ap_flywheelMotorOutputObserver;               ap_flywheelMotorOutputObserver = NULL;
    delete ap_flywheelPidControllerOutputObserver;       ap_flywheelPidControllerOutputObserver = NULL;
    delete ap_flywheelFloodLightStateObserver;           ap_flywheelFloodLightStateObserver = NULL;
    delete ap_flywheelFreshImageFlagObserver;            ap_flywheelFreshImageFlagObserver = NULL;
    delete ap_flywheelLocationSpeedMappingObserver;      ap_flywheelLocationSpeedMappingObserver = NULL;
    delete ap_flywheelManualSpeedPotValueObserver;       ap_flywheelManualSpeedPotValueObserver = NULL;
    delete ap_flywheelControlPotValueObserver;           ap_flywheelControlPotValueObserver = NULL;
    delete ap_flywheelPidConfigObserver;                 ap_flywheelPidConfigObserver = NULL;
    delete ap_flywheelCompensationSwitchStateObserver;   ap_flywheelCompensationSwitchStateObserver = NULL;
    delete ap_flywheelCompensationPotValueObserver;      ap_flywheelCompensationPotValueObserver = NULL;
    
    // Vision
    delete ap_visionNumVisibleTargetsObserver;           ap_visionNumVisibleTargetsObserver = NULL;
    // @TODO Top
    // @TODO Left
    // @TODO Right
    // @TODO Bottom
    
    // Drive Base
    delete ap_leftDriveEncoderObserver;                  ap_leftDriveEncoderObserver = NULL;
    delete ap_rightDriveEncoderObserver;                 ap_rightDriveEncoderObserver = NULL;
    delete ap_leftDriveMotorOutputObserver;              ap_leftDriveMotorOutputObserver = NULL;
    delete ap_rightDriveMotorOutputObserver;             ap_rightDriveMotorOutputObserver = NULL;
    delete ap_shifterStateObserver;                      ap_shifterStateObserver = NULL;
    delete ap_antiTurboEnabledObserver;                  ap_antiTurboEnabledObserver = NULL;
    delete ap_driverThrottleValueObserver;               ap_driverThrottleValueObserver = NULL;
    delete ap_driverHeadingValueObserver;                ap_driverHeadingValueObserver = NULL;
    delete ap_driverAntiTurboButtonStateObserver;        ap_driverAntiTurboButtonStateObserver = NULL;
    delete ap_driverShifterButtonStateObserver;          ap_driverShifterButtonStateObserver = NULL;
    delete ap_driveBaseAccelerationButtonStateObserver;  ap_driveBaseAccelerationButtonStateObserver = NULL;
    delete ap_driveBaseAccelerationEnabledObserver;      ap_driveBaseAccelerationEnabledObserver = NULL;
    delete ap_pogoStickStateObserver;                    ap_pogoStickStateObserver = NULL;
    delete ap_bridgeBrakeStateObserver;                  ap_bridgeBrakeStateObserver = NULL;
    delete ap_robotGrabberStateObserver;                 ap_robotGrabberStateObserver = NULL;
    
    // Compressor
    delete ap_compressorStateObserver;                   ap_compressorStateObserver = NULL;
    delete ap_pneumaticPressureReadingObserver;          ap_pneumaticPressureReadingObserver = NULL;
    
    // Shooter Preset
    delete ap_shooterPresetConfigObserver;               ap_shooterPresetConfigObserver = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsDashboardUpdater::update(void)
{
    WS_LOG_TRACE("Begin");
    
    // Only send data every so often
    if (false == ap_dataSendTimer->HasPeriodPassed(0.250))
    { return; }
    
    // Get a handle to the packet data "assembler"
    Dashboard* p_packer = &(DriverStation::GetInstance()->GetLowPriorityDashboardPacker());
    
    // Pack our custom data structure
    p_packer->AddCluster();
    {
        // BEGIN:  Turret
        p_packer->AddCluster();
        {
            p_packer->AddBoolean(a_turretAtPositionFlag);
            p_packer->AddBoolean(a_turretNearPositionFlag);
            p_packer->AddFloat(a_turretMotorOutput);
            p_packer->AddFloat(a_turretHeadingPotValue);
            p_packer->AddFloat(a_turretPidSetpoint);
            p_packer->AddBoolean(a_turretPidEnabledFlag);
            p_packer->AddFloat(a_robotHeadingGyroAngle);
            
            p_packer->AddCluster();
            {
                p_packer->AddFloat(a_turretPidConfig.k_p);
                p_packer->AddFloat(a_turretPidConfig.k_i);
                p_packer->AddFloat(a_turretPidConfig.k_d);
                p_packer->AddFloat(a_turretPidConfig.error_epsilon);
                p_packer->AddFloat(a_turretPidConfig.static_epsilon);
                p_packer->AddFloat(a_turretPidConfig.max_integral);
                p_packer->AddFloat(a_turretPidConfig.error_increment);
                p_packer->AddFloat(a_turretPidConfig.integral_error_thresh);
                p_packer->AddFloat(a_turretPidConfig.differentiator_band_limit);
                p_packer->AddFloat(a_turretPidConfig.max_output);
                p_packer->AddFloat(a_turretPidConfig.min_output);
                p_packer->AddFloat(a_turretPidConfig.max_input);
                p_packer->AddFloat(a_turretPidConfig.min_input);
                p_packer->AddFloat(a_turretPidConfig.min_on_target_time);
            }
            p_packer->FinalizeCluster();
        }
        p_packer->FinalizeCluster();
        // BEGIN:  Turret
        
        
        // BEGIN:  Auto program
        p_packer->AddCluster();
        {
            p_packer->AddBoolean(a_automonousLockInState);
            p_packer->AddI32(a_autonomousProgramId);
            p_packer->AddI32(a_lockedAutonomousProgramId);
        }
        p_packer->FinalizeCluster();
        // END:  Auto program
        
        
        // BEGIN:  Flywheel
        p_packer->AddCluster();
        {
            p_packer->AddBoolean(a_flywheelAtSpeedFlag);
            p_packer->AddBoolean(a_flywheelNearSpeedFlag);
            p_packer->AddFloat(a_flywheelRotationalSpeed);
            p_packer->AddFloat(a_flywheelPidSetpoint);
            p_packer->AddBoolean(a_flywheelPidEnabledFlag);
            p_packer->AddFloat(a_flywheelMotorOutput);
            p_packer->AddFloat(a_flywheelPidControllerOutput);
            p_packer->AddBoolean(a_flywheelFloodLightState);
            p_packer->AddBoolean(a_flywheelFreshImageFlag);
            
            p_packer->AddCluster();
            {
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.fender_distance);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.fender_fws);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.fender_speed);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.key_bottom_distance);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.key_bottom_fws);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.key_bottom_speed);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.key_middle_distance);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.key_middle_fws);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.key_middle_speed);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.key_top_distance);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.key_top_fws);
                p_packer->AddFloat(a_flywheelLocationSpeedMapping.key_top_speed);
            }
            p_packer->FinalizeCluster();
            
            p_packer->AddFloat(a_flywheelManualSpeedPotValue);
            p_packer->AddFloat(a_flywheelControlStatePotValue);
            
            p_packer->AddCluster();
            {
                p_packer->AddFloat(a_flywheelPidConfig.k_p);
                p_packer->AddFloat(a_flywheelPidConfig.k_i);
                p_packer->AddFloat(a_flywheelPidConfig.k_d);
                p_packer->AddFloat(a_flywheelPidConfig.error_epsilon);
                p_packer->AddFloat(a_flywheelPidConfig.static_epsilon);
                p_packer->AddFloat(a_flywheelPidConfig.max_integral);
                p_packer->AddFloat(a_flywheelPidConfig.error_increment);
                p_packer->AddFloat(a_flywheelPidConfig.integral_error_thresh);
                p_packer->AddFloat(a_flywheelPidConfig.differentiator_band_limit);
                p_packer->AddFloat(a_flywheelPidConfig.max_output);
                p_packer->AddFloat(a_flywheelPidConfig.min_output);
                p_packer->AddFloat(a_flywheelPidConfig.max_input);
                p_packer->AddFloat(a_flywheelPidConfig.min_input);
                p_packer->AddFloat(a_flywheelPidConfig.min_on_target_time);
            }
            p_packer->FinalizeCluster();
            
            p_packer->AddBoolean(a_flywheelCompensationSwitchState);
            p_packer->AddFloat(a_flywheelCompensationPotValue);
        }
        p_packer->FinalizeCluster();
        // END:  Flywheel
        
        
        // BEGIN:  Vision
        p_packer->AddCluster();
        {
            // Top Target
            p_packer->AddCluster();
            {
                p_packer->AddI32(a_topTargetImageInfo.cm_x);
                p_packer->AddI32(a_topTargetImageInfo.cm_y);
                p_packer->AddI32(a_topTargetImageInfo.target_area);
                p_packer->AddFloat(a_topTargetImageInfo.compactness);
                p_packer->AddI32(a_topTargetImageInfo.br_left);
                p_packer->AddI32(a_topTargetImageInfo.br_top);
                p_packer->AddI32(a_topTargetImageInfo.br_width);
                p_packer->AddI32(a_topTargetImageInfo.br_height);
                p_packer->AddBoolean(a_topTargetImageInfo.has_target);
            }
            p_packer->FinalizeCluster();
            
            // Left Target
            p_packer->AddCluster();
            {
                p_packer->AddI32(a_leftTargetImageInfo.cm_x);
                p_packer->AddI32(a_leftTargetImageInfo.cm_y);
                p_packer->AddI32(a_leftTargetImageInfo.target_area);
                p_packer->AddFloat(a_leftTargetImageInfo.compactness);
                p_packer->AddI32(a_leftTargetImageInfo.br_left);
                p_packer->AddI32(a_leftTargetImageInfo.br_top);
                p_packer->AddI32(a_leftTargetImageInfo.br_width);
                p_packer->AddI32(a_leftTargetImageInfo.br_height);
                p_packer->AddBoolean(a_leftTargetImageInfo.has_target);
            }
            p_packer->FinalizeCluster();
            
            // Right Target
            p_packer->AddCluster();
            {
                p_packer->AddI32(a_rightTargetImageInfo.cm_x);
                p_packer->AddI32(a_rightTargetImageInfo.cm_y);
                p_packer->AddI32(a_rightTargetImageInfo.target_area);
                p_packer->AddFloat(a_rightTargetImageInfo.compactness);
                p_packer->AddI32(a_rightTargetImageInfo.br_left);
                p_packer->AddI32(a_rightTargetImageInfo.br_top);
                p_packer->AddI32(a_rightTargetImageInfo.br_width);
                p_packer->AddI32(a_rightTargetImageInfo.br_height);
                p_packer->AddBoolean(a_rightTargetImageInfo.has_target);
            }
            p_packer->FinalizeCluster();
            
            // Bottom Target
            p_packer->AddCluster();
            {
                p_packer->AddI32(a_bottomTargetImageInfo.cm_x);
                p_packer->AddI32(a_bottomTargetImageInfo.cm_y);
                p_packer->AddI32(a_bottomTargetImageInfo.target_area);
                p_packer->AddFloat(a_bottomTargetImageInfo.compactness);
                p_packer->AddI32(a_bottomTargetImageInfo.br_left);
                p_packer->AddI32(a_bottomTargetImageInfo.br_top);
                p_packer->AddI32(a_bottomTargetImageInfo.br_width);
                p_packer->AddI32(a_bottomTargetImageInfo.br_height);
                p_packer->AddBoolean(a_bottomTargetImageInfo.has_target);
            }
            p_packer->FinalizeCluster();
            
            p_packer->AddI32(a_numVisibleTargets);
        }
        p_packer->FinalizeCluster();
        // END:  Vision
        
        
        // BEGIN:  Drive Base
        p_packer->AddCluster();
        {
            p_packer->AddI32(a_leftEncoderCount);
            p_packer->AddI32(a_rightEncoderCount);
            p_packer->AddFloat(a_leftDriveMotorOutput);
            p_packer->AddFloat(a_rightDriveMotorOutput);
            p_packer->AddBoolean(WsTypes::WS_S_HIGHGEAR == a_shifterState);
            p_packer->AddBoolean(a_antiTurboEnabled);
            p_packer->AddBoolean(a_driveBaseAccelerationEnabledFlag);
            p_packer->AddFloat(a_driverThrottleValue);
            p_packer->AddFloat(a_driverHeadingValue);
            p_packer->AddBoolean(a_driverAntiTurboButtonState);
            p_packer->AddBoolean(a_driverShifterButtonState);
            p_packer->AddBoolean(a_driveBaseAccelerationButtonState);
            p_packer->AddBoolean(WsTypes::WS_POGO_STICK_CYLINDER_DOWN == a_pogoState);
            p_packer->AddBoolean(WsTypes::WS_BRIDGE_BRAKE_CYLINDER_ENGAGED == a_brakeState);
            p_packer->AddBoolean(WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED == a_robotGrabberState);
        }
        p_packer->FinalizeCluster();
        // END:  Drive Base
        
        
        // BEGIN:  Compressor
        p_packer->AddCluster();
        {
            p_packer->AddBoolean(WsTypes::WS_COMPRESSOR_ON == a_compressorState);
            p_packer->AddFloat(a_pneumaticPressureReading);
        }
        p_packer->FinalizeCluster();
        // BEGIN:  Compressor
        
        
        // BEGIN:  Shooter Preset
        p_packer->AddCluster();
        {
            // Key Forward
            p_packer->AddFloat(a_shooterPresetConfig.turret_key_forward_voltage);
            p_packer->AddFloat(a_shooterPresetConfig.flywheel_key_forward_speed);
            p_packer->AddFloat(a_shooterPresetConfig.flywheel_key_forward_motor_val);
            
            // Key Backward
            p_packer->AddFloat(a_shooterPresetConfig.turret_key_reverse_voltage);
            p_packer->AddFloat(a_shooterPresetConfig.flywheel_key_reverse_speed);
            p_packer->AddFloat(a_shooterPresetConfig.flywheel_key_reverse_motor_val);
            
            // Fender Left
            p_packer->AddFloat(a_shooterPresetConfig.turret_fender_left_voltage);
            p_packer->AddFloat(a_shooterPresetConfig.flywheel_fender_left_speed);
            p_packer->AddFloat(a_shooterPresetConfig.flywheel_fender_left_motor_val);
            
            // Fender Right
            p_packer->AddFloat(a_shooterPresetConfig.turret_fender_right_voltage);
            p_packer->AddFloat(a_shooterPresetConfig.flywheel_fender_right_speed);
            p_packer->AddFloat(a_shooterPresetConfig.flywheel_fender_right_motor_val);
            
            // Generic Auton
            p_packer->AddFloat(a_shooterPresetConfig.turret_auton_voltage);
            p_packer->AddFloat(a_shooterPresetConfig.flywheel_auton_speed);
            p_packer->AddFloat(a_shooterPresetConfig.flywheel_auton_motor_val);
        }
        p_packer->FinalizeCluster();
        // BEGIN:  Shooter Preset
    }
    p_packer->FinalizeCluster();
    p_packer->Finalize(); // This actually sends the data packet
    
    // Reset our send timer
    ap_dataSendTimer->Reset();
    
    WS_LOG_TRACE("End");
}


//-----------------------------------------------------------------------------
// Turret
//-----------------------------------------------------------------------------
void
WsDashboardUpdater::acceptNewTurretAtPositionFlag(const bool& rc_newFlag)
{
    a_turretAtPositionFlag = rc_newFlag;
}

void
WsDashboardUpdater::acceptNewTurretNearPositionFlag(const bool& rc_newFlag)
{
    a_turretNearPositionFlag = rc_newFlag;
}

void
WsDashboardUpdater::acceptNewTurretMotorOutput(const float& rc_newValue)
{
    a_turretMotorOutput = rc_newValue;
}

void
WsDashboardUpdater::acceptNewTurretHeadingAngle(const float& rc_newValue)
{
    a_turretHeadingAngle = rc_newValue;
}

void
WsDashboardUpdater::acceptNewTurretHeadingPotValue(const float& rc_newValue)
{
    a_turretHeadingPotValue = rc_newValue;
}

void
WsDashboardUpdater::acceptNewTurretPidSetpoint(const float& rc_newValue)
{
    a_turretPidSetpoint = rc_newValue;
}

void
WsDashboardUpdater::acceptNewTurretPidEnabledFlag(const bool& rc_newFlag)
{
    a_turretPidEnabledFlag = rc_newFlag;
}

void
WsDashboardUpdater::acceptNewTurretPidConfig(const WsTypes::WsPidConfigurationDataT& rc_newConfig)
{
    a_turretPidConfig = rc_newConfig;
}

void
WsDashboardUpdater::acceptNewRobotHeadingGyroAngle(const float& rc_newValue)
{
    a_robotHeadingGyroAngle = rc_newValue;
}



//-----------------------------------------------------------------------------
// Auto Program
//-----------------------------------------------------------------------------
void
WsDashboardUpdater::acceptNewAutonomousLockInState(const bool& rc_newState)
{
    a_automonousLockInState = rc_newState;
}

void
WsDashboardUpdater::acceptNewAutonomousProgramId(const INT32& rc_newValue)
{
    a_autonomousProgramId = rc_newValue;
}

void
WsDashboardUpdater::acceptNewLockedAutoProgramId(const INT32& rc_newValue)
{
    a_lockedAutonomousProgramId = rc_newValue;
}

//-----------------------------------------------------------------------------
// Flywheel
//-----------------------------------------------------------------------------
void
WsDashboardUpdater::acceptNewFlywheelAtSpeedFlag(const bool& rc_newFlag)
{
    a_flywheelAtSpeedFlag = rc_newFlag;
}

void
WsDashboardUpdater::acceptNewFlywheelNearSpeedFlag(const bool& rc_newFlag)
{
    a_flywheelNearSpeedFlag = rc_newFlag;
}

void
WsDashboardUpdater::acceptNewFlywheelRotationalSpeed(const float& rc_newSpeed)
{
    a_flywheelRotationalSpeed = rc_newSpeed;
}

void
WsDashboardUpdater::acceptNewFlywheelPidSetpoint(const float& rc_newValue)
{
    a_flywheelPidSetpoint = rc_newValue;
}

void
WsDashboardUpdater::acceptNewFlywheelPidEnabledFlag(const bool& rc_newFlag)
{
    a_flywheelPidEnabledFlag = rc_newFlag;
}

void
WsDashboardUpdater::acceptNewFlywheelMotorOutput(const float& rc_newValue)
{
    a_flywheelMotorOutput = rc_newValue;
}

void
WsDashboardUpdater::acceptNewFlywheelPidControllerOutput(const float& rc_newValue)
{
    a_flywheelPidControllerOutput = rc_newValue;
}

void
WsDashboardUpdater::acceptNewFloodlightState(const bool& rc_newState)
{
    a_flywheelFloodLightState = rc_newState;
}

void
WsDashboardUpdater::acceptNewFreshImageFlag(const bool& rc_newFlag)
{
    a_flywheelFreshImageFlag = rc_newFlag;
}

void
WsDashboardUpdater::acceptNewLocationSpeedMapping(const WsTypes::WsLocationSpeedMappingType& rc_newMapping)
{
    a_flywheelLocationSpeedMapping = rc_newMapping;
}

void
WsDashboardUpdater::acceptNewFlywheelManualPotValue(const float& rc_newValue)
{
    a_flywheelManualSpeedPotValue = rc_newValue;
}

void
WsDashboardUpdater::acceptNewFlywheelStatePotValue(const float& rc_newValue)
{
    a_flywheelControlStatePotValue = rc_newValue;
}

void
WsDashboardUpdater::acceptNewFlywheelPidConfig(const WsTypes::WsPidConfigurationDataT& rc_newConfig)
{
    a_flywheelPidConfig = rc_newConfig;
}

void
WsDashboardUpdater::acceptNewFlywheelCompensationSwitchState(const bool& rc_newState)
{
    a_flywheelCompensationSwitchState = rc_newState;
}

void
WsDashboardUpdater::acceptNewFlywheelCompensationPotValue(const float& rc_newValue)
{
    a_flywheelCompensationPotValue = rc_newValue;
}


//-----------------------------------------------------------------------------
// Vision
//-----------------------------------------------------------------------------
void
WsDashboardUpdater::acceptNewTopTargetImageInfo(const WsTypes::WsTargetImageInfoType& rc_newValue)
{
    a_topTargetImageInfo = rc_newValue;
}

void
WsDashboardUpdater::acceptNewLeftTargetImageInfo(const WsTypes::WsTargetImageInfoType& rc_newValue)
{
    a_leftTargetImageInfo = rc_newValue;
}

void
WsDashboardUpdater::acceptNewRightTargetImageInfo(const WsTypes::WsTargetImageInfoType& rc_newValue)
{
    a_rightTargetImageInfo = rc_newValue;
}

void
WsDashboardUpdater::acceptNewBottomTargetImageInfo(const WsTypes::WsTargetImageInfoType& rc_newValue)
{
    a_bottomTargetImageInfo = rc_newValue;
}

void
WsDashboardUpdater::acceptNewNumVisibleTargets(const int& rc_newValue)
{
    a_numVisibleTargets = rc_newValue;
}

//-----------------------------------------------------------------------------
// Drive Base
//-----------------------------------------------------------------------------
void
WsDashboardUpdater::acceptNewLeftEncoderValue(const INT32& rc_newValue)
{
    a_leftEncoderCount = rc_newValue;
}

void
WsDashboardUpdater::acceptNewRightEncoderValue(const INT32& rc_newValue)
{
    a_rightEncoderCount = rc_newValue;
}

void
WsDashboardUpdater::acceptNewLeftDriveMotorOutput(const float& rc_newValue)
{
    a_leftDriveMotorOutput = rc_newValue;
}

void
WsDashboardUpdater::acceptNewRightDriveMotorOutput(const float& rc_newValue)
{
    a_rightDriveMotorOutput = rc_newValue;
}

void
WsDashboardUpdater::acceptNewShifterState(const WsTypes::WsShifterStateType& rc_newValue)
{
    a_shifterState = rc_newValue;
}

void
WsDashboardUpdater::acceptNewPogoState(const WsTypes::WsPogoStickCylinderStateType& rc_newValue)
{
    a_pogoState = rc_newValue;
}

void
WsDashboardUpdater::acceptNewBridgeBrakeState(const WsTypes::WsBridgeBrakeCylinderStateType& rc_newValue)
{
    a_brakeState = rc_newValue;
}

void
WsDashboardUpdater::acceptNewRobotGrabberState(const WsTypes::WsRobotGrabberCylinderStateType& rc_newValue)
{
    a_robotGrabberState = rc_newValue;
}

void
WsDashboardUpdater::acceptNewAntiTurboEnabledFlag(const bool& rc_newValue)
{
    a_antiTurboEnabled = rc_newValue;
}

void
WsDashboardUpdater::acceptNewDriverThrottleValue(const float& rc_newValue)
{
    a_driverThrottleValue = rc_newValue;
}

void
WsDashboardUpdater::acceptNewDriverHeadingValue(const float& rc_newValue)
{
    a_driverHeadingValue = rc_newValue;
}

void
WsDashboardUpdater::acceptNewAntiTurboButtonState(const bool& rc_newValue)
{
    a_driverAntiTurboButtonState = rc_newValue;
}

void
WsDashboardUpdater::acceptNewShifterButtonState(const bool& rc_newValue)
{
    a_driverShifterButtonState = rc_newValue;
}

void
WsDashboardUpdater::acceptNewDriveBaseAccelerationButtuonState(const bool& rc_newValue)
{
    a_driveBaseAccelerationButtonState = rc_newValue;
}

void
WsDashboardUpdater::acceptNewDriveBaseAccelerationEnabledFlag(const bool& rc_newValue)
{
    a_driveBaseAccelerationEnabledFlag = rc_newValue;
}


//-----------------------------------------------------------------------------
// Compressor
//-----------------------------------------------------------------------------
void
WsDashboardUpdater::acceptNewCompressorState(const WsTypes::WsCompressorStateType& rc_newState)
{
    a_compressorState = rc_newState;
}

void
WsDashboardUpdater::acceptNewPneumaticPressureReading(const float& rc_newValue)
{
    a_pneumaticPressureReading = rc_newValue;
}


//-----------------------------------------------------------------------------
// Shooter Preset
//-----------------------------------------------------------------------------
void
WsDashboardUpdater::acceptNewShooterPresetConfig(const WsTypes::WsShooterPresetConfigT& rc_newConfig)
{
    a_shooterPresetConfig = rc_newConfig;
}


//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

