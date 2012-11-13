#ifndef __WsFlywheelSpeedPidOutput_hh__
#define __WsFlywheelSpeedPidOutput_hh__

#include "WsPidOutput.hh"

class WsFlywheelSpeedPidOutput : public WsPidOutput
{
    public:
        WsFlywheelSpeedPidOutput(void);
        virtual ~WsFlywheelSpeedPidOutput(void);
        
        virtual void pidWrite(float output);
        
        void overwriteCumulativeOutput(const float& rc_new_val);
        
    private:
        float a_cumulativeOutput;
        
        WsFlywheelSpeedPidOutput(const WsFlywheelSpeedPidOutput& rc_rhs);
        const WsFlywheelSpeedPidOutput& operator=(const WsFlywheelSpeedPidOutput& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

