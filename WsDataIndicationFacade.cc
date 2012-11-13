#include "WsDataIndicationFacade.hh"
#include "WsLogger.hh"
#include "Synchronized.h"

WsDataIndicationFacade* WsDataIndicationFacade::asp_instance = NULL;
WsDataIndicationFacade::WsDataIndicationFacade(void)
    : ap_mutexSemaphore(NULL)
    , ap_flywheelPidSetpointElement(NULL)
    , ap_flywheelPidEnabledElement(NULL)
    , ap_flywheelPidControllerOutputElement(NULL)
    , ap_flywheelAtTargetSpeedElement(NULL)
    , ap_flywheelNearTargetSpeedElement(NULL)
    , ap_turretAtTargetPositionElement(NULL)
    , ap_turretNearTargetPositionElement(NULL)
    , ap_turretHeadingAngleElement(NULL)
    , ap_turretPidSetpointElement(NULL)
    , ap_turretPidEnabledElement(NULL)
    , ap_driveBaseAntiTurboEnabledElement(NULL)
    , ap_driveBaseAccelerationEnabledElement(NULL)
    , ap_lockedAutoProgramIdElement(NULL)
{
    // Create a mutex to protect our data
    ap_mutexSemaphore = semMCreate(SEM_Q_PRIORITY |
                                   SEM_DELETE_SAFE |
                                   SEM_INVERSION_SAFE);
                                   
                                   
    //-------------------------------------------------------------------------
    // Create All Facade Data Elements
    //-------------------------------------------------------------------------
    float default_pid_setpoint = 0.0f;
    ap_flywheelPidSetpointElement =
        new WsFacadeElement<float>("FlywheelSetpointElement",
                                   sizeof(default_pid_setpoint),
                                   &default_pid_setpoint,
                                   ap_mutexSemaphore);
                                   
    bool default_pid_enabled_flag = false;
    ap_flywheelPidEnabledElement =
        new WsFacadeElement<bool>("FlywheelPidEnabled",
                                  sizeof(default_pid_enabled_flag),
                                  &default_pid_enabled_flag,
                                  ap_mutexSemaphore);
                                  
    float default_pid_output = 0.0f;
    ap_flywheelPidControllerOutputElement =
        new WsFacadeElement<float>("FlywheelControllerOutputElement",
                                   sizeof(default_pid_output),
                                   &default_pid_output,
                                   ap_mutexSemaphore);
                                   
    bool default_fats_flag = false;
    ap_flywheelAtTargetSpeedElement =
        new WsFacadeElement<bool>("FlywheelAtTargetSpeedFlag",
                                  sizeof(default_fats_flag),
                                  &default_fats_flag,
                                  ap_mutexSemaphore);
                                  
    bool default_fnts_flag = false;
    ap_flywheelNearTargetSpeedElement =
        new WsFacadeElement<bool>("FlywheelNearTargetSpeedFlag",
                                  sizeof(default_fnts_flag),
                                  &default_fnts_flag,
                                  ap_mutexSemaphore);
                                  
    bool default_tatp_flag = false;
    ap_turretAtTargetPositionElement =
        new WsFacadeElement<bool>("TurretAtTargetPositionFlag",
                                  sizeof(default_tatp_flag),
                                  &default_tatp_flag,
                                  ap_mutexSemaphore);
                                  
    bool default_tntp_flag = false;
    ap_turretNearTargetPositionElement =
        new WsFacadeElement<bool>("TurretNearTargetPositionFlag",
                                  sizeof(default_tntp_flag),
                                  &default_tntp_flag,
                                  ap_mutexSemaphore);
                                  
    float default_heading_angle = 0.0f;
    ap_turretHeadingAngleElement =
        new WsFacadeElement<float>("TurretHeadingAngle",
                                   sizeof(default_heading_angle),
                                   &default_heading_angle,
                                   ap_mutexSemaphore);
                                   
    ap_turretPidSetpointElement =
        new WsFacadeElement<float>("TurretPidSetpoint",
                                   sizeof(default_pid_setpoint),
                                   &default_pid_setpoint,
                                   ap_mutexSemaphore);
                                   
    ap_turretPidEnabledElement =
        new WsFacadeElement<bool>("TurretPidEnabled",
                                  sizeof(default_pid_enabled_flag),
                                  &default_pid_enabled_flag,
                                  ap_mutexSemaphore);
                                  
    bool default_dbat_flag = false;
    ap_driveBaseAntiTurboEnabledElement =
        new WsFacadeElement<bool>("DriveBaseAntiTurboEnabled",
                                  sizeof(default_dbat_flag),
                                  &default_dbat_flag,
                                  ap_mutexSemaphore);
                                  
    bool default_dbae_flag = true;
    ap_driveBaseAccelerationEnabledElement =
        new WsFacadeElement<bool>("DriveBaseAccelerationEnabled",
                                  sizeof(default_dbae_flag),
                                  &default_dbae_flag,
                                  ap_mutexSemaphore);
                                  
                                  
    // @TODO:  Move the non-locked auto-program ID here.
    WsTypes::WsAutonomousProgramIdType default_id = WsTypes::WS_AUTO_PROGRAM_SLEEPER;
    ap_lockedAutoProgramIdElement =
        new WsFacadeElement<WsTypes::WsAutonomousProgramIdType>("LockedAutoProgramId",
                                                                sizeof(default_id),
                                                                &default_id,
                                                                ap_mutexSemaphore);
}

WsDataIndicationFacade::~WsDataIndicationFacade(void)
{
    // Tear down our element data objects
    delete ap_flywheelPidSetpointElement;            ap_flywheelPidSetpointElement = NULL;
    delete ap_flywheelPidEnabledElement;             ap_flywheelPidEnabledElement = NULL;
    delete ap_flywheelAtTargetSpeedElement;          ap_flywheelAtTargetSpeedElement = NULL;
    delete ap_flywheelNearTargetSpeedElement;        ap_flywheelNearTargetSpeedElement = NULL;
    delete ap_turretAtTargetPositionElement;         ap_turretAtTargetPositionElement = NULL;
    delete ap_turretNearTargetPositionElement;       ap_turretNearTargetPositionElement = NULL;
    delete ap_turretHeadingAngleElement;             ap_turretHeadingAngleElement = NULL;
    delete ap_turretPidSetpointElement;              ap_turretPidSetpointElement = NULL;
    delete ap_turretPidEnabledElement;               ap_turretPidEnabledElement = NULL;
    delete ap_driveBaseAntiTurboEnabledElement;      ap_driveBaseAntiTurboEnabledElement = NULL;
    delete ap_driveBaseAccelerationEnabledElement;   ap_driveBaseAccelerationEnabledElement = NULL;
    delete ap_lockedAutoProgramIdElement;            ap_lockedAutoProgramIdElement = NULL;
    
    // Let's clean up our mutex nicely
    semFlush(ap_mutexSemaphore);
    semDelete(ap_mutexSemaphore);
    ap_mutexSemaphore = NULL;
}

WsDataIndicationFacade*
WsDataIndicationFacade::instance(void)
{
    if (NULL == asp_instance)
    { asp_instance = new WsDataIndicationFacade(); }
    return asp_instance;
}

void
WsDataIndicationFacade::update(void)
{
    // Nothing to do here... this facade is just a data store
}


//-----------------------------------------------------------------------------
// Accessor Methods (Get/Set)
//-----------------------------------------------------------------------------
WsSubject*
WsDataIndicationFacade::getFlywheelPidSetpoint(float& r_value)
{
    ap_flywheelPidSetpointElement->getState(r_value);
    return ap_flywheelPidSetpointElement;
}

void
WsDataIndicationFacade::setFlywheelPidSetpoint(const float& rc_value)
{
    ap_flywheelPidSetpointElement->setState(rc_value);
}

WsSubject*
WsDataIndicationFacade::getFlywheelPidEnabled(bool& r_value)
{
    ap_flywheelPidEnabledElement->getState(r_value);
    return ap_flywheelPidEnabledElement;
}

void
WsDataIndicationFacade::setFlywheelPidEnabled(const bool& rc_value)
{
    ap_flywheelPidEnabledElement->setState(rc_value);
}

WsSubject*
WsDataIndicationFacade::getFlywheelPidControllerOutput(float& r_value)
{
    ap_flywheelPidControllerOutputElement->getState(r_value);
    return ap_flywheelPidControllerOutputElement;
}

void
WsDataIndicationFacade::setFlywheelPidControllerOutput(const float& rc_value)
{
    ap_flywheelPidControllerOutputElement->setState(rc_value);
}

WsSubject*
WsDataIndicationFacade::getFlywheelAtTargetSpeedFlag(bool& r_flag)
{
    ap_flywheelAtTargetSpeedElement->getState(r_flag);
    return ap_flywheelAtTargetSpeedElement;
}

void
WsDataIndicationFacade::setFlywheelAtTargetSpeedFlag(const bool& rc_flag)
{
    ap_flywheelAtTargetSpeedElement->setState(rc_flag);
}

WsSubject*
WsDataIndicationFacade::getFlywheelNearTargetSpeedFlag(bool& r_flag)
{
    ap_flywheelNearTargetSpeedElement->getState(r_flag);
    return ap_flywheelNearTargetSpeedElement;
}

void
WsDataIndicationFacade::setFlywheelNearTargetSpeedFlag(const bool& rc_flag)
{
    ap_flywheelNearTargetSpeedElement->setState(rc_flag);
}

WsSubject*
WsDataIndicationFacade::getTurretAtTargetPositionFlag(bool& r_flag)
{
    ap_turretAtTargetPositionElement->getState(r_flag);
    return ap_turretAtTargetPositionElement;
}

void
WsDataIndicationFacade::setTurretAtTargetPositionFlag(const bool& rc_flag)
{
    ap_turretAtTargetPositionElement->setState(rc_flag);
}

WsSubject*
WsDataIndicationFacade::getTurretNearTargetPositionFlag(bool& r_flag)
{
    ap_turretNearTargetPositionElement->getState(r_flag);
    return ap_turretNearTargetPositionElement;
}

void
WsDataIndicationFacade::setTurretNearTargetPositionFlag(const bool& rc_flag)
{
    ap_turretNearTargetPositionElement->setState(rc_flag);
}

WsSubject*
WsDataIndicationFacade::getTurretHeadingAngle(float& r_value)
{
    ap_turretHeadingAngleElement->getState(r_value);
    return ap_turretHeadingAngleElement;
}

void
WsDataIndicationFacade::setTurretHeadingAngle(const float& rc_value)
{
    ap_turretHeadingAngleElement->setState(rc_value);
}

WsSubject*
WsDataIndicationFacade::getTurretPidSetpoint(float& r_value)
{
    ap_turretPidSetpointElement->getState(r_value);
    return ap_turretPidSetpointElement;
}

void
WsDataIndicationFacade::setTurretPidSetpoint(const float& rc_value)
{
    ap_turretPidSetpointElement->setState(rc_value);
}

WsSubject*
WsDataIndicationFacade::getTurretPidEnabled(bool& r_flag)
{
    ap_turretPidEnabledElement->getState(r_flag);
    return ap_turretPidEnabledElement;
}

void
WsDataIndicationFacade::setTurretPidEnabled(const bool& rc_flag)
{
    ap_turretPidEnabledElement->setState(rc_flag);
}

WsSubject*
WsDataIndicationFacade::getDriveBaseAntiTurboEnabled(bool& r_flag)
{
    ap_driveBaseAntiTurboEnabledElement->getState(r_flag);
    return ap_driveBaseAntiTurboEnabledElement;
}

void
WsDataIndicationFacade::setDriveBaseAntiTurboEnabled(const bool& rc_flag)
{
    ap_driveBaseAntiTurboEnabledElement->setState(rc_flag);
}

WsSubject*
WsDataIndicationFacade::getDriveBaseAccelerationEnabled(bool& r_flag)
{
    ap_driveBaseAccelerationEnabledElement->getState(r_flag);
    return ap_driveBaseAccelerationEnabledElement;
}

void
WsDataIndicationFacade::setDriveBaseAccelerationEnabled(const bool& rc_flag)
{
    ap_driveBaseAccelerationEnabledElement->setState(rc_flag);
}

WsSubject*
WsDataIndicationFacade::getLockedAutoProgramId(WsTypes::WsAutonomousProgramIdType& r_value)
{
    ap_lockedAutoProgramIdElement->getState(r_value);
    return ap_lockedAutoProgramIdElement;
}

void
WsDataIndicationFacade::setLockedAutoProgramId(const WsTypes::WsAutonomousProgramIdType& rc_value)
{
    ap_lockedAutoProgramIdElement->setState(rc_value);
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

