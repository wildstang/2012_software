#ifndef __WsBridgeBrakeButtonStateObserver_hh__
#define __WsBridgeBrakeButtonStateObserver_hh__

#include "WsObserver.hh"

class WsBridgeBrake;
//-----------------------------------------------------------------------------
class WsBridgeBrakeButtonStateObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsBridgeBrakeButtonStateObserver(WsBridgeBrake* p_bridgeBrakeSubsystem);
        virtual ~WsBridgeBrakeButtonStateObserver(void);
        virtual void update(void);
        
    private:
        WsBridgeBrake* ap_bridgeBrakeSubsystem;
        bool   a_currentBridgeBrakeButtonState;
        
    private:
        WsBridgeBrakeButtonStateObserver(const WsBridgeBrakeButtonStateObserver& rc_rhs);
        WsBridgeBrakeButtonStateObserver& operator=(const WsBridgeBrakeButtonStateObserver& rc_rhs);
};

#endif // __WsBridgeBrakeButtonStateObserver_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

