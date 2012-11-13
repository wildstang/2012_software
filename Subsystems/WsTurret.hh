#ifndef __WsTurret_hh__
#define __WsTurret_hh__

#include "WsSubsystem.hh"
#include "WsTypes.hh"

class WsObserver;
class WsPidController;
class WsTurretPidInput;
class WsTurretPidOutput;
class WsTurret : public WsSubsystem
{
    public:
    
        typedef enum
        {
            WS_TURRET_MANUAL_CONTROL,
            WS_TURRET_PRESET_CONTROL,
            WS_TURRET_OFF
        } WsTurretControlType;
        
        typedef enum
        {
            WS_TRACKING_STATE_NO_TARGET,
            WS_TRACKING_STATE_COARSE,
            WS_TRACKING_STATE_FINE,
            WS_TRACKING_STATE_ON_TARGET,
            WS_TRACKING_STATE_NUM
        } WsAutoTrackingStateType;
        
    public:
        WsTurret(void);
        ~WsTurret(void);
        
        void update(void);
        
        void acceptNewHeadingAxisValue(const float& rc_newValue);
        void acceptNewHeadingPotValue(const float& rc_newValue);
        void acceptNewTurboButtonState(const bool& rc_newButtonState);
        void acceptNewCwLimitSwitchState(const bool& rc_newSwitchState);
        void acceptNewCcwLimitSwitchState(const bool& rc_newSwitchState);
        void acceptNewPidConfig(const WsTypes::WsPidConfigurationDataT& rc_newData);
        void acceptNewRequestedPreset(const WsTypes::WsShooterPresetType& rc_newValue);
        void acceptNewPresetConfig(const WsTypes::WsShooterPresetConfigT& rc_newConfig);
        
        void updateAutoTracking(void);
        
    private:
        float                           a_headingAxisValue;
        float                           a_headingPotValue;
        WsTurretControlType             a_turretControlState;
        bool                            a_cwLimitSwitchState;
        bool                            a_ccwLimitSwitchState;
        bool                            a_turboFlag;
        WsTypes::WsShooterPresetType    a_currentRequestedPreset;
        WsPidController*                ap_turretPidController;
        WsTurretPidInput*               ap_turretPidInput;
        WsTurretPidOutput*              ap_turretPidOutput;
        WsTypes::WsShooterPresetConfigT a_presetConfig;
        
        WsObserver*   ap_headingAxisValueObserver;
        WsObserver*   ap_headingPotValueObserver;
        WsObserver*   ap_turboButtonStateObserver;
        WsObserver*   ap_cwLimitSwitchStateObserver;
        WsObserver*   ap_ccwLimitSwitchStateObserver;
        WsObserver*   ap_turretPidConfigObserver;
        WsObserver*   ap_requestedPresetObserver;
        WsObserver*   ap_presetConfigObserver;
        
        WsAutoTrackingStateType a_autoTrackingState;
        
    private:
        WsTurret(const WsTurret& rc_rhs);
        const WsTurret& operator=(const WsTurret& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

