#ifndef __WsTurretPidInput_hh__
#define __WsTurretPidInput_hh__

#include "WsPidInput.hh"

class WsTurretPidInput : public WsPidInput
{
    public:
        WsTurretPidInput(void);
        virtual ~WsTurretPidInput(void);
        
        virtual float pidRead(void);
        
    private:
        WsTurretPidInput(const WsTurretPidInput& rc_rhs);
        const WsTurretPidInput& operator=(const WsTurretPidInput& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

