#ifndef _WsOutputFacade_hh_
#define _WsOutputFacade_hh_

#include "semLib.h"
#include "WsTypes.hh"
#include "WsFacadeElement.hh"

class Victor;
class DoubleSolenoid;
class DigitalOutput;
class Relay;
class Servo;
class Solenoid;
class WsVictor;
class WsSubject;

//-----------------------------------------------------------------------------
class WsOutputFacade
//-----------------------------------------------------------------------------
{
    public:
        virtual ~WsOutputFacade(void);
        static WsOutputFacade* instance(void);
        
    public:
        void update(void);
        
        WsSubject* getFlywheelMotorSpeed(float& r_speed);
        void       setFlywheelMotorSpeed(const float& rc_speed);
        
        WsSubject* getLandingGearState(WsTypes::WsLandingGearStateType& r_state);
        void       setLandingGearState(const WsTypes::WsLandingGearStateType& rc_state);
        
        WsSubject* getShifterState(WsTypes::WsShifterStateType& r_state);
        void       setShifterState(const WsTypes::WsShifterStateType& rc_state);
        
        WsSubject* getAccumulatorSpeed(float& r_speed);
        void       setAccumulatorSpeed(const float& rc_speed);
        
        WsSubject* getCompressorState(WsTypes::WsCompressorStateType& r_state);
        void       setCompressorState(const WsTypes::WsCompressorStateType& rc_state);
        
        WsSubject* getTurretHeadingSpeed(float& r_speed);
        void       setTurretHeadingSpeed(const float& rc_speed);
        
        WsSubject* getFloodLightState(bool& r_state);
        void       setFloodLightState(const bool& rc_state);
        
        WsSubject* getEscalatorSpeed(float& r_speed);
        void       setEscalatorSpeed(const float& rc_speed);
        
        WsSubject* getElevatorSpeed(float& r_speed);
        void       setElevatorSpeed(const float& rc_speed);
        
        WsSubject* getShooterCylinderState(WsTypes::WsShooterCylinderStateType& r_state);
        void       setShooterCylinderState(const WsTypes::WsShooterCylinderStateType& rc_state);
        
        WsSubject* getHoodState(WsTypes::WsHoodCylinderStateType& r_state);
        void       setHoodState(const WsTypes::WsHoodCylinderStateType& rc_state);
        
        WsSubject* getRobotGrabberInnerCylinderState(WsTypes::WsRobotGrabberCylinderStateType& r_state);
        void       setRobotGrabberInnerCylinderState(const WsTypes::WsRobotGrabberCylinderStateType& rc_state);
        
        WsSubject* getRobotGrabberOuterCylinderState(WsTypes::WsRobotGrabberCylinderStateType& r_state);
        void       setRobotGrabberOuterCylinderState(const WsTypes::WsRobotGrabberCylinderStateType& rc_state);
        
        WsSubject* getAccumulatorState(WsTypes::WsAccumulatorStateType& r_state);
        void       setAccumulatorState(const WsTypes::WsAccumulatorStateType& rc_state);
        
        WsSubject* getPogoStickCylinderState(WsTypes::WsPogoStickCylinderStateType& r_state);
        void       setPogoStickCylinderState(const WsTypes::WsPogoStickCylinderStateType& rc_state);
        
        WsSubject* getBridgeBrakeCylinderState(WsTypes::WsBridgeBrakeCylinderStateType& r_state);
        void       setBridgeBrakeCylinderState(const WsTypes::WsBridgeBrakeCylinderStateType& rc_state);
        
        
        // Values taken in by drive motors that are positive are assumed forward
        // and values taken in that are less than 1 are assumed backwards
        WsSubject* getRightDriveMotorSpeed(float& r_speed);
        void       setRightDriveMotorSpeed(const float& rc_speed);
        
        WsSubject* getLeftDriveMotorSpeed(float& r_speed);
        void       setLeftDriveMotorSpeed(const float& rc_speed);
        
    protected:
        WsOutputFacade();
        
    private:
        static WsOutputFacade* asp_instance;
        SEM_ID                 ap_mutexSemaphore;
        
    private:
        WsFacadeElement<WsTypes::WsShooterCylinderStateType>*       ap_shooterCylinderStateElement;
        WsFacadeElement<bool>*                                      ap_floodLightStateElement;
        WsFacadeElement<float>*                                     ap_flywheelSpeedElement;
        WsFacadeElement<float>*                                     ap_rightDriveMotorSpeedElement;
        WsFacadeElement<float>*                                     ap_leftDriveMotorSpeedElement;
        WsFacadeElement<float>*                                     ap_accumulatorMotorSpeedElement;
        WsFacadeElement<float>*                                     ap_turretMotorSpeedElement;
        WsFacadeElement<float>*                                     ap_escalatorMotorSpeedElement;
        WsFacadeElement<float>*                                     ap_elevatorMotorSpeedElement;
        WsFacadeElement<WsTypes::WsHoodCylinderStateType>*          ap_hoodCylinderStateElement;
        WsFacadeElement<WsTypes::WsAccumulatorStateType>*           ap_accumulatorDeploymentStateElement;
        WsFacadeElement<WsTypes::WsLandingGearStateType>*           ap_landingGearDeploymentStateElement;
        WsFacadeElement<WsTypes::WsShifterStateType>*               ap_shifterStateElement;
        WsFacadeElement<WsTypes::WsCompressorStateType>*            ap_compressorRelayStateElement;
        WsFacadeElement<WsTypes::WsRobotGrabberCylinderStateType>*  ap_robotGrabberInnerCylinderStateElement;
        WsFacadeElement<WsTypes::WsRobotGrabberCylinderStateType>*  ap_robotGrabberOuterCylinderStateElement;
        WsFacadeElement<WsTypes::WsPogoStickCylinderStateType>*     ap_pogoStickDeploymentStateElement;
        WsFacadeElement<WsTypes::WsBridgeBrakeCylinderStateType>*   ap_bridgeBrakeCylinderStateElement;
        
    private:
        WsVictor*       ap_flywheelSpeedController;
        WsVictor*       ap_rightMotor1SpeedController;
        WsVictor*       ap_rightMotor2SpeedController;
        WsVictor*       ap_leftMotor1SpeedController;
        WsVictor*       ap_leftMotor2SpeedController;
        Victor*         ap_accumulatorSpeedController;
        Victor*         ap_turretSpeedController;
        Victor*         ap_escalatorSpeedController;
        Victor*         ap_elevatorSpeedController;
        Solenoid*       ap_hoodSolenoid;
        Solenoid*       ap_landingGearSolenoid;
        Solenoid*       ap_shifterSolenoid;
        Relay*          ap_floodLightRelay;
        Relay*          ap_compressorRelay;
        Solenoid*       ap_ballShooterSolenoid;
        Solenoid*       ap_robotGrabberInnerSolenoid;
        Solenoid*       ap_robotGrabberOuterSolenoid;
        Solenoid*       ap_accumulatorSolenoid;
        DoubleSolenoid* ap_pogoStickSolenoid;
        DoubleSolenoid* ap_bridgeBrakeSolenoid;
        
    private:
        WsOutputFacade(const WsOutputFacade& rc_rhs);
        const WsOutputFacade& operator=(const WsOutputFacade& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

