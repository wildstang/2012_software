#ifndef _WsDataIndicationFacade_hh_
#define _WsDataIndicationFacade_hh_

#include "semLib.h"
#include "WsTypes.hh"
#include "WsFacadeElement.hh"

class WsSubject;

//-----------------------------------------------------------------------------
class WsDataIndicationFacade
//-----------------------------------------------------------------------------
{
    public:
        virtual ~WsDataIndicationFacade(void);
        static WsDataIndicationFacade* instance(void);
        
    public:
        void update(void);
        
        WsSubject* getFlywheelPidSetpoint(float& r_value);
        void       setFlywheelPidSetpoint(const float& rc_value);
        WsSubject* getFlywheelPidEnabled(bool& r_flag);
        void       setFlywheelPidEnabled(const bool& rc_flag);
        WsSubject* getFlywheelPidControllerOutput(float& r_value);
        void       setFlywheelPidControllerOutput(const float& rc_value);
        WsSubject* getFlywheelAtTargetSpeedFlag(bool& r_flag);
        void       setFlywheelAtTargetSpeedFlag(const bool& rc_flag);
        WsSubject* getFlywheelNearTargetSpeedFlag(bool& r_flag);
        void       setFlywheelNearTargetSpeedFlag(const bool& rc_flag);
        WsSubject* getTurretAtTargetPositionFlag(bool& r_flag);
        void       setTurretAtTargetPositionFlag(const bool& rc_flag);
        WsSubject* getTurretNearTargetPositionFlag(bool& r_flag);
        void       setTurretNearTargetPositionFlag(const bool& rc_flag);
        WsSubject* getTurretHeadingAngle(float& r_value);
        void       setTurretHeadingAngle(const float& rc_value);
        WsSubject* getTurretPidSetpoint(float& r_value);
        void       setTurretPidSetpoint(const float& rc_value);
        WsSubject* getTurretPidEnabled(bool& r_flag);
        void       setTurretPidEnabled(const bool& rc_flag);
        WsSubject* getDriveBaseAntiTurboEnabled(bool& r_flag);
        void       setDriveBaseAntiTurboEnabled(const bool& rc_flag);
        WsSubject* getDriveBaseAccelerationEnabled(bool& r_flag);
        void       setDriveBaseAccelerationEnabled(const bool& rc_flag);
        
        // @TODO:  Move autonomous "instantaneous" program here
        WsSubject* getLockedAutoProgramId(WsTypes::WsAutonomousProgramIdType& r_value);
        void       setLockedAutoProgramId(const WsTypes::WsAutonomousProgramIdType& rc_value);
        
    protected:
        WsDataIndicationFacade();
        
    private:
        static WsDataIndicationFacade* asp_instance;
        SEM_ID                         ap_mutexSemaphore;
        
    private:
        WsFacadeElement<float>*                               ap_flywheelPidSetpointElement;
        WsFacadeElement<bool>*                                ap_flywheelPidEnabledElement;
        WsFacadeElement<float>*                               ap_flywheelPidControllerOutputElement;
        WsFacadeElement<bool>*                                ap_flywheelAtTargetSpeedElement;
        WsFacadeElement<bool>*                                ap_flywheelNearTargetSpeedElement;
        WsFacadeElement<bool>*                                ap_turretAtTargetPositionElement;
        WsFacadeElement<bool>*                                ap_turretNearTargetPositionElement;
        WsFacadeElement<float>*                               ap_turretHeadingAngleElement;
        WsFacadeElement<float>*                               ap_turretPidSetpointElement;
        WsFacadeElement<bool>*                                ap_turretPidEnabledElement;
        WsFacadeElement<bool>*                                ap_driveBaseAntiTurboEnabledElement;
        WsFacadeElement<bool>*                                ap_driveBaseAccelerationEnabledElement;
        WsFacadeElement<WsTypes::WsAutonomousProgramIdType>*  ap_lockedAutoProgramIdElement;
        
    private:
        WsDataIndicationFacade(const WsDataIndicationFacade& rc_rhs);
        const WsDataIndicationFacade& operator=(const WsDataIndicationFacade& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

