#ifndef __WsTurretObservers_hh__
#define __WsTurretObservers_hh__

#include "WsObserver.hh"
#include "WsTypes.hh"

class Timer;
class WsTurret;
//-----------------------------------------------------------------------------
class WsTurretCcwLimitSwitchStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsTurretCcwLimitSwitchStateObserver(WsTurret* p_subsystem);
        virtual ~WsTurretCcwLimitSwitchStateObserver(void);
        virtual void update(void);
        
    private:
        WsTurret* ap_subsystem;
        bool      a_currentState;
        
    private:
        WsTurretCcwLimitSwitchStateObserver(const WsTurretCcwLimitSwitchStateObserver& rc_rhs);
        WsTurretCcwLimitSwitchStateObserver& operator=(const WsTurretCcwLimitSwitchStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsTurretCwLimitSwitchStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsTurretCwLimitSwitchStateObserver(WsTurret* p_subsystem);
        virtual ~WsTurretCwLimitSwitchStateObserver(void);
        virtual void update(void);
        
    private:
        WsTurret* ap_subsystem;
        bool      a_currentState;
        
    private:
        WsTurretCwLimitSwitchStateObserver(const WsTurretCwLimitSwitchStateObserver& rc_rhs);
        WsTurretCwLimitSwitchStateObserver& operator=(const WsTurretCwLimitSwitchStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsTurretHeadingAxisValueObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsTurretHeadingAxisValueObserver(WsTurret* p_subsystem);
        virtual ~WsTurretHeadingAxisValueObserver(void);
        virtual void update(void);
        
    private:
        WsTurret* ap_subsystem;
        float     a_currentValue;
        
    private:
        WsTurretHeadingAxisValueObserver(const WsTurretHeadingAxisValueObserver& rc_rhs);
        WsTurretHeadingAxisValueObserver& operator=(const WsTurretHeadingAxisValueObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsTurretHeadingPotValueObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsTurretHeadingPotValueObserver(WsTurret* p_subsystem);
        virtual ~WsTurretHeadingPotValueObserver(void);
        virtual void update(void);
        
    private:
        WsTurret* ap_subsystem;
        float     a_currentValue;
        
    private:
        WsTurretHeadingPotValueObserver(const WsTurretHeadingPotValueObserver& rc_rhs);
        WsTurretHeadingPotValueObserver& operator=(const WsTurretHeadingPotValueObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsTurretTurboButtonStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsTurretTurboButtonStateObserver(WsTurret* p_subsystem);
        virtual ~WsTurretTurboButtonStateObserver(void);
        virtual void update(void);
        
    private:
        WsTurret* ap_subsystem;
        bool      a_currentButtonState;
        
    private:
        WsTurretTurboButtonStateObserver(const WsTurretTurboButtonStateObserver& rc_rhs);
        WsTurretTurboButtonStateObserver& operator=(const WsTurretTurboButtonStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsTurretPidConfigObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsTurretPidConfigObserver(WsTurret* p_subsystem);
        virtual ~WsTurretPidConfigObserver(void);
        virtual void update(void);
        
    private:
        WsTurret*                         ap_subsystem;
        WsTypes::WsPidConfigurationDataT  a_currentConfig;
        
    private:
        WsTurretPidConfigObserver(const WsTurretPidConfigObserver& rc_rhs);
        WsTurretPidConfigObserver& operator=(const WsTurretPidConfigObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsTurretRequestedPresetObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsTurretRequestedPresetObserver(WsTurret* p_subsystem);
        virtual ~WsTurretRequestedPresetObserver(void);
        virtual void update(void);
        
    private:
        WsTurret*                     ap_subsystem;
        WsTypes::WsShooterPresetType  a_currentValue;
        WsTypes::WsShooterPresetType  a_requestedValue;
        Timer*                        ap_debounceTimer;
        
    private:
        WsTurretRequestedPresetObserver(const WsTurretRequestedPresetObserver& rc_rhs);
        WsTurretRequestedPresetObserver& operator=(const WsTurretRequestedPresetObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsTurretPresetConfigObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsTurretPresetConfigObserver(WsTurret* p_subsystem);
        virtual ~WsTurretPresetConfigObserver(void);
        virtual void update(void);
        
    private:
        WsTurret*                        ap_subsystem;
        WsTypes::WsShooterPresetConfigT  a_currentValue;
        
    private:
        WsTurretPresetConfigObserver(const WsTurretPresetConfigObserver& rc_rhs);
        WsTurretPresetConfigObserver& operator=(const WsTurretPresetConfigObserver& rc_rhs);
};


#endif // __WsTurretObservers_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

