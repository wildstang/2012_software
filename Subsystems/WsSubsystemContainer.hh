#ifndef _WsSubsystemContainer_hh_
#define _WsSubsystemContainer_hh_

#include "WsSubsystem.hh"

//
// These definitions are provided for convenience and are 100% tied to the
//  order of the WsSubsystemId enumeration.  If that enum changes, these
//  defines might have to change.
//
#define WS_FIRST_SUBSYSTEM_ID WsSubsystemContainer::WS_SS_APPENDAGE_HANDLER_ID
#define WS_LAST_SUBSYSTEM_ID  WsSubsystemContainer::WS_SS_NUM_SUBSYSTEMS - 1

class WsSubsystemContainer
{
    public:
        typedef enum
        {
            WS_SS_APPENDAGE_HANDLER_ID = 0,
            WS_SS_BALL_TRANSPORT_ID,
            WS_SS_COMPRESSOR_ID,
            WS_SS_DRIVEBASE_ID,
            WS_SS_HOOD_ID,
            WS_SS_SHOOTER_FLYWHEEL_ID,
            WS_SS_DASHBOARD_UPDATER_ID,
            WS_SS_TURRET_ID,
            WS_SS_POGO_STICK_ID,
            WS_SS_BRIDGE_BRAKE_ID,
            
            // Insert New Subsystem IDs above this line
            WS_SS_NUM_SUBSYSTEMS
        } WsSubsystemId;
        
    public:
        virtual ~WsSubsystemContainer(void);
        
        static WsSubsystemContainer* instance(void);
        WsSubsystem* getSubsystemById(const WsSubsystemId& rc_id);
        void update(void);
        
    protected:
        WsSubsystemContainer(void);
        
    private:
        static WsSubsystemContainer* asp_instance;
        WsSubsystem* ap_subsystemTable[WS_SS_NUM_SUBSYSTEMS];
        
        WsSubsystemContainer(const WsSubsystemContainer& rc_rhs);
        const WsSubsystemContainer& operator=(const WsSubsystemContainer& rc_rhs);
};

#endif //_WsSubsystemContainer_hh_

