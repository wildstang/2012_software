#ifndef _WsInputFacade_hh_
#define _WsInputFacade_hh_

#include "WsTypes.hh"
#include "WsFacadeElement.hh"
#include "semLib.h"

class AnalogChannel;
class Joystick;
class DriverStation;
class DigitalInput;
class Encoder;
class WsGyro;

class WsInputFacade
{
    public:
        typedef struct WsSensoryInputStruct
        {
            WsTypes::WsTargetImageInfoType TargetImageInfo[WsTypes::WS_BASKET_COUNT];
        } WsSensoryInputsType;
        
        virtual ~WsInputFacade();
        static WsInputFacade* instance();
        void updateSensorData(void);
        void updateOiData(void);
        
        WsSubject* getDriverThrottleValue(float& r_throttle);
        void       setDriverThrottleValue(const float& rc_throttle);
        WsSubject* getDriverHeadingValue(float& r_heading);
        void       setDriverHeadingValue(const float& rc_heading);
        WsSubject* getRequestedTurretDirection(float& r_direction);
        void       setRequestedTurretDirection(const float& rc_direction);
        WsSubject* getFlywheelStatePotValue(float& r_value);
        void       setFlywheelStatePotValue(const float& rc_value);
        WsSubject* getShifterButtonState(bool& r_state);
        void       setShifterButtonState(const bool& rc_state);
        WsSubject* getAntiTurboButtonState(bool& r_state);
        void       setAntiTurboButtonState(const bool& rc_state);
        WsSubject* getAccumulatorDeployButtonState(bool& r_state);
        void       setAccumulatorDeployButtonState(const bool& rc_state);
        WsSubject* getLandingGearButtonState(bool& r_state);
        void       setLandingGearButtonState(const bool& rc_state);
        WsSubject* getFloodLightButtonState(bool& r_state);
        void       setFloodLightButtonState(const bool& rc_state);
        WsSubject* getTurretHeadingPotValue(float& r_heading);
        void       setTurretHeadingPotValue(const float& rc_heading);
        WsSubject* getHoodButtonState(bool& r_state);
        void       setHoodButtonState(const bool& rc_state);
        WsSubject* getPressureSwitchState(bool& r_state);
        void       setPressureSwitchState(const bool& rc_state);
        WsSubject* getLeftDriveEncoderCount(INT32& r_count);
        void       setLeftDriveEncoderCount(const INT32& rc_count);
        WsSubject* getRightDriveEncoderCount(INT32& r_count);
        void       setRightDriveEncoderCount(const INT32& rc_count);
        void       resetDriveEncoderCounts(void);
        WsSubject* getEscalatorDownButtonState(bool& r_state);
        void       setEscalatorDownButtonState(const bool& rc_state);
        WsSubject* getEscalatorUpButtonState(bool& r_state);
        void       setEscalatorUpButtonState(const bool& rc_state);
        WsSubject* getBallShootButtonState(bool& r_state);
        void       setBallShootButtonState(const bool& rc_state);
        WsSubject* getBallShootOverrideButtonState(bool& r_state);
        void       setBallShootOverrideButtonState(const bool& rc_state);
        WsSubject* getTurretCwLimitSwitchState(bool& r_state);
        void       setTurretCwLimitSwitchState(const bool& rc_state);
        WsSubject* getTurretCcwLimitSwitchState(bool& r_state);
        void       setTurretCcwLimitSwitchState(const bool& rc_state);
        
        WsSubject* getRobotGrabberButtonState(bool& r_state);
        void       setRobotGrabberButtonState(const bool& rc_state);
        WsSubject* getLandingGearRetractionSwitch(bool& r_state);
        void       setLandingGearRetractionSwitch(const bool& rc_state);
        WsSubject* getFlywheelManualPot(float& r_value);
        void       setFlywheelManualPot(const float& rc_value);
        WsSubject* getFlywheelRotationalSpeed(float& r_speed);
        void       setFlywheelRotationalSpeed(const float& rc_speed);
        
        WsSubject* getPogoStickDeploymentButtonState(bool& r_state);
        void       setPogoStickDeploymentButtonState(const bool& rc_state);
        
        WsSubject* getFreshImageFlag(bool& r_flag);
        void       setFreshImageFlag(const bool& rc_flag);
        
        // @TODO: Make these subjects
        void getTargetImageInfo(const WsTypes::WsTargetBasketIdxType& rc_target_idx,
                                WsTypes::WsTargetImageInfoType& r_info);
        void setTargetImageInfo(const WsTypes::WsTargetBasketIdxType& rc_target_idx,
                                WsTypes::WsTargetImageInfoType& r_info);
        void getTargetImageIndecesByRank(WsTypes::WsTargetBasketIdxType& r_rank_one_idx,
                                         WsTypes::WsTargetBasketIdxType& r_rank_two_idx,
                                         WsTypes::WsTargetBasketIdxType& r_rank_three_idx,
                                         WsTypes::WsTargetBasketIdxType& r_rank_four_idx,
                                         WsTypes::WsTargetRankingCriteriaType criteria = WsTypes::WS_RANKING_CRITERIA_PARTICLE_AREA);
                                         
        WsSubject* getNumVisibleTargets(int& r_numTargets);
        void       setNumVisibleTargets(const int& rc_numTargets);
        WsSubject* getTurretTurboButtonState(bool& r_state);
        void       setTurretTurboButtonState(const bool& rc_state);
        WsSubject* getLockInSwitchState(bool& r_state);
        void       setLockInSwitchState(const bool& rc_state);
        WsSubject* getAutonomousProgramId(WsTypes::WsAutonomousProgramIdType& r_id);
        void       setAutonomousProgramId(const WsTypes::WsAutonomousProgramIdType& rc_id);
        WsSubject* getAutoProgramSelectorValue(float& r_value);
        void       setAutoProgramSelectorValue(const float& rc_value);
        WsSubject* getShooterPreset(WsTypes::WsShooterPresetType& r_value);
        void       setShooterPreset(const WsTypes::WsShooterPresetType& rc_value);
        WsSubject* getPneumaticPressureReading(float& r_value);
        void       setPneumaticPressureReading(const float& rc_value);
        WsSubject* getDriveBaseAccelerationButtonState(bool& r_value);
        void       setDriveBaseAccelerationButtonState(const bool& rc_value);
        WsSubject* getAccumulatorMotorButtonState(bool& r_state);
        void       setAccumulatorMotorButtonState(const bool& rc_state);
        WsSubject* getRobotHeadingGyroAngle(float& r_value);
        void       setRobotHeadingGyroAngle(const float& rc_value);
        WsSubject* getBridgeBrakeActivationButtonState(bool& r_state);
        void       setBridgeBrakeActivationButtonState(const bool& rc_state);
        WsSubject* getFlywheelSpeedCompensationPotValue(float& r_value);
        void       setFlywheelSpeedCompensationPotValue(const float& rc_value);
        WsSubject* getFlywheelSpeedCompensationEnableSwitchState(bool& r_state);
        void       setFlywheelSpeedCompensationEnableSwitchState(const bool& rc_state);
        
    protected:
        WsInputFacade();
        
    private:
        typedef struct
        {
            INT32  tick_count;
            double fpga_time;
        } WsFlywheelSnapshotType;
        
    private:
        static WsInputFacade* asp_instance;
        SEM_ID                ap_mutexSemaphore;
        
        // Input "handles"
        AnalogChannel*        ap_turretHeadingPotentiometer;
        Joystick*             ap_driverJoystick;
        Joystick*             ap_manipulatorJoystick;
        Encoder*              ap_flywheelSpeedEncoder;
        DigitalInput*         ap_pressureSwitchDigitalInput;
        Encoder*              ap_leftDriveEncoder;
        Encoder*              ap_rightDriveEncoder;
        DigitalInput*         ap_turretCwLimitSwitchDigitalInput;
        DigitalInput*         ap_turretCcwLimitSwitchDigitalInput;
        DigitalInput*         ap_landingGearRetractionDigitalInput;
        AnalogChannel*        ap_pneumaticPressureTransducer;
        WsGyro*               ap_robotHeadingGyro;
        
        // Thread-protected data
        WsSensoryInputsType   a_sensorData;
        
        // Data type for calculating real flywheel speed
        typedef std::list<WsFlywheelSnapshotType> WsFlywheelSnapshotListType;
        WsFlywheelSnapshotListType                a_flywheelSnapshotList;
        
    private:
        WsFacadeElement<bool>*                                ap_hoodButtonElement;
        WsFacadeElement<float>*                               ap_flywheelRotationalSpeedElement;
        WsFacadeElement<bool>*                                ap_accumulatorDeployButtonElement;
        WsFacadeElement<bool>*                                ap_robotGrabberButtonElement;
        WsFacadeElement<bool>*                                ap_shooterButtonElement;
        WsFacadeElement<bool>*                                ap_shooterOverrideButtonElement;
        WsFacadeElement<bool>*                                ap_escalatorUpButtonElement;
        WsFacadeElement<bool>*                                ap_escalatorDownButtonElement;
        WsFacadeElement<bool>*                                ap_pressureSwitchElement;
        WsFacadeElement<float>*                               ap_flywheelStatePotValueElement;
        WsFacadeElement<float>*                               ap_flywheelManualPotElement;
        WsFacadeElement<float>*                               ap_turretRequestedDirectionElement;
        WsFacadeElement<float>*                               ap_turretHeadingPotValueElement;
        WsFacadeElement<bool>*                                ap_turretCwLimitSwitchElement;
        WsFacadeElement<bool>*                                ap_turretCcwLimitSwitchElement;
        WsFacadeElement<bool>*                                ap_turretTurboButtonElement;
        WsFacadeElement<float>*                               ap_driveThrottleElement;
        WsFacadeElement<float>*                               ap_driveHeadingElement;
        WsFacadeElement<bool>*                                ap_antiturboButtonElement;
        WsFacadeElement<bool>*                                ap_shifterButtonElement;
        WsFacadeElement<bool>*                                ap_landingGearButtonElement;
        WsFacadeElement<bool>*                                ap_landingGearRetractionSwitchElement;
        WsFacadeElement<bool>*                                ap_lockInSwitchElement;
        WsFacadeElement<bool>*                                ap_floodLightStateElement;
        WsFacadeElement<bool>*                                ap_pogoStickButtonElement;
        WsFacadeElement<bool>*                                ap_freshImageFlagElement;
        WsFacadeElement<WsTypes::WsAutonomousProgramIdType>*  ap_autonomousProgramIdElement;
        WsFacadeElement<int>*                                 ap_numVisibleTargetsElement;
        WsFacadeElement<float>*                               ap_autoProgramSelectorElement;
        WsFacadeElement<WsTypes::WsShooterPresetType>*        ap_shooterPresetElement;
        WsFacadeElement<INT32>*                               ap_leftDriveEncoderElement;
        WsFacadeElement<INT32>*                               ap_rightDriveEncoderElement;
        WsFacadeElement<float>*                               ap_pneumaticPressureReadingElement;
        WsFacadeElement<bool>*                                ap_driveBaseAccelerationButtonElement;
        WsFacadeElement<bool>*                                ap_accumulatorMotorButtonElement;
        WsFacadeElement<float>*                               ap_robotHeadingGyroAngleElement;
        WsFacadeElement<bool>*                                ap_bridgeBrakeButtonElement;
        WsFacadeElement<bool>*                                ap_flywheelSpeedCompensationEnableSwitchElement;
        WsFacadeElement<float>*                               ap_flywheelSpeedCompensationPotElement;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

