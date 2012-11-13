#ifndef __WsFlywheel_hh__
#define __WsFlywheel_hh__

#include "WsSubsystem.hh"
#include "WsTypes.hh"

class WsPidController;
class WsFlywheelSpeedPidInput;
class WsFlywheelSpeedPidOutput;
class WsObserver;
class Timer;

class WsFlywheel : public WsSubsystem
{
    public:
        WsFlywheel(void);
        virtual ~WsFlywheel(void);
        
        void update(void);
        
        void acceptNewFloodlightState(const bool& rc_state);
        void acceptNewFreshImageFlag(const bool& rc_flag);
        void acceptNewFlywheelStatePotValue(const float& rc_newValue);
        void acceptNewFlywheelManualPotValue(const float& rc_newValue);
        void acceptNewPidConfig(const WsTypes::WsPidConfigurationDataT& rc_newData);
        void acceptNewLocationSpeedMapping(const WsTypes::WsLocationSpeedMappingType& rc_newMapping);
        void acceptNewRequestedPreset(const WsTypes::WsShooterPresetType& rc_newValue);
        void acceptNewPresetConfig(const WsTypes::WsShooterPresetConfigT& rc_newConfig);
        void acceptNewCompensationFactor(const float& r_newValue);
        
        // Pseudo-Pid functionality
        void calculateCoarseSpeed(const float& rc_setPoint);
        
    private:
        bool                                 a_floodLightState;
        WsPidController*                     ap_flywheelSpeedPidController;
        WsFlywheelSpeedPidInput*             ap_flywheelSpeedPidInput;
        WsFlywheelSpeedPidOutput*            ap_flywheelSpeedPidOutput;
        WsTypes::WsFlywheelControlType       a_flywheelControlType;
        float                                a_flywheelManualPotValue;
        float                                a_lastMappedMotorOutput;
        WsTypes::WsLocationSpeedMappingType  a_flywheelLocationSpeedMapping;
        bool                                 a_newImageReady;     // set to true when image data is ready to be processed
        bool                                 a_lastImageHadValidDistance;     // set to true when image data is ready to be processed
        float                                a_lastKnownDistanceToTarget;
        WsTypes::WsShooterPresetType         a_currentRequestedPreset;
        WsTypes::WsShooterPresetConfigT      a_presetConfig;
        float*                               ap_currentSetpoint;
        WsTypes::WsShooterPresetType         a_rawRequestedPreset;
        float                                a_defaultFlywheelSetpoint;  // Don't change this
        
        WsObserver*  ap_stateSwitchObserver;
        WsObserver*  ap_manualPotObserver;
        WsObserver*  ap_floodLightStateObserver;
        WsObserver*  ap_freshCameraImageObserver;
        WsObserver*  ap_flywheelPidConfigObserver;
        WsObserver*  ap_flywheelLocationSpeedMappingObserver;
        WsObserver*  ap_requestedPresetObserver;
        WsObserver*  ap_presetConfigObserver;
        WsObserver*  ap_presetCompensationObserver;
        Timer*       ap_flywheelCoarseSetupTimer;
        
    private:
        //        bool getDistanceToBottomTarget(float& r_distance);
        bool calculateDistanceFromImage(float& r_distance);
        float interpolateFlywheelSpeed(const float& rc_distanceToBottom);
        float interpolateFlywheelSpeedControllerSpeed(const float& rc_distanceToBottom);
        void processPresets(const WsTypes::WsShooterPresetType& rc_newValue);
        
    private:
        WsFlywheel(const WsFlywheel& rc_rhs);
        const WsFlywheel& operator=(const WsFlywheel& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

