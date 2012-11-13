#ifndef __WsPogoStickButtonStateObserver_hh__
#define __WsPogoStickButtonStateObserver_hh__

#include "WsObserver.hh"

class WsPogoStick;
//-----------------------------------------------------------------------------
class WsPogoStickButtonStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsPogoStickButtonStateObserver(WsPogoStick* p_pogoStickSubsystem);
        virtual ~WsPogoStickButtonStateObserver(void);
        virtual void update(void);
        
    private:
        WsPogoStick* ap_pogoStickSubsystem;
        bool   a_currentPogoStickButtonState;
        
    private:
        WsPogoStickButtonStateObserver(const WsPogoStickButtonStateObserver& rc_rhs);
        WsPogoStickButtonStateObserver& operator=(const WsPogoStickButtonStateObserver& rc_rhs);
};

#endif // __WsPogoStickButtonStateObserver_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

