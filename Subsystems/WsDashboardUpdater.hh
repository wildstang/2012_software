#ifndef __WsDashboardUpdater_hh__
#define __WsDashboardUpdater_hh__

#include "WsSubsystem.hh"
#include "WsTypes.hh"

class Timer;
class WsObserver;
class WsDashboardUpdater : public WsSubsystem
{
    public:
        WsDashboardUpdater(void);
        virtual ~WsDashboardUpdater(void);
        void update(void);
        
        // Turret
        void acceptNewTurretAtPositionFlag(const bool& rc_newFlag);
        void acceptNewTurretNearPositionFlag(const bool& rc_newFlag);
        void acceptNewTurretMotorOutput(const float& rc_newValue);
        void acceptNewTurretHeadingAngle(const float& rc_newValue);
        void acceptNewTurretHeadingPotValue(const float& rc_newValue);
        void acceptNewTurretPidSetpoint(const float& rc_newValue);
        void acceptNewTurretPidEnabledFlag(const bool& rc_newFlag);
        void acceptNewTurretPidConfig(const WsTypes::WsPidConfigurationDataT& rc_newConfig);
        void acceptNewRobotHeadingGyroAngle(const float& rc_newValue);
        
        // Auto Program
        void acceptNewAutonomousLockInState(const bool& rc_newState);
        void acceptNewAutonomousProgramId(const INT32& rc_newValue);
        void acceptNewLockedAutoProgramId(const INT32& rc_newValue);
        
        // Flywheel
        void acceptNewFlywheelAtSpeedFlag(const bool& rc_newFlag);
        void acceptNewFlywheelNearSpeedFlag(const bool& rc_newFlag);
        void acceptNewFlywheelRotationalSpeed(const float& rc_newValue);
        void acceptNewFlywheelPidSetpoint(const float& rc_newValue);
        void acceptNewFlywheelPidEnabledFlag(const bool& rc_newFlag);
        void acceptNewFlywheelMotorOutput(const float& rc_newValue);
        void acceptNewFlywheelPidControllerOutput(const float& rc_newValue);
        void acceptNewFloodlightState(const bool& rc_newState);
        void acceptNewFreshImageFlag(const bool& rc_newFlag);
        void acceptNewLocationSpeedMapping(const WsTypes::WsLocationSpeedMappingType& rc_newMapping);
        void acceptNewFlywheelManualPotValue(const float& rc_newValue);
        void acceptNewFlywheelStatePotValue(const float& rc_newValue);
        void acceptNewFlywheelPidConfig(const WsTypes::WsPidConfigurationDataT& rc_newConfig);
        void acceptNewFlywheelCompensationSwitchState(const bool& rc_newState);
        void acceptNewFlywheelCompensationPotValue(const float& rc_newValue);
        
        // Vision
        void acceptNewTopTargetImageInfo(const WsTypes::WsTargetImageInfoType& rc_newValue);
        void acceptNewLeftTargetImageInfo(const WsTypes::WsTargetImageInfoType& rc_newValue);
        void acceptNewRightTargetImageInfo(const WsTypes::WsTargetImageInfoType& rc_newValue);
        void acceptNewBottomTargetImageInfo(const WsTypes::WsTargetImageInfoType& rc_newValue);
        void acceptNewNumVisibleTargets(const int& rc_newValue);
        
        // DriveBase
        void acceptNewLeftEncoderValue(const INT32& rc_newValue);
        void acceptNewRightEncoderValue(const INT32& rc_newValue);
        void acceptNewLeftDriveMotorOutput(const float& rc_newValue);
        void acceptNewRightDriveMotorOutput(const float& rc_newValue);
        void acceptNewShifterState(const WsTypes::WsShifterStateType& rc_newValue);
        void acceptNewPogoState(const WsTypes::WsPogoStickCylinderStateType& rc_newValue);
        void acceptNewBridgeBrakeState(const WsTypes::WsBridgeBrakeCylinderStateType& rc_newValue);
        void acceptNewRobotGrabberState(const WsTypes::WsRobotGrabberCylinderStateType& rc_newValue);
        void acceptNewAntiTurboEnabledFlag(const bool& rc_newValue);
        void acceptNewDriverThrottleValue(const float& rc_newValue);
        void acceptNewDriverHeadingValue(const float& rc_newValue);
        void acceptNewAntiTurboButtonState(const bool& rc_newValue);
        void acceptNewShifterButtonState(const bool& rc_newValue);
        void acceptNewDriveBaseAccelerationButtuonState(const bool& rc_newValue);
        void acceptNewDriveBaseAccelerationEnabledFlag(const bool& rc_newValue);
        
        // Compressor
        void acceptNewCompressorState(const WsTypes::WsCompressorStateType& rc_newState);
        void acceptNewPneumaticPressureReading(const float& rc_newValue);
        
        // Shooter Preset
        void acceptNewShooterPresetConfig(const WsTypes::WsShooterPresetConfigT& rc_newConfig);
        
        // Next Subsystem...
        
        
    private:
        // Turret
        bool                             a_turretAtPositionFlag;
        bool                             a_turretNearPositionFlag;
        float                            a_turretMotorOutput;
        float                            a_turretHeadingAngle;
        float                            a_turretHeadingPotValue;
        float                            a_turretPidSetpoint;
        bool                             a_turretPidEnabledFlag;
        WsTypes::WsPidConfigurationDataT a_turretPidConfig;
        float                            a_robotHeadingGyroAngle;
        
        // Auto Program
        bool                             a_automonousLockInState;
        INT32                            a_autonomousProgramId;
        INT32                            a_lockedAutonomousProgramId;
        
        // Flywheel
        bool                                 a_flywheelAtSpeedFlag;
        bool                                 a_flywheelNearSpeedFlag;
        float                                a_flywheelRotationalSpeed;
        float                                a_flywheelPidSetpoint;
        bool                                 a_flywheelPidEnabledFlag;
        float                                a_flywheelMotorOutput;
        float                                a_flywheelPidControllerOutput;
        bool                                 a_flywheelFloodLightState;
        bool                                 a_flywheelFreshImageFlag;
        WsTypes::WsLocationSpeedMappingType  a_flywheelLocationSpeedMapping;
        float                                a_flywheelManualSpeedPotValue;
        float                                a_flywheelControlStatePotValue;
        WsTypes::WsPidConfigurationDataT     a_flywheelPidConfig;
        bool                                 a_flywheelCompensationSwitchState;
        float                                a_flywheelCompensationPotValue;
        
        // Vision
        WsTypes::WsTargetImageInfoType  a_topTargetImageInfo;
        WsTypes::WsTargetImageInfoType  a_leftTargetImageInfo;
        WsTypes::WsTargetImageInfoType  a_rightTargetImageInfo;
        WsTypes::WsTargetImageInfoType  a_bottomTargetImageInfo;
        int                             a_numVisibleTargets;
        
        // DriveBase
        INT32                        a_leftEncoderCount;
        INT32                        a_rightEncoderCount;
        float                        a_leftDriveMotorOutput;
        float                        a_rightDriveMotorOutput;
        WsTypes::WsShifterStateType  a_shifterState;
        WsTypes::WsPogoStickCylinderStateType  a_pogoState;
        WsTypes::WsBridgeBrakeCylinderStateType  a_brakeState;
        WsTypes::WsRobotGrabberCylinderStateType  a_robotGrabberState;
        bool                         a_antiTurboEnabled;
        float                        a_driverThrottleValue;
        float                        a_driverHeadingValue;
        bool                         a_driverAntiTurboButtonState;
        bool                         a_driverShifterButtonState;
        bool                         a_driveBaseAccelerationButtonState;
        bool                         a_driveBaseAccelerationEnabledFlag;
        
        // Compressor
        WsTypes::WsCompressorStateType  a_compressorState;
        float                           a_pneumaticPressureReading;
        
        // Shooter Preset
        WsTypes::WsShooterPresetConfigT  a_shooterPresetConfig;
        
        // Next Subsystem...
        
    private:
        // Turret Observers
        WsObserver*   ap_turretAtPositionObserver;
        WsObserver*   ap_turretNearPositionObserver;
        WsObserver*   ap_turretMotorOutputObserver;
        WsObserver*   ap_turretHeadingAngleObserver;
        WsObserver*   ap_turretHeadingPotValueObserver;
        WsObserver*   ap_turretPidSetpointObserver;
        WsObserver*   ap_turretPidEnabledObserver;
        WsObserver*   ap_turretPidConfigObserver;
        WsObserver*   ap_robotHeadingGyroAngleObserver;
        
        // Auto Program Observers
        WsObserver*   ap_autonomousLockInStateObserver;
        WsObserver*   ap_autonomousProgramIdObserver;
        WsObserver*   ap_lockedAutonomousProgramIdObserver;
        
        // Flywheel Observers
        WsObserver*   ap_flywheelAtSpeedObserver;
        WsObserver*   ap_flywheelNearSpeedObserver;
        WsObserver*   ap_flywheelRotationalSpeedObserver;
        WsObserver*   ap_flywheelPidSetpointObserver;
        WsObserver*   ap_flywheelPidEnabledObserver;
        WsObserver*   ap_flywheelMotorOutputObserver;
        WsObserver*   ap_flywheelPidControllerOutputObserver;
        WsObserver*   ap_flywheelFloodLightStateObserver;
        WsObserver*   ap_flywheelFreshImageFlagObserver;
        WsObserver*   ap_flywheelLocationSpeedMappingObserver;
        WsObserver*   ap_flywheelManualSpeedPotValueObserver;
        WsObserver*   ap_flywheelControlPotValueObserver;
        WsObserver*   ap_flywheelPidConfigObserver;
        WsObserver*   ap_flywheelCompensationSwitchStateObserver;
        WsObserver*   ap_flywheelCompensationPotValueObserver;
        
        // Vision Observers
        WsObserver*   ap_visionNumVisibleTargetsObserver;
        // Top Target Observer
        // Left Target Observer
        // Right Target Observer
        // Bottom Target Observer
        
        // DriveBase Observers
        WsObserver*   ap_leftDriveEncoderObserver;
        WsObserver*   ap_rightDriveEncoderObserver;
        WsObserver*   ap_leftDriveMotorOutputObserver;
        WsObserver*   ap_rightDriveMotorOutputObserver;
        WsObserver*   ap_shifterStateObserver;
        WsObserver*   ap_antiTurboEnabledObserver;
        WsObserver*   ap_driverThrottleValueObserver;
        WsObserver*   ap_driverHeadingValueObserver;
        WsObserver*   ap_driverAntiTurboButtonStateObserver;
        WsObserver*   ap_driverShifterButtonStateObserver;
        WsObserver*   ap_driveBaseAccelerationButtonStateObserver;
        WsObserver*   ap_driveBaseAccelerationEnabledObserver;
        WsObserver*   ap_pogoStickStateObserver;
        WsObserver*   ap_bridgeBrakeStateObserver;
        WsObserver*   ap_robotGrabberStateObserver;
        
        // Pneumatics Observers
        WsObserver*   ap_compressorStateObserver;
        WsObserver*   ap_pneumaticPressureReadingObserver;
        
        // Shooter Preset
        WsObserver*   ap_shooterPresetConfigObserver;
        
        // Create all observers above this
        Timer*        ap_dataSendTimer;
        
    private:
        WsDashboardUpdater(const WsDashboardUpdater& rc_rhs);
        const WsDashboardUpdater& operator=(const WsDashboardUpdater& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

