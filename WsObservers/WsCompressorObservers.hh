#ifndef __WsPressureSwitchStateObserver_hh__
#define __WsPressureSwitchStateObserver_hh__

#include "WsObserver.hh"

class WsCompressor;
//-----------------------------------------------------------------------------
class WsPressureSwitchStateObserver : public WsObserver
    //-----------------------------------------------------------------------------
{
    public:
        WsPressureSwitchStateObserver(WsCompressor* p_compressorSubsystem);
        virtual ~WsPressureSwitchStateObserver(void);
        virtual void update(void);
        
    private:
        WsCompressor* ap_compressorSubsystem;
        bool   a_currentPressureSwitchState;
        
    private:
        WsPressureSwitchStateObserver(const WsPressureSwitchStateObserver& rc_rhs);
        WsPressureSwitchStateObserver& operator=(const WsPressureSwitchStateObserver& rc_rhs);
};

#endif // __WsPressureSwitchStateObserver_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

