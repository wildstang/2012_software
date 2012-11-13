#ifndef __WsBridgeBrake_hh__
#define __WsBridgeBrake_hh__

#include "WsSubsystem.hh"
#include "WsTypes.hh"

class WsObserver;
class WsBridgeBrake : public WsSubsystem
{
    public:
        WsBridgeBrake(void);
        ~WsBridgeBrake(void);
        
        void update(void);
        void acceptNewBridgeBrakeButtonState(bool oldValue, bool newValue);
        
    private:
        WsTypes::WsBridgeBrakeCylinderStateType a_bridgeBrakeState;
        WsObserver*                           ap_bridgeBrakeValueObserver;
        
    private:
        WsBridgeBrake(const WsBridgeBrake& rc_rhs);
        const WsBridgeBrake& operator=(const WsBridgeBrake& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

