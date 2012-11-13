#ifndef __WsTurretPidOutput_hh__
#define __WsTurretPidOutput_hh__

#include "Base.h"
#include "WsPidOutput.hh"

class WsTurretPidOutput : public WsPidOutput
{
    public:
        WsTurretPidOutput(void);
        virtual ~WsTurretPidOutput(void);
        
        virtual void pidWrite(float output);
        
    private:
        WsTurretPidOutput(const WsTurretPidOutput& rc_rhs);
        const WsTurretPidOutput& operator=(const WsTurretPidOutput& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

