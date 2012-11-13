#ifndef __WsHood_hh__
#define __WsHood_hh__

#include "WsSubsystem.hh"
#include "WsTypes.hh"

class WsObserver;
class WsHood : public WsSubsystem
{
    public:
        WsHood(void);
        ~WsHood(void);
        
        void update(void);
        void acceptNewHoodButtonState(bool oldValue, bool newValue);
        void acceptNewRequestedPreset(const WsTypes::WsShooterPresetType& rc_newPreset);
        
    private:
        WsTypes::WsHoodCylinderStateType a_hoodState;
        WsObserver*                      ap_hoodAxisValueObserver;
        WsObserver*                      ap_presetObserver;
        
    private:
        WsHood(const WsHood& rc_rhs);
        const WsHood& operator=(const WsHood& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

