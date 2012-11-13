#ifndef __WsBtMotorValsObserver_hh__
#define __WsBtMotorValsObserver_hh__

#include "WsObserver.hh"
#include "WsTypes.hh"

class WsBallTransport;
class WsBtMotorValsObserver : public WsObserver
{
    public:
        WsBtMotorValsObserver(WsBallTransport* p_ballTransport);
        virtual ~WsBtMotorValsObserver(void);
        virtual void update(void);
        
    private:
        WsBallTransport*                  ap_ballTransport;
        WsTypes::WsBallTransportMotorValueType  a_currentVals;
        
    private:
        WsBtMotorValsObserver(const WsBtMotorValsObserver& rc_rhs);
        WsBtMotorValsObserver& operator=(const WsBtMotorValsObserver& rc_rhs);
};

#endif // __WsBtMotorValsObserver

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

