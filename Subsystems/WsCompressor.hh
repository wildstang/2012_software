#ifndef __WsCompressor_hh__
#define __WsCompressor_hh__

#include "WsSubsystem.hh"

class WsObserver;

class WsCompressor : public WsSubsystem
{
    public:
        WsCompressor(void);
        ~WsCompressor(void);
        
        void setCompressorState(const bool& compressor_state);
        void getCompressorState(bool& compressor_state);
        
        void acceptNewPressureSwitchState(bool sensorState);
        
        void update(void);
        
    private:
        WsCompressor(const WsCompressor& rc_rhs);
        const WsCompressor& operator=(const WsCompressor& rc_rhs);
        
        bool a_compressorState;
        
        WsObserver* ap_pressureSwitchObserver;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

