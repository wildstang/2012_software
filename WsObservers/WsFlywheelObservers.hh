#ifndef __WsFlywheelObservers_hh__
#define __WsFlywheelObservers_hh__

#include "WsObserver.hh"
#include "WsTypes.hh"

class Timer;
class WsFlywheel;
//-----------------------------------------------------------------------------
class WsFlywheelFloodLightStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsFlywheelFloodLightStateObserver(WsFlywheel* p_shooterFlywheel);
        virtual ~WsFlywheelFloodLightStateObserver(void);
        virtual void update(void);
        
    private:
        WsFlywheel*  ap_subsystem;
        bool         a_currentState;
        
    private:
        WsFlywheelFloodLightStateObserver(const WsFlywheelFloodLightStateObserver& rc_rhs);
        WsFlywheelFloodLightStateObserver& operator=(const WsFlywheelFloodLightStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsFlywheelFreshImageObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsFlywheelFreshImageObserver(WsFlywheel* p_shooterFlywheel);
        virtual ~WsFlywheelFreshImageObserver(void);
        virtual void update(void);
        
    private:
        WsFlywheel*  ap_subsystem;
        bool         a_currentState;
        
    private:
        WsFlywheelFreshImageObserver(const WsFlywheelFreshImageObserver& rc_rhs);
        WsFlywheelFreshImageObserver& operator=(const WsFlywheelFreshImageObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsFlywheelLocationSpeedMappingObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsFlywheelLocationSpeedMappingObserver(WsFlywheel* p_flywheelSubsystem);
        virtual ~WsFlywheelLocationSpeedMappingObserver(void);
        virtual void update(void);
        
    private:
        WsFlywheel*                          ap_subsystem;
        WsTypes::WsLocationSpeedMappingType  a_currentConfig;
        
    private:
        WsFlywheelLocationSpeedMappingObserver(const WsFlywheelLocationSpeedMappingObserver& rc_rhs);
        WsFlywheelLocationSpeedMappingObserver& operator=(const WsFlywheelLocationSpeedMappingObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsFlywheelManualPotObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsFlywheelManualPotObserver(WsFlywheel* p_shooterFlywheel);
        virtual ~WsFlywheelManualPotObserver(void);
        virtual void update(void);
        
    private:
        WsFlywheel*  ap_subsystem;
        float        a_currentFlywheelManualPotValue;
        
    private:
        WsFlywheelManualPotObserver(const WsFlywheelManualPotObserver& rc_rhs);
        WsFlywheelManualPotObserver& operator=(const WsFlywheelManualPotObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsFlywheelStatePotValueObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsFlywheelStatePotValueObserver(WsFlywheel* p_shooterFlywheel);
        virtual ~WsFlywheelStatePotValueObserver(void);
        virtual void update(void);
        
    private:
        WsFlywheel*  ap_subsystem;
        float        a_currentValue;
        
    private:
        WsFlywheelStatePotValueObserver(const WsFlywheelStatePotValueObserver& rc_rhs);
        WsFlywheelStatePotValueObserver& operator=(const WsFlywheelStatePotValueObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsFlywheelPidConfigObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsFlywheelPidConfigObserver(WsFlywheel* p_flywheelSubsystem);
        virtual ~WsFlywheelPidConfigObserver(void);
        virtual void update(void);
        
    private:
        WsFlywheel*                       ap_subsystem;
        WsTypes::WsPidConfigurationDataT  a_currentConfig;
        
    private:
        WsFlywheelPidConfigObserver(const WsFlywheelPidConfigObserver& rc_rhs);
        WsFlywheelPidConfigObserver& operator=(const WsFlywheelPidConfigObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsFlywheelRequestedPresetObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsFlywheelRequestedPresetObserver(WsFlywheel* p_subsystem);
        virtual ~WsFlywheelRequestedPresetObserver(void);
        virtual void update(void);
        
    private:
        WsFlywheel*                   ap_subsystem;
        WsTypes::WsShooterPresetType  a_currentValue;
        WsTypes::WsShooterPresetType  a_requestedValue;
        Timer*                        ap_debounceTimer;
        
    private:
        WsFlywheelRequestedPresetObserver(const WsFlywheelRequestedPresetObserver& rc_rhs);
        WsFlywheelRequestedPresetObserver& operator=(const WsFlywheelRequestedPresetObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsFlywheelPresetConfigObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsFlywheelPresetConfigObserver(WsFlywheel* p_subsystem);
        virtual ~WsFlywheelPresetConfigObserver(void);
        virtual void update(void);
        
    private:
        WsFlywheel*                      ap_subsystem;
        WsTypes::WsShooterPresetConfigT  a_currentValue;
        
    private:
        WsFlywheelPresetConfigObserver(const WsFlywheelPresetConfigObserver& rc_rhs);
        WsFlywheelPresetConfigObserver& operator=(const WsFlywheelPresetConfigObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsFlywheelSpeedCompensationFactorObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsFlywheelSpeedCompensationFactorObserver(WsFlywheel* p_subsystem);
        virtual ~WsFlywheelSpeedCompensationFactorObserver(void);
        virtual void update(void);
        
    private:
        WsFlywheel*  ap_subsystem;
        bool         a_currentSwitchState;
        float        a_currentPotValue;
        
    private:
        WsFlywheelSpeedCompensationFactorObserver(const WsFlywheelSpeedCompensationFactorObserver& rc_rhs);
        WsFlywheelSpeedCompensationFactorObserver& operator=(const WsFlywheelSpeedCompensationFactorObserver& rc_rhs);
};

#endif // __WsFlywheelObservers_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

