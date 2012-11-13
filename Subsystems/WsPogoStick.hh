#ifndef __WsPogoStick_hh__
#define __WsPogoStick_hh__

#include "WsSubsystem.hh"
#include "WsTypes.hh"

class WsObserver;
class WsPogoStick : public WsSubsystem
{
    public:
        WsPogoStick(void);
        ~WsPogoStick(void);
        
        void update(void);
        void acceptNewPogoStickButtonState(bool oldValue, bool newValue);
        
    private:
        WsTypes::WsPogoStickCylinderStateType a_pogoStickState;
        WsObserver*                           ap_pogoStickValueObserver;
        
    private:
        WsPogoStick(const WsPogoStick& rc_rhs);
        const WsPogoStick& operator=(const WsPogoStick& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

