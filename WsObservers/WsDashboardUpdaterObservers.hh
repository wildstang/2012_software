#ifndef __WsDashboardUpdaterObservers_hh__
#define __WsDashboardUpdaterObservers_hh__

#include "WsObserver.hh"
#include "WsTypes.hh"

class WsDashboardUpdater;


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
class WsDashboardUpdaterTurretAtPositionObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterTurretAtPositionObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterTurretAtPositionObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        bool        a_currentState;
        
    private:
        WsDashboardUpdaterTurretAtPositionObserver(const WsDashboardUpdaterTurretAtPositionObserver& rc_rhs);
        WsDashboardUpdaterTurretAtPositionObserver& operator=(const WsDashboardUpdaterTurretAtPositionObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterTurretNearPositionObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterTurretNearPositionObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterTurretNearPositionObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        bool        a_currentState;
        
    private:
        WsDashboardUpdaterTurretNearPositionObserver(const WsDashboardUpdaterTurretNearPositionObserver& rc_rhs);
        WsDashboardUpdaterTurretNearPositionObserver& operator=(const WsDashboardUpdaterTurretNearPositionObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterTurretMotorOutputObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterTurretMotorOutputObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterTurretMotorOutputObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float               a_currentValue;
        
    private:
        WsDashboardUpdaterTurretMotorOutputObserver(const WsDashboardUpdaterTurretMotorOutputObserver& rc_rhs);
        WsDashboardUpdaterTurretMotorOutputObserver& operator=(const WsDashboardUpdaterTurretMotorOutputObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterTurretHeadingAngleObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterTurretHeadingAngleObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterTurretHeadingAngleObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float               a_currentValue;
        
    private:
        WsDashboardUpdaterTurretHeadingAngleObserver(const WsDashboardUpdaterTurretHeadingAngleObserver& rc_rhs);
        WsDashboardUpdaterTurretHeadingAngleObserver& operator=(const WsDashboardUpdaterTurretHeadingAngleObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterTurretHeadingPotValueObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterTurretHeadingPotValueObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterTurretHeadingPotValueObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float               a_currentValue;
        
    private:
        WsDashboardUpdaterTurretHeadingPotValueObserver(const WsDashboardUpdaterTurretHeadingPotValueObserver& rc_rhs);
        WsDashboardUpdaterTurretHeadingPotValueObserver& operator=(const WsDashboardUpdaterTurretHeadingPotValueObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterTurretPidSetpointObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterTurretPidSetpointObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterTurretPidSetpointObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float               a_currentValue;
        
    private:
        WsDashboardUpdaterTurretPidSetpointObserver(const WsDashboardUpdaterTurretPidSetpointObserver& rc_rhs);
        WsDashboardUpdaterTurretPidSetpointObserver& operator=(const WsDashboardUpdaterTurretPidSetpointObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterTurretPidEnabledObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterTurretPidEnabledObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterTurretPidEnabledObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        bool                a_currentState;
        
    private:
        WsDashboardUpdaterTurretPidEnabledObserver(const WsDashboardUpdaterTurretPidEnabledObserver& rc_rhs);
        WsDashboardUpdaterTurretPidEnabledObserver& operator=(const WsDashboardUpdaterTurretPidEnabledObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterTurretPidConfigObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterTurretPidConfigObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterTurretPidConfigObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*               ap_subsystem;
        WsTypes::WsPidConfigurationDataT  a_currentConfig;
        
    private:
        WsDashboardUpdaterTurretPidConfigObserver(const WsDashboardUpdaterTurretPidConfigObserver& rc_rhs);
        WsDashboardUpdaterTurretPidConfigObserver& operator=(const WsDashboardUpdaterTurretPidConfigObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterRobotHeadingGyroAngleObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterRobotHeadingGyroAngleObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterRobotHeadingGyroAngleObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float               a_currentValue;
        
    private:
        WsDashboardUpdaterRobotHeadingGyroAngleObserver(const WsDashboardUpdaterRobotHeadingGyroAngleObserver& rc_rhs);
        WsDashboardUpdaterRobotHeadingGyroAngleObserver& operator=(const WsDashboardUpdaterRobotHeadingGyroAngleObserver& rc_rhs);
};


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
class WsDashboardUpdaterAutoProgramLockInObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterAutoProgramLockInObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterAutoProgramLockInObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        bool        a_currentState;
        
    private:
        WsDashboardUpdaterAutoProgramLockInObserver(const WsDashboardUpdaterAutoProgramLockInObserver& rc_rhs);
        WsDashboardUpdaterAutoProgramLockInObserver& operator=(const WsDashboardUpdaterAutoProgramLockInObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterAutoProgramObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterAutoProgramObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterAutoProgramObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        WsTypes::WsAutonomousProgramIdType a_currentValue;
        
    private:
        WsDashboardUpdaterAutoProgramObserver(const WsDashboardUpdaterAutoProgramObserver& rc_rhs);
        WsDashboardUpdaterAutoProgramObserver& operator=(const WsDashboardUpdaterAutoProgramObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterLockedAutoProgramIdObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterLockedAutoProgramIdObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterLockedAutoProgramIdObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        WsTypes::WsAutonomousProgramIdType a_currentValue;
        
    private:
        WsDashboardUpdaterLockedAutoProgramIdObserver(const WsDashboardUpdaterLockedAutoProgramIdObserver& rc_rhs);
        WsDashboardUpdaterLockedAutoProgramIdObserver& operator=(const WsDashboardUpdaterLockedAutoProgramIdObserver& rc_rhs);
};


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
class WsDashboardUpdaterFlywheelAtSpeedObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelAtSpeedObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelAtSpeedObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        bool        a_currentState;
        
    private:
        WsDashboardUpdaterFlywheelAtSpeedObserver(const WsDashboardUpdaterFlywheelAtSpeedObserver& rc_rhs);
        WsDashboardUpdaterFlywheelAtSpeedObserver& operator=(const WsDashboardUpdaterFlywheelAtSpeedObserver& rc_rhs);
};


//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelNearSpeedObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelNearSpeedObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelNearSpeedObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        bool        a_currentState;
        
    private:
        WsDashboardUpdaterFlywheelNearSpeedObserver(const WsDashboardUpdaterFlywheelNearSpeedObserver& rc_rhs);
        WsDashboardUpdaterFlywheelNearSpeedObserver& operator=(const WsDashboardUpdaterFlywheelNearSpeedObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelRotationalSpeedObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelRotationalSpeedObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelRotationalSpeedObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float       a_currentValue;
        
    private:
        WsDashboardUpdaterFlywheelRotationalSpeedObserver(const WsDashboardUpdaterFlywheelRotationalSpeedObserver& rc_rhs);
        WsDashboardUpdaterFlywheelRotationalSpeedObserver& operator=(const WsDashboardUpdaterFlywheelRotationalSpeedObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelPidSetpointObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelPidSetpointObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelPidSetpointObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float               a_currentValue;
        
    private:
        WsDashboardUpdaterFlywheelPidSetpointObserver(const WsDashboardUpdaterFlywheelPidSetpointObserver& rc_rhs);
        WsDashboardUpdaterFlywheelPidSetpointObserver& operator=(const WsDashboardUpdaterFlywheelPidSetpointObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelPidEnabledObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelPidEnabledObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelPidEnabledObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        bool                a_currentState;
        
    private:
        WsDashboardUpdaterFlywheelPidEnabledObserver(const WsDashboardUpdaterFlywheelPidEnabledObserver& rc_rhs);
        WsDashboardUpdaterFlywheelPidEnabledObserver& operator=(const WsDashboardUpdaterFlywheelPidEnabledObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelPidControllerOutputObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelPidControllerOutputObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelPidControllerOutputObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float               a_currentValue;
        
    private:
        WsDashboardUpdaterFlywheelPidControllerOutputObserver(const WsDashboardUpdaterFlywheelPidControllerOutputObserver& rc_rhs);
        WsDashboardUpdaterFlywheelPidControllerOutputObserver& operator=(const WsDashboardUpdaterFlywheelPidControllerOutputObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelMotorOutputObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelMotorOutputObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelMotorOutputObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float               a_currentValue;
        
    private:
        WsDashboardUpdaterFlywheelMotorOutputObserver(const WsDashboardUpdaterFlywheelMotorOutputObserver& rc_rhs);
        WsDashboardUpdaterFlywheelMotorOutputObserver& operator=(const WsDashboardUpdaterFlywheelMotorOutputObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFloodLightStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFloodLightStateObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFloodLightStateObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        bool        a_currentState;
        
    private:
        WsDashboardUpdaterFloodLightStateObserver(const WsDashboardUpdaterFloodLightStateObserver& rc_rhs);
        WsDashboardUpdaterFloodLightStateObserver& operator=(const WsDashboardUpdaterFloodLightStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFreshImageObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFreshImageObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFreshImageObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        bool        a_currentState;
        
    private:
        WsDashboardUpdaterFreshImageObserver(const WsDashboardUpdaterFreshImageObserver& rc_rhs);
        WsDashboardUpdaterFreshImageObserver& operator=(const WsDashboardUpdaterFreshImageObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterLocationSpeedMappingObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterLocationSpeedMappingObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterLocationSpeedMappingObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*                          ap_subsystem;
        WsTypes::WsLocationSpeedMappingType  a_currentConfig;
        
    private:
        WsDashboardUpdaterLocationSpeedMappingObserver(const WsDashboardUpdaterLocationSpeedMappingObserver& rc_rhs);
        WsDashboardUpdaterLocationSpeedMappingObserver& operator=(const WsDashboardUpdaterLocationSpeedMappingObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelManualPotObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelManualPotObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelManualPotObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float       a_currentValue;
        
    private:
        WsDashboardUpdaterFlywheelManualPotObserver(const WsDashboardUpdaterFlywheelManualPotObserver& rc_rhs);
        WsDashboardUpdaterFlywheelManualPotObserver& operator=(const WsDashboardUpdaterFlywheelManualPotObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelStatePotObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelStatePotObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelStatePotObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater* ap_subsystem;
        float       a_currentValue;
        
    private:
        WsDashboardUpdaterFlywheelStatePotObserver(const WsDashboardUpdaterFlywheelStatePotObserver& rc_rhs);
        WsDashboardUpdaterFlywheelStatePotObserver& operator=(const WsDashboardUpdaterFlywheelStatePotObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelPidConfigObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelPidConfigObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelPidConfigObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*                       ap_subsystem;
        WsTypes::WsPidConfigurationDataT  a_currentConfig;
        
    private:
        WsDashboardUpdaterFlywheelPidConfigObserver(const WsDashboardUpdaterFlywheelPidConfigObserver& rc_rhs);
        WsDashboardUpdaterFlywheelPidConfigObserver& operator=(const WsDashboardUpdaterFlywheelPidConfigObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        bool                 a_currentState;
        
    private:
        WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver(const WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver& rc_rhs);
        WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver& operator=(const WsDashboardUpdaterFlywheelSpeedCompensationSwitchObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        float                a_currentValue;
        
    private:
        WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver(const WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver& rc_rhs);
        WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver& operator=(const WsDashboardUpdaterFlywheelSpeedCompensationPotValueObserver& rc_rhs);
};


//-----------------------------------------------------------------------------
//
//               ### ###
//                #   #     #               #
//                #   #
//                #   #   ###     #####   ###     #####  ## ##
//                 # #      #    #     #    #    #     #  ##  #
//                 # #      #     ###       #    #     #  #   #
//                 # #      #        ##     #    #     #  #   #
//                  #       #    #     #    #    #     #  #   #
//                  #     #####   #####   #####   #####  ### ###
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class WsDashboardUpdaterVisionNumVisibleTargetsObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterVisionNumVisibleTargetsObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterVisionNumVisibleTargetsObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        int                  a_currentValue;
        
    private:
        WsDashboardUpdaterVisionNumVisibleTargetsObserver(const WsDashboardUpdaterVisionNumVisibleTargetsObserver& rc_rhs);
        WsDashboardUpdaterVisionNumVisibleTargetsObserver& operator=(const WsDashboardUpdaterVisionNumVisibleTargetsObserver& rc_rhs);
};

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
class WsDashboardUpdaterDriveBaseLeftEncoderObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseLeftEncoderObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseLeftEncoderObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        INT32                  a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseLeftEncoderObserver(const WsDashboardUpdaterDriveBaseLeftEncoderObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseLeftEncoderObserver& operator=(const WsDashboardUpdaterDriveBaseLeftEncoderObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseRightEncoderObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseRightEncoderObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseRightEncoderObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        INT32                  a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseRightEncoderObserver(const WsDashboardUpdaterDriveBaseRightEncoderObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseRightEncoderObserver& operator=(const WsDashboardUpdaterDriveBaseRightEncoderObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        float                a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver(const WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver& operator=(const WsDashboardUpdaterDriveBaseLeftDriveMotorOutputObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        float                a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver(const WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver& operator=(const WsDashboardUpdaterDriveBaseRightDriveMotorOutputObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseShifterStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseShifterStateObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseShifterStateObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*          ap_subsystem;
        WsTypes::WsShifterStateType  a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseShifterStateObserver(const WsDashboardUpdaterDriveBaseShifterStateObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseShifterStateObserver& operator=(const WsDashboardUpdaterDriveBaseShifterStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        bool                 a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver(const WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver& operator=(const WsDashboardUpdaterDriveBaseAntiTurboEnabledObserver& rc_rhs);
};
//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBasePogoEnabledObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBasePogoEnabledObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBasePogoEnabledObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        WsTypes::WsPogoStickCylinderStateType  a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBasePogoEnabledObserver(const WsDashboardUpdaterDriveBasePogoEnabledObserver& rc_rhs);
        WsDashboardUpdaterDriveBasePogoEnabledObserver& operator=(const WsDashboardUpdaterDriveBasePogoEnabledObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        WsTypes::WsBridgeBrakeCylinderStateType  a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver(const WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver& operator=(const WsDashboardUpdaterDriveBaseBridgeBrakeEnabledObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*                       ap_subsystem;
        WsTypes::WsRobotGrabberCylinderStateType  a_currentInnerValue;
        WsTypes::WsRobotGrabberCylinderStateType  a_currentOuterValue;
        
    private:
        WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver(const WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver& operator=(const WsDashboardUpdaterDriveBaseRobotGrabberEnabledObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseDriverThrottleValueObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseDriverThrottleValueObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseDriverThrottleValueObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        float                a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseDriverThrottleValueObserver(const WsDashboardUpdaterDriveBaseDriverThrottleValueObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseDriverThrottleValueObserver& operator=(const WsDashboardUpdaterDriveBaseDriverThrottleValueObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseDriverHeadingValueObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseDriverHeadingValueObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseDriverHeadingValueObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        float                a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseDriverHeadingValueObserver(const WsDashboardUpdaterDriveBaseDriverHeadingValueObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseDriverHeadingValueObserver& operator=(const WsDashboardUpdaterDriveBaseDriverHeadingValueObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        bool                 a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver(const WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver& operator=(const WsDashboardUpdaterDriveBaseDriverAntiTurboButtonStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        bool                 a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver(const WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver& operator=(const WsDashboardUpdaterDriveBaseDriverShifterButtonStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        bool                 a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver(const WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver& operator=(const WsDashboardUpdaterDriveBaseAccelerationButtonStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterDriveBaseAccelerationEnabledObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterDriveBaseAccelerationEnabledObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterDriveBaseAccelerationEnabledObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        bool                 a_currentValue;
        
    private:
        WsDashboardUpdaterDriveBaseAccelerationEnabledObserver(const WsDashboardUpdaterDriveBaseAccelerationEnabledObserver& rc_rhs);
        WsDashboardUpdaterDriveBaseAccelerationEnabledObserver& operator=(const WsDashboardUpdaterDriveBaseAccelerationEnabledObserver& rc_rhs);
};


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
class WsDashboardUpdaterCompressorStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterCompressorStateObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterCompressorStateObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*              ap_subsystem;
        WsTypes::WsCompressorStateType   a_currentState;
        
    private:
        WsDashboardUpdaterCompressorStateObserver(const WsDashboardUpdaterCompressorStateObserver& rc_rhs);
        WsDashboardUpdaterCompressorStateObserver& operator=(const WsDashboardUpdaterCompressorStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsDashboardUpdaterPneumaticPressureReadingObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterPneumaticPressureReadingObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterPneumaticPressureReadingObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*  ap_subsystem;
        float                a_currentValue;
        
    private:
        WsDashboardUpdaterPneumaticPressureReadingObserver(const WsDashboardUpdaterPneumaticPressureReadingObserver& rc_rhs);
        WsDashboardUpdaterPneumaticPressureReadingObserver& operator=(const WsDashboardUpdaterPneumaticPressureReadingObserver& rc_rhs);
};


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
class WsDashboardUpdaterShooterPresetConfigObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsDashboardUpdaterShooterPresetConfigObserver(WsDashboardUpdater* p_subsystem);
        virtual ~WsDashboardUpdaterShooterPresetConfigObserver(void);
        virtual void update(void);
        
    private:
        WsDashboardUpdater*              ap_subsystem;
        WsTypes::WsShooterPresetConfigT  a_currentConfig;
        
    private:
        WsDashboardUpdaterShooterPresetConfigObserver(const WsDashboardUpdaterShooterPresetConfigObserver& rc_rhs);
        WsDashboardUpdaterShooterPresetConfigObserver& operator=(const WsDashboardUpdaterShooterPresetConfigObserver& rc_rhs);
};


#endif // __WsDashboardUpdaterObservers_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

