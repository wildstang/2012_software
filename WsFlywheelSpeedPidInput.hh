#ifndef __WsFlywheelSpeedPidInput_hh__
#define __WsFlywheelSpeedPidInput_hh__

#include "WsPidInput.hh"

class WsFlywheelSpeedPidInput : public WsPidInput
{
    public:
        WsFlywheelSpeedPidInput(void);
        virtual ~WsFlywheelSpeedPidInput(void);
        
        virtual float pidRead(void);
        
    private:
        WsFlywheelSpeedPidInput(const WsFlywheelSpeedPidInput& rc_rhs);
        const WsFlywheelSpeedPidInput& operator=(const WsFlywheelSpeedPidInput& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

