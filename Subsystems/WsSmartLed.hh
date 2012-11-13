#ifndef __WsSmartLed_hh__
#define __WsSmartLed_hh__

#include "WsSubsystem.hh"

class I2C;
class Notifier;
class WsSmartLed : public WsSubsystem
{
    public:
        WsSmartLed(void);
        virtual ~WsSmartLed(void);
        static WsSmartLed* instance(void);
        
        static void callUpdate(void* p_this);
        void update(void);
        
    private:
        I2C*               ap_i2cInterface;
        Notifier*          ap_updateLoopNotifier;
        static WsSmartLed* asp_instance;
        
    private:
        void sendPayloadData(UINT8* p_data, UINT8 size);
        
    private:
        WsSmartLed(const WsSmartLed& rc_rhs);
        const WsSmartLed& operator=(const WsSmartLed& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

