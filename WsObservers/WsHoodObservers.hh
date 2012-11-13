#ifndef __WsHoodObservers_hh__
#define __WsHoodObservers_hh__

#include "WsObserver.hh"
#include "WsTypes.hh"

class WsHood;
//-----------------------------------------------------------------------------
class WsHoodButtonStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsHoodButtonStateObserver(WsHood* p_subsystem);
        virtual ~WsHoodButtonStateObserver(void);
        virtual void update(void);
        
    private:
        WsHood* ap_subsystem;
        bool   a_currentState;
        
    private:
        WsHoodButtonStateObserver(const WsHoodButtonStateObserver& rc_rhs);
        WsHoodButtonStateObserver& operator=(const WsHoodButtonStateObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsHoodShooterPresetObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsHoodShooterPresetObserver(WsHood* p_subsystem);
        virtual ~WsHoodShooterPresetObserver(void);
        virtual void update(void);
        
    private:
        WsHood*               ap_subsystem;
        WsTypes::WsShooterPresetType   a_currentState;
        
    private:
        WsHoodShooterPresetObserver(const WsHoodShooterPresetObserver& rc_rhs);
        WsHoodShooterPresetObserver& operator=(const WsHoodShooterPresetObserver& rc_rhs);
};

#endif // __WsHoodShooterPresetObserver

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

