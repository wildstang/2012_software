#include "WsDashboardUpdaterObservers.hh"
#include "WsInputFacade.hh"
#include "WsOutputFacade.hh"
#include "WsConfigFacade.hh"
#include "WsDataIndicationFacade.hh"
#include "WsDashboardUpdater.hh"
#include "WsLogger.hh"


//-----------------------------------------------------------------------------
//
//                #######
//                #  #  #                                   #
//                   #                                      #
//                   #    ##  ##  ### ##  ### ##   #####   ####
//                   #     #   #    ##  #   ##  # #     #   #
//                   #     #   #    #       #     #######   #
//                   #     #   #    #       #     #         #
//                   #     #  ##    #       #     #     #   #  #
//                  ###     ## ## #####   #####    #####     ##
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// WsDashboardUpdaterTurretAtPositionObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterTurretAtPositionObserver::WsDashboardUpdaterTurretAtPositionObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterTurretAtPositionObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getTurretAtTargetPositionFlag(a_currentState);
    p_subject->attach(this);
    ap_subsystem->acceptNewTurretAtPositionFlag(a_currentState);
}

WsDashboardUpdaterTurretAtPositionObserver::~WsDashboardUpdaterTurretAtPositionObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getTurretAtTargetPositionFlag(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterTurretAtPositionObserver::update(void)
{
    WsDataIndicationFacade::instance()->getTurretAtTargetPositionFlag(a_currentState);
    ap_subsystem->acceptNewTurretAtPositionFlag(a_currentState);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterTurretNearPositionObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterTurretNearPositionObserver::WsDashboardUpdaterTurretNearPositionObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterTurretNearPositionObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getTurretNearTargetPositionFlag(a_currentState);
    p_subject->attach(this);
    ap_subsystem->acceptNewTurretNearPositionFlag(a_currentState);
}

WsDashboardUpdaterTurretNearPositionObserver::~WsDashboardUpdaterTurretNearPositionObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getTurretNearTargetPositionFlag(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterTurretNearPositionObserver::update(void)
{
    WsDataIndicationFacade::instance()->getTurretNearTargetPositionFlag(a_currentState);
    ap_subsystem->acceptNewTurretNearPositionFlag(a_currentState);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterTurretMotorOutputObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterTurretMotorOutputObserver::WsDashboardUpdaterTurretMotorOutputObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterTurretMotorOutputObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsOutputFacade::instance()->getTurretHeadingSpeed(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewTurretMotorOutput(a_currentValue);
}

WsDashboardUpdaterTurretMotorOutputObserver::~WsDashboardUpdaterTurretMotorOutputObserver(void)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getTurretHeadingSpeed(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterTurretMotorOutputObserver::update(void)
{
    WsOutputFacade::instance()->getTurretHeadingSpeed(a_currentValue);
    ap_subsystem->acceptNewTurretMotorOutput(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterTurretHeadingAngleObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterTurretHeadingAngleObserver::WsDashboardUpdaterTurretHeadingAngleObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterTurretHeadingAngleObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getTurretHeadingAngle(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewTurretHeadingAngle(a_currentValue);
}

WsDashboardUpdaterTurretHeadingAngleObserver::~WsDashboardUpdaterTurretHeadingAngleObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getTurretHeadingAngle(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterTurretHeadingAngleObserver::update(void)
{
    WsDataIndicationFacade::instance()->getTurretHeadingAngle(a_currentValue);
    ap_subsystem->acceptNewTurretHeadingAngle(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterTurretHeadingPotValueObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterTurretHeadingPotValueObserver::WsDashboardUpdaterTurretHeadingPotValueObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterTurretHeadingPotValueObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getTurretHeadingPotValue(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewTurretHeadingPotValue(a_currentValue);
}

WsDashboardUpdaterTurretHeadingPotValueObserver::~WsDashboardUpdaterTurretHeadingPotValueObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getTurretHeadingPotValue(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterTurretHeadingPotValueObserver::update(void)
{
    WsInputFacade::instance()->getTurretHeadingPotValue(a_currentValue);
    ap_subsystem->acceptNewTurretHeadingPotValue(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterTurretPidSetpointObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterTurretPidSetpointObserver::WsDashboardUpdaterTurretPidSetpointObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterTurretPidSetpointObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getTurretPidSetpoint(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewTurretPidSetpoint(a_currentValue);
}

WsDashboardUpdaterTurretPidSetpointObserver::~WsDashboardUpdaterTurretPidSetpointObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getTurretPidSetpoint(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterTurretPidSetpointObserver::update(void)
{
    WsDataIndicationFacade::instance()->getTurretPidSetpoint(a_currentValue);
    ap_subsystem->acceptNewTurretPidSetpoint(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterTurretPidEnabledObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterTurretPidEnabledObserver::WsDashboardUpdaterTurretPidEnabledObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterTurretPidEnabledObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getTurretPidEnabled(a_currentState);
    p_subject->attach(this);
    ap_subsystem->acceptNewTurretPidEnabledFlag(a_currentState);
}

WsDashboardUpdaterTurretPidEnabledObserver::~WsDashboardUpdaterTurretPidEnabledObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getTurretPidEnabled(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterTurretPidEnabledObserver::update(void)
{
    WsDataIndicationFacade::instance()->getTurretPidEnabled(a_currentState);
    ap_subsystem->acceptNewTurretPidEnabledFlag(a_currentState);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterTurretPidConfigObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterTurretPidConfigObserver::
WsDashboardUpdaterTurretPidConfigObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterTurretPidConfigObserver")
    , ap_subsystem(p_subsystem)
    , a_currentConfig()
{
    // Zero-out the current config
    ::memset(&a_currentConfig, 0x00, sizeof(a_currentConfig));
    
    WsSubject* p_subject =
        WsConfigFacade::instance()->getTurretPidConfig(a_currentConfig);
    p_subject->attach(this);
    ap_subsystem->acceptNewTurretPidConfig(a_currentConfig);
}

WsDashboardUpdaterTurretPidConfigObserver::
~WsDashboardUpdaterTurretPidConfigObserver(void)
{
    WsSubject* p_subject =
        WsConfigFacade::instance()->getTurretPidConfig(a_currentConfig);
    p_subject->detach(this);
}

void
WsDashboardUpdaterTurretPidConfigObserver::update(void)
{
    WsConfigFacade::instance()->getTurretPidConfig(a_currentConfig);
    ap_subsystem->acceptNewTurretPidConfig(a_currentConfig);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterRobotHeadingGyroAngleObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterRobotHeadingGyroAngleObserver::WsDashboardUpdaterRobotHeadingGyroAngleObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterRobotHeadingGyroAngleObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getRobotHeadingGyroAngle(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewRobotHeadingGyroAngle(a_currentValue);
}

WsDashboardUpdaterRobotHeadingGyroAngleObserver::~WsDashboardUpdaterRobotHeadingGyroAngleObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getRobotHeadingGyroAngle(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterRobotHeadingGyroAngleObserver::update(void)
{
    WsInputFacade::instance()->getRobotHeadingGyroAngle(a_currentValue);
    ap_subsystem->acceptNewRobotHeadingGyroAngle(a_currentValue);
}


//-----------------------------------------------------------------------------
//
//         ##                            ######
//          #              #              #    #
//          #              #              #    #
//         # #   ##  ##   ####    #####   #    # ### ##   ###### ### #
//         # #    #   #    #     #     #  #####    ##  # #    #   # # #
//        #   #   #   #    #     #     #  #        #     #    #   # # #
//        #####   #   #    #     #     #  #        #     #    #   # # #
//        #   #   #  ##    #  #  #     #  #        #      #####   # # #
//       ### ###   ## ##    ##    #####  ####    #####        #  ## # ##
//                                                      #
//                                                        ####
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// WsDashboardUpdaterAutoProgramLockInObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterAutoProgramLockInObserver::WsDashboardUpdaterAutoProgramLockInObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterAutoProgramLockInObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getLockInSwitchState(a_currentState);
    p_subject->attach(this);
    ap_subsystem->acceptNewAutonomousLockInState(a_currentState);
}

WsDashboardUpdaterAutoProgramLockInObserver::~WsDashboardUpdaterAutoProgramLockInObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getLockInSwitchState(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterAutoProgramLockInObserver::update(void)
{
    WsInputFacade::instance()->getLockInSwitchState(a_currentState);
    ap_subsystem->acceptNewAutonomousLockInState(a_currentState);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterAutoProgramObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterAutoProgramObserver::WsDashboardUpdaterAutoProgramObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterAutoProgramObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(WsTypes::WS_AUTO_PROGRAM_SLEEPER)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsInputFacade::instance()->getAutonomousProgramId(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewAutonomousProgramId((INT32)a_currentValue);
}

WsDashboardUpdaterAutoProgramObserver::~WsDashboardUpdaterAutoProgramObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getAutonomousProgramId(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterAutoProgramObserver::update(void)
{
    WsInputFacade::instance()->getAutonomousProgramId(a_currentValue);
    ap_subsystem->acceptNewAutonomousProgramId((INT32)a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterLockedAutoProgramIdObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterLockedAutoProgramIdObserver::WsDashboardUpdaterLockedAutoProgramIdObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterLockedAutoProgramIdObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(WsTypes::WS_AUTO_PROGRAM_SLEEPER)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getLockedAutoProgramId(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewLockedAutoProgramId((INT32)a_currentValue);
}

WsDashboardUpdaterLockedAutoProgramIdObserver::~WsDashboardUpdaterLockedAutoProgramIdObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getLockedAutoProgramId(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterLockedAutoProgramIdObserver::update(void)
{
    WsDataIndicationFacade::instance()->getLockedAutoProgramId(a_currentValue);
    ap_subsystem->acceptNewLockedAutoProgramId((INT32)a_currentValue);
}


//-----------------------------------------------------------------------------
//
//        #######   ##                    ##                        ##
//         #    #    #                     #                         #
//         #         #                     #                         #
//         #  #      #    ### ### ### ###  # ##    #####   #####     #
//         ####      #     #   #   #   #   ##  #  #     # #     #    #
//         #  #      #     #   #   # # #   #   #  ####### #######    #
//         #         #      # #    # # #   #   #  #       #          #
//         #         #      # #     # #    #   #  #     # #     #    #
//        ####     #####     #      # #   ### ###  #####   #####   #####
//                           #
//                         ##
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelAtSpeedObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelAtSpeedObserver::WsDashboardUpdaterFlywheelAtSpeedObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelAtSpeedObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getFlywheelAtTargetSpeedFlag(a_currentState);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelAtSpeedFlag(a_currentState);
}

WsDashboardUpdaterFlywheelAtSpeedObserver::~WsDashboardUpdaterFlywheelAtSpeedObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getFlywheelAtTargetSpeedFlag(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelAtSpeedObserver::update(void)
{
    WsDataIndicationFacade::instance()->getFlywheelAtTargetSpeedFlag(a_currentState);
    ap_subsystem->acceptNewFlywheelAtSpeedFlag(a_currentState);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelNearSpeedObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelNearSpeedObserver::WsDashboardUpdaterFlywheelNearSpeedObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelNearSpeedObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getFlywheelNearTargetSpeedFlag(a_currentState);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelNearSpeedFlag(a_currentState);
}

WsDashboardUpdaterFlywheelNearSpeedObserver::~WsDashboardUpdaterFlywheelNearSpeedObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getFlywheelNearTargetSpeedFlag(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelNearSpeedObserver::update(void)
{
    WsDataIndicationFacade::instance()->getFlywheelNearTargetSpeedFlag(a_currentState);
    ap_subsystem->acceptNewFlywheelNearSpeedFlag(a_currentState);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelRotationalSpeedObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelRotationalSpeedObserver::WsDashboardUpdaterFlywheelRotationalSpeedObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelRotationalSpeedObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelRotationalSpeed(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelRotationalSpeed(a_currentValue);
}

WsDashboardUpdaterFlywheelRotationalSpeedObserver::~WsDashboardUpdaterFlywheelRotationalSpeedObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelRotationalSpeed(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelRotationalSpeedObserver::update(void)
{
    WsInputFacade::instance()->getFlywheelRotationalSpeed(a_currentValue);
    ap_subsystem->acceptNewFlywheelRotationalSpeed(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelPidSetpointObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelPidSetpointObserver::WsDashboardUpdaterFlywheelPidSetpointObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelPidSetpointObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getFlywheelPidSetpoint(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelPidSetpoint(a_currentValue);
}

WsDashboardUpdaterFlywheelPidSetpointObserver::~WsDashboardUpdaterFlywheelPidSetpointObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getFlywheelPidSetpoint(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelPidSetpointObserver::update(void)
{
    WsDataIndicationFacade::instance()->getFlywheelPidSetpoint(a_currentValue);
    ap_subsystem->acceptNewFlywheelPidSetpoint(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelPidEnabledObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelPidEnabledObserver::WsDashboardUpdaterFlywheelPidEnabledObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelPidEnabledObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    // Attach to the subject for future updates
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getFlywheelPidEnabled(a_currentState);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelPidEnabledFlag(a_currentState);
}

WsDashboardUpdaterFlywheelPidEnabledObserver::~WsDashboardUpdaterFlywheelPidEnabledObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getFlywheelPidEnabled(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelPidEnabledObserver::update(void)
{
    WsDataIndicationFacade::instance()->getFlywheelPidEnabled(a_currentState);
    ap_subsystem->acceptNewFlywheelPidEnabledFlag(a_currentState);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelPidControllerOutputObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelPidControllerOutputObserver::WsDashboardUpdaterFlywheelPidControllerOutputObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelPidControllerOutputObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getFlywheelPidControllerOutput(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelPidControllerOutput(a_currentValue);
}

WsDashboardUpdaterFlywheelPidControllerOutputObserver::~WsDashboardUpdaterFlywheelPidControllerOutputObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getFlywheelPidControllerOutput(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelPidControllerOutputObserver::update(void)
{
    WsDataIndicationFacade::instance()->getFlywheelPidControllerOutput(a_currentValue);
    ap_subsystem->acceptNewFlywheelPidControllerOutput(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelMotorOutputObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelMotorOutputObserver::WsDashboardUpdaterFlywheelMotorOutputObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelMotorOutputObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getFlywheelMotorSpeed(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelMotorOutput(a_currentValue);
}

WsDashboardUpdaterFlywheelMotorOutputObserver::~WsDashboardUpdaterFlywheelMotorOutputObserver(void)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getFlywheelMotorSpeed(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelMotorOutputObserver::update(void)
{
    WsOutputFacade::instance()->getFlywheelMotorSpeed(a_currentValue);
    ap_subsystem->acceptNewFlywheelMotorOutput(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFloodLightStateObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFloodLightStateObserver::WsDashboardUpdaterFloodLightStateObserver(WsDashboardUpdater* p_shooterFlywheel)
    : WsObserver("WsDashboardUpdaterFloodLightStateObserver")
    , ap_subsystem(p_shooterFlywheel)
    , a_currentState(false)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFloodLightButtonState(a_currentState);
    p_subject->attach(this);
    ap_subsystem->acceptNewFloodlightState(a_currentState);
}

WsDashboardUpdaterFloodLightStateObserver::~WsDashboardUpdaterFloodLightStateObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFloodLightButtonState(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFloodLightStateObserver::update(void)
{
    WsInputFacade::instance()->getFloodLightButtonState(a_currentState);
    ap_subsystem->acceptNewFloodlightState(a_currentState);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFreshImageObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFreshImageObserver::WsDashboardUpdaterFreshImageObserver(WsDashboardUpdater* p_shooterFlywheel)
    : WsObserver("WsDashboardUpdaterFreshImageObserver")
    , ap_subsystem(p_shooterFlywheel)
    , a_currentState(false)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFreshImageFlag(a_currentState);
    p_subject->attach(this);
    ap_subsystem->acceptNewFreshImageFlag(a_currentState);
}

WsDashboardUpdaterFreshImageObserver::~WsDashboardUpdaterFreshImageObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFreshImageFlag(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFreshImageObserver::update(void)
{
    WsInputFacade::instance()->getFreshImageFlag(a_currentState);
    ap_subsystem->acceptNewFreshImageFlag(a_currentState);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterLocationSpeedMappingObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterLocationSpeedMappingObserver::WsDashboardUpdaterLocationSpeedMappingObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterLocationSpeedMappingObserver")
    , ap_subsystem(p_subsystem)
    , a_currentConfig()
{
    WsSubject* p_subject =
        WsConfigFacade::instance()->getFlywheelLocationSpeedMapping(a_currentConfig);
    p_subject->attach(this);
    ap_subsystem->acceptNewLocationSpeedMapping(a_currentConfig);
}

WsDashboardUpdaterLocationSpeedMappingObserver::~WsDashboardUpdaterLocationSpeedMappingObserver(void)
{
    WsSubject* p_subject =
        WsConfigFacade::instance()->getFlywheelLocationSpeedMapping(a_currentConfig);
    p_subject->detach(this);
}

void
WsDashboardUpdaterLocationSpeedMappingObserver::update(void)
{
    WsConfigFacade::instance()->getFlywheelLocationSpeedMapping(a_currentConfig);
    ap_subsystem->acceptNewLocationSpeedMapping(a_currentConfig);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelManualPotObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelManualPotObserver::WsDashboardUpdaterFlywheelManualPotObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelManualPotObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelManualPot(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelManualPotValue(a_currentValue);
}

WsDashboardUpdaterFlywheelManualPotObserver::~WsDashboardUpdaterFlywheelManualPotObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelManualPot(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelManualPotObserver::update(void)
{
    WsInputFacade::instance()->getFlywheelManualPot(a_currentValue);
    ap_subsystem->acceptNewFlywheelManualPotValue(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelStatePotObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelStatePotObserver::WsDashboardUpdaterFlywheelStatePotObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelStatePotObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelStatePotValue(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelStatePotValue(a_currentValue);
}

WsDashboardUpdaterFlywheelStatePotObserver::~WsDashboardUpdaterFlywheelStatePotObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelStatePotValue(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelStatePotObserver::update(void)
{
    WsInputFacade::instance()->getFlywheelStatePotValue(a_currentValue);
    ap_subsystem->acceptNewFlywheelStatePotValue(a_currentValue);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelPidConfigObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelPidConfigObserver::WsDashboardUpdaterFlywheelPidConfigObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelPidConfigObserver")
    , ap_subsystem(p_subsystem)
    , a_currentConfig()
{
    // Zero-out the current config
    ::memset(&a_currentConfig, 0x00, sizeof(a_currentConfig));
    
    WsSubject* p_subject =
        WsConfigFacade::instance()->getFlywheelPidConfig(a_currentConfig);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelPidConfig(a_currentConfig);
}

WsDashboardUpdaterFlywheelPidConfigObserver::~WsDashboardUpdaterFlywheelPidConfigObserver(void)
{
    WsSubject* p_subject =
        WsConfigFacade::instance()->getFlywheelPidConfig(a_currentConfig);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelPidConfigObserver::update(void)
{
    WsConfigFacade::instance()->getFlywheelPidConfig(a_currentConfig);
    ap_subsystem->acceptNewFlywheelPidConfig(a_currentConfig);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver::
WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(false)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelSpeedCompensationEnableSwitchState(a_currentState);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelCompensationSwitchState(a_currentState);
}

WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver::
~WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelSpeedCompensationEnableSwitchState(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver::update(void)
{
    WsInputFacade::instance()->getFlywheelSpeedCompensationEnableSwitchState(a_currentState);
    ap_subsystem->acceptNewFlywheelCompensationSwitchState(a_currentState);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver::
WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelSpeedCompensationPotValue(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewFlywheelCompensationPotValue(a_currentValue);
}

WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver::
~WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getFlywheelSpeedCompensationPotValue(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver::update(void)
{
    WsInputFacade::instance()->getFlywheelSpeedCompensationPotValue(a_currentValue);
    ap_subsystem->acceptNewFlywheelCompensationPotValue(a_currentValue);
}



//-----------------------------------------------------------------------------
//
// ### ###
//  #   #     #               #
//  #   #
//  #   #   ###     #####   ###     #####  ## ##
//   # #      #    #     #    #    #     #  ##  #
//   # #      #     ###       #    #     #  #   #
//   # #      #        ##     #    #     #  #   #
//    #       #    #     #    #    #     #  #   #
//    #     #####   #####   #####   #####  ### ###
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// WsDashboardUpdaterVisionNumVisibleTargetsObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterVisionNumVisibleTargetsObserver::WsDashboardUpdaterVisionNumVisibleTargetsObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterVisionNumVisibleTargetsObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue()
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getNumVisibleTargets(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewNumVisibleTargets(a_currentValue);
}

WsDashboardUpdaterVisionNumVisibleTargetsObserver::~WsDashboardUpdaterVisionNumVisibleTargetsObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getNumVisibleTargets(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterVisionNumVisibleTargetsObserver::update(void)
{
    WsInputFacade::instance()->getNumVisibleTargets(a_currentValue);
    ap_subsystem->acceptNewNumVisibleTargets(a_currentValue);
}


//-----------------------------------------------------------------------------
//
//   #####              #                    ######
//    #   #                                   #    #
//    #    #                                  #    #
//    #    # ### ##   ###    ### ###  #####   #    #  ####    #####   #####
//    #    #   ##  #    #     #   #  #     #  #####       #  #     # #     #
//    #    #   #        #     #   #  #######  #    #  #####   ###    #######
//    #    #   #        #      # #   #        #    # #    #      ##  #
//    #   #    #        #      # #   #     #  #    # #    #  #     # #     #
//   #####   #####    #####     #     #####  ######   #### #  #####   #####
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseLeftEncoderObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseLeftEncoderObserver::
WsDashboardUpdaterDriveBaseLeftEncoderObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseLeftEncoderObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getLeftDriveEncoderCount(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseLeftEncoderObserver::
~WsDashboardUpdaterDriveBaseLeftEncoderObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getLeftDriveEncoderCount(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseLeftEncoderObserver::update(void)
{
    WsInputFacade::instance()->getLeftDriveEncoderCount(a_currentValue);
    
    ap_subsystem->acceptNewLeftEncoderValue(a_currentValue);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseRightEncoderObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseRightEncoderObserver::
WsDashboardUpdaterDriveBaseRightEncoderObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseRightEncoderObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getRightDriveEncoderCount(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseRightEncoderObserver::~WsDashboardUpdaterDriveBaseRightEncoderObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getRightDriveEncoderCount(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseRightEncoderObserver::update(void)
{
    WsInputFacade::instance()->getRightDriveEncoderCount(a_currentValue);
    ap_subsystem->acceptNewRightEncoderValue(a_currentValue);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver::
WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getLeftDriveMotorSpeed(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver::
~WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver(void)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getLeftDriveMotorSpeed(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver::update(void)
{
    WsOutputFacade::instance()->getLeftDriveMotorSpeed(a_currentValue);
    ap_subsystem->acceptNewLeftDriveMotorOutput(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver::
WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getRightDriveMotorSpeed(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver::
~WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver(void)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getRightDriveMotorSpeed(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver::update(void)
{
    WsOutputFacade::instance()->getRightDriveMotorSpeed(a_currentValue);
    ap_subsystem->acceptNewRightDriveMotorOutput(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseShifterStateObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseShifterStateObserver::
WsDashboardUpdaterDriveBaseShifterStateObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseShifterStateObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(WsTypes::WS_S_LOWGEAR)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getShifterState(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseShifterStateObserver::
~WsDashboardUpdaterDriveBaseShifterStateObserver(void)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getShifterState(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseShifterStateObserver::update(void)
{
    WsOutputFacade::instance()->getShifterState(a_currentValue);
    ap_subsystem->acceptNewShifterState(a_currentValue);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver::
WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(false)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getDriveBaseAntiTurboEnabled(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver::
~WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getDriveBaseAntiTurboEnabled(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver::update(void)
{
    WsDataIndicationFacade::instance()->getDriveBaseAntiTurboEnabled(a_currentValue);
    ap_subsystem->acceptNewAntiTurboEnabledFlag(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver::
WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(WsTypes::WS_BRIDGE_BRAKE_CYLINDER_DISENGAGED)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getBridgeBrakeCylinderState(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver::
~WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver(void)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getBridgeBrakeCylinderState(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver::update(void)
{
    WsOutputFacade::instance()->getBridgeBrakeCylinderState(a_currentValue);
    ap_subsystem->acceptNewBridgeBrakeState(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver::
WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver")
    , ap_subsystem(p_subsystem)
    , a_currentInnerValue(WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED)
    , a_currentOuterValue(WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getRobotGrabberInnerCylinderState(a_currentInnerValue);
    p_subject->attach(this);
    
    p_subject =
        WsOutputFacade::instance()->getRobotGrabberOuterCylinderState(a_currentOuterValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver::
~WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver(void)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getRobotGrabberInnerCylinderState(a_currentInnerValue);
    p_subject->detach(this);
    
    p_subject =
        WsOutputFacade::instance()->getRobotGrabberOuterCylinderState(a_currentOuterValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver::update(void)
{
    WsOutputFacade::instance()->getRobotGrabberInnerCylinderState(a_currentInnerValue);
    WsOutputFacade::instance()->getRobotGrabberOuterCylinderState(a_currentOuterValue);
    
    // If either cylinder is extended, then push "extended" to the subsystem
    WsTypes::WsRobotGrabberCylinderStateType result =
        ((WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED == a_currentInnerValue) ||
         (WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED == a_currentOuterValue))
        ? WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED
        : WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
        
    ap_subsystem->acceptNewRobotGrabberState(result);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBasePogoEnabledObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBasePogoEnabledObserver::
WsDashboardUpdaterDriveBasePogoEnabledObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBasePogoEnabledObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(WsTypes::WS_POGO_STICK_CYLINDER_UP)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getPogoStickCylinderState(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBasePogoEnabledObserver::
~WsDashboardUpdaterDriveBasePogoEnabledObserver(void)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getPogoStickCylinderState(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBasePogoEnabledObserver::update(void)
{
    WsOutputFacade::instance()->getPogoStickCylinderState(a_currentValue);
    ap_subsystem->acceptNewPogoState(a_currentValue);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseDriverThrottleValueObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseDriverThrottleValueObserver::
WsDashboardUpdaterDriveBaseDriverThrottleValueObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseDriverThrottleValueObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getDriverThrottleValue(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseDriverThrottleValueObserver::
~WsDashboardUpdaterDriveBaseDriverThrottleValueObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getDriverThrottleValue(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseDriverThrottleValueObserver::update(void)
{
    WsInputFacade::instance()->getDriverThrottleValue(a_currentValue);
    ap_subsystem->acceptNewDriverThrottleValue(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseDriverHeadingValueObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseDriverHeadingValueObserver::
WsDashboardUpdaterDriveBaseDriverHeadingValueObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseDriverHeadingValueObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getDriverHeadingValue(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseDriverHeadingValueObserver::
~WsDashboardUpdaterDriveBaseDriverHeadingValueObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getDriverHeadingValue(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseDriverHeadingValueObserver::update(void)
{
    WsInputFacade::instance()->getDriverHeadingValue(a_currentValue);
    ap_subsystem->acceptNewDriverHeadingValue(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver::
WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseAntiTurboButtonStateObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(false)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getAntiTurboButtonState(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver::
~WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getAntiTurboButtonState(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver::update(void)
{
    WsInputFacade::instance()->getAntiTurboButtonState(a_currentValue);
    ap_subsystem->acceptNewAntiTurboButtonState(a_currentValue);
}

//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver::
WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(false)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getShifterButtonState(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver::
~WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getShifterButtonState(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver::update(void)
{
    WsInputFacade::instance()->getShifterButtonState(a_currentValue);
    ap_subsystem->acceptNewShifterButtonState(a_currentValue);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver::
WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(false)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getDriveBaseAccelerationButtonState(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewDriveBaseAccelerationEnabledFlag(a_currentValue);
}

WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver::
~WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getDriveBaseAccelerationButtonState(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver::update(void)
{
    WsInputFacade::instance()->getDriveBaseAccelerationButtonState(a_currentValue);
    ap_subsystem->acceptNewDriveBaseAccelerationButtuonState(a_currentValue);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterDriveBaseAccelerationEnabledObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterDriveBaseAccelerationEnabledObserver::
WsDashboardUpdaterDriveBaseAccelerationEnabledObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterDriveBaseAccelerationEnabledObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(false)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getDriveBaseAccelerationEnabled(a_currentValue);
    p_subject->attach(this);
    ap_subsystem->acceptNewDriveBaseAccelerationEnabledFlag(a_currentValue);
}

WsDashboardUpdaterDriveBaseAccelerationEnabledObserver::
~WsDashboardUpdaterDriveBaseAccelerationEnabledObserver(void)
{
    WsSubject* p_subject =
        WsDataIndicationFacade::instance()->getDriveBaseAccelerationEnabled(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterDriveBaseAccelerationEnabledObserver::update(void)
{
    WsDataIndicationFacade::instance()->getDriveBaseAccelerationEnabled(a_currentValue);
    ap_subsystem->acceptNewDriveBaseAccelerationEnabledFlag(a_currentValue);
}



//-----------------------------------------------------------------------------
//
//   ####
//  #    #
// #
// #        #####  ### #   ######  ### ##   #####   #####   #####   #####  ### ##
// #       #     #  # # #   #    #   ##  # #     # #     # #     # #     #   ##  #
// #       #     #  # # #   #    #   #     #######  ###     ###    #     #   #
// #       #     #  # # #   #    #   #     #           ##      ##  #     #   #
//  #    # #     #  # # #   #    #   #     #     # #     # #     # #     #   #
//   ####   #####  ## # ##  #####  #####    #####   #####   #####   #####  #####
//                          #
//                         ###
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// WsDashboardUpdaterCompressorStateObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterCompressorStateObserver::
WsDashboardUpdaterCompressorStateObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterCompressorStateObserver")
    , ap_subsystem(p_subsystem)
    , a_currentState(WsTypes::WS_COMPRESSOR_OFF)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getCompressorState(a_currentState);
    p_subject->attach(this);
}

WsDashboardUpdaterCompressorStateObserver::
~WsDashboardUpdaterCompressorStateObserver(void)
{
    WsSubject* p_subject =
        WsOutputFacade::instance()->getCompressorState(a_currentState);
    p_subject->detach(this);
}

void
WsDashboardUpdaterCompressorStateObserver::update(void)
{
    WsOutputFacade::instance()->getCompressorState(a_currentState);
    ap_subsystem->acceptNewCompressorState(a_currentState);
}


//-----------------------------------------------------------------------------
// WsDashboardUpdaterPneumaticPressureReadingObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterPneumaticPressureReadingObserver::
WsDashboardUpdaterPneumaticPressureReadingObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterPneumaticPressureReadingObserver")
    , ap_subsystem(p_subsystem)
    , a_currentValue(0.0f)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getPneumaticPressureReading(a_currentValue);
    p_subject->attach(this);
}

WsDashboardUpdaterPneumaticPressureReadingObserver::
~WsDashboardUpdaterPneumaticPressureReadingObserver(void)
{
    WsSubject* p_subject =
        WsInputFacade::instance()->getPneumaticPressureReading(a_currentValue);
    p_subject->detach(this);
}

void
WsDashboardUpdaterPneumaticPressureReadingObserver::update(void)
{
    WsInputFacade::instance()->getPneumaticPressureReading(a_currentValue);
    ap_subsystem->acceptNewPneumaticPressureReading(a_currentValue);
}


//-----------------------------------------------------------------------------
//
//     #####  ##
//    #     #  #                        #
//    #        #                        #
//    #        # ##    #####   #####   ####    #####  ### ##
//     #####   ##  #  #     # #     #   #     #     #   ##  #
//          #  #   #  #     # #     #   #     #######   #
//          #  #   #  #     # #     #   #     #         #
//    #     #  #   #  #     # #     #   #  #  #     #   #
//     #####  ### ###  #####   #####     ##    #####  #####
//
//
//    ######
//     #    #                                   #
//     #    #                                   #
//     #    # ### ##   #####   #####   #####   ####    #####
//     #####    ##  # #     # #     # #     #   #     #     #
//     #        #     #######  ###    #######   #      ###
//     #        #     #           ##  #         #         ##
//     #        #     #     # #     # #     #   #  #  #     #
//    ####    #####    #####   #####   #####     ##    #####
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// WsDashboardUpdaterShooterPresetConfigObserver
//-----------------------------------------------------------------------------
WsDashboardUpdaterShooterPresetConfigObserver::
WsDashboardUpdaterShooterPresetConfigObserver(WsDashboardUpdater* p_subsystem)
    : WsObserver("WsDashboardUpdaterShooterPresetConfigObserver")
    , ap_subsystem(p_subsystem)
    , a_currentConfig()
{
    // Zero-out the current config
    ::memset(&a_currentConfig, 0x00, sizeof(a_currentConfig));
    
    WsSubject* p_subject =
        WsConfigFacade::instance()->getShooterPresetConfig(a_currentConfig);
    p_subject->attach(this);
    ap_subsystem->acceptNewShooterPresetConfig(a_currentConfig);
}

WsDashboardUpdaterShooterPresetConfigObserver::
~WsDashboardUpdaterShooterPresetConfigObserver(void)
{
    WsSubject* p_subject =
        WsConfigFacade::instance()->getShooterPresetConfig(a_currentConfig);
    p_subject->detach(this);
}

void
WsDashboardUpdaterShooterPresetConfigObserver::update(void)
{
    WsConfigFacade::instance()->getShooterPresetConfig(a_currentConfig);
    ap_subsystem->acceptNewShooterPresetConfig(a_currentConfig);
}


//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

