#include "WsSubsystemContainer.hh"
#include "WsDriveBase.hh"
#include "WsBallTransport.hh"
#include "WsDashboardUpdater.hh"
#include "WsTurret.hh"
#include "WsCompressor.hh"
#include "WsAppendageHandler.hh"
#include "WsFlywheel.hh"
#include "WsHood.hh"
#include "WsSubsystem.hh"
#include "WsLogger.hh"
#include "WsPogoStick.hh"
#include "WsBridgeBrake.hh"

WsSubsystemContainer* WsSubsystemContainer::asp_instance = NULL;
WsSubsystemContainer::WsSubsystemContainer(void)
{
    // Create all the subsystems the robot will use
    WsSubsystem* p_ss = NULL;
    
    // Initialize all to NULL
    for (int i = WS_FIRST_SUBSYSTEM_ID; i <= WS_LAST_SUBSYSTEM_ID; i++)
    {
        ap_subsystemTable[i] = NULL;
    }
    
    p_ss = new WsAppendageHandler();
    ap_subsystemTable[WS_SS_APPENDAGE_HANDLER_ID] = p_ss;
    WS_LOG_DEBUG("%s Subsystem Created", p_ss->GetName().c_str());
    
    p_ss = new WsBallTransport();
    ap_subsystemTable[WS_SS_BALL_TRANSPORT_ID] = p_ss;
    WS_LOG_DEBUG("%s Subsystem Created", p_ss->GetName().c_str());
    
    p_ss = new WsCompressor();
    ap_subsystemTable[WS_SS_COMPRESSOR_ID] = p_ss;
    WS_LOG_DEBUG("%s Subsystem Created", p_ss->GetName().c_str());
    
    p_ss = new WsDriveBase();
    ap_subsystemTable[WS_SS_DRIVEBASE_ID] = p_ss;
    WS_LOG_DEBUG("%s Subsystem Created", p_ss->GetName().c_str());
    
    p_ss = new WsHood();
    ap_subsystemTable[WS_SS_HOOD_ID] = p_ss;
    WS_LOG_DEBUG("%s Subsystem Created", p_ss->GetName().c_str());
    
    p_ss = new WsFlywheel();
    ap_subsystemTable[WS_SS_SHOOTER_FLYWHEEL_ID] = p_ss;
    WS_LOG_DEBUG("%s Subsystem Created", p_ss->GetName().c_str());
    
    p_ss = new WsTurret();
    ap_subsystemTable[WS_SS_TURRET_ID] = p_ss;
    WS_LOG_DEBUG("%s Subsystem Created", p_ss->GetName().c_str());
    
    p_ss = new WsDashboardUpdater();
    ap_subsystemTable[WS_SS_DASHBOARD_UPDATER_ID] = p_ss;
    WS_LOG_DEBUG("%s Subsystem Created", p_ss->GetName().c_str());
    
    p_ss = new WsPogoStick();
    ap_subsystemTable[WS_SS_POGO_STICK_ID] = p_ss;
    WS_LOG_DEBUG("%s Subsystem Created", p_ss->GetName().c_str());
    
    p_ss = new WsBridgeBrake();
    ap_subsystemTable[WS_SS_BRIDGE_BRAKE_ID] = p_ss;
    WS_LOG_DEBUG("%s Subsystem Created", p_ss->GetName().c_str());
    
    // Check to make sure we initialized all sub-systems
    for (int i = WS_FIRST_SUBSYSTEM_ID; i <= WS_LAST_SUBSYSTEM_ID; i++)
    {
        if (NULL == ap_subsystemTable[i])
        {
            WS_LOG_ERROR("Subsystem ID %d is NULL on creation", i);
        }
    }
}

WsSubsystemContainer::~WsSubsystemContainer(void)
{
    // Free up memory associated with all created subsystems
    for (int i = WS_FIRST_SUBSYSTEM_ID; i <= WS_LAST_SUBSYSTEM_ID; i++)
    {
        WsSubsystem* p_ss = ap_subsystemTable[i];
        WS_LOG_DEBUG("%s Subsystem Destroyed", p_ss->GetName().c_str());
        delete p_ss;
        ap_subsystemTable[i] = NULL;
    }
    
    // Don't forget to reset asp_instance
    asp_instance = NULL;
}

void
WsSubsystemContainer::update(void)
{
    // Call update on all subsystems
    for (int i = WS_FIRST_SUBSYSTEM_ID; i <= WS_LAST_SUBSYSTEM_ID; i++)
    {
        ap_subsystemTable[i]->update();
    }
}

WsSubsystemContainer*
WsSubsystemContainer::instance(void)
{
    if (NULL == asp_instance)
    { asp_instance = new WsSubsystemContainer(); }
    return asp_instance;
}

WsSubsystem*
WsSubsystemContainer::getSubsystemById(const WsSubsystemId& rc_id)
{
    WsSubsystem* p_ss = NULL;
    
    // Bounds-check the incoming ID
    if ((rc_id >= WS_FIRST_SUBSYSTEM_ID) &&
        (rc_id <= WS_LAST_SUBSYSTEM_ID))
    {
        p_ss = ap_subsystemTable[rc_id];
    }
    else
    {
        p_ss = NULL;
        WS_LOG_ERROR("Subsystem ID Out of range. (%d < %d > %d)",
                     WS_FIRST_SUBSYSTEM_ID,
                     rc_id,
                     WS_LAST_SUBSYSTEM_ID);
    }
    
    return p_ss;
}

