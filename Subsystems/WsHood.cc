#include "WsHood.hh"
#include "WsOutputFacade.hh"
#include "WsHoodObservers.hh"
#include "WsLogger.hh"

WsHood::WsHood(void)
    : WsSubsystem("WsHood")
    , a_hoodState(WsTypes::WS_HOOD_CYLINDER_UP)
    , ap_hoodAxisValueObserver(NULL)
    , ap_presetObserver(NULL)
{
    WS_LOG_TRACE("Begin");
    
    ap_hoodAxisValueObserver = new WsHoodButtonStateObserver(this);
    ap_presetObserver = new WsHoodShooterPresetObserver(this);
    WsOutputFacade::instance()->setHoodState(a_hoodState);
    
    WS_LOG_TRACE("End");
}

WsHood::~WsHood(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_hoodAxisValueObserver;    ap_hoodAxisValueObserver = NULL;
    delete ap_presetObserver;           ap_presetObserver = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsHood::update(void)
{
    WS_LOG_TRACE("Begin");
    
    WsOutputFacade::instance()->setHoodState(a_hoodState);
    
    WS_LOG_TRACE("End");
}

void
WsHood::acceptNewHoodButtonState(bool oldValue, bool newValue)
{
    if ((false == oldValue) &&
        (true == newValue))
    {
        a_hoodState = (WsTypes::WS_HOOD_CYLINDER_UP == a_hoodState)
                      ? WsTypes::WS_HOOD_CYLINDER_DOWN
                      : WsTypes::WS_HOOD_CYLINDER_UP;
    }
    // else do nothing
}

void
WsHood::acceptNewRequestedPreset(const WsTypes::WsShooterPresetType& rc_newPreset)
{
    // Only react to "valid" request changes
    if (WsTypes::WS_SHOOTER_PRESET_NONE != rc_newPreset)
    {
        switch (rc_newPreset)
        {
            case WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT:
            case WsTypes::WS_SHOOTER_PRESET_FENDER_RIGHT:
            {
                a_hoodState = WsTypes::WS_HOOD_CYLINDER_DOWN;
            } break;
            
            case WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD:
            case WsTypes::WS_SHOOTER_PRESET_KEY_BACK:
            case WsTypes::WS_SHOOTER_PRESET_AUTON:
            case WsTypes::WS_SHOOTER_PRESET_NONE:
            default:
            {
                a_hoodState = WsTypes::WS_HOOD_CYLINDER_UP;
            } break;
        } // end switch
    }
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

