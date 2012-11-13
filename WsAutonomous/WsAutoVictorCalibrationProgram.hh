#ifndef _WsAutoVictorCalibrationProgram_hh__
#define _WsAutoVictorCalibrationProgram_hh__

#include "WsAutonomousProgram.hh"

class Timer;
class WsAutoVictorCalibrationProgram : public WsAutonomousProgram
{
    public:
        WsAutoVictorCalibrationProgram(void);
        virtual ~WsAutoVictorCalibrationProgram(void);
        
        virtual void init(void);
        virtual bool run(void);
        virtual bool isDone(void);
        virtual void clear(void);
        
    private:
        WsAutoVictorCalibrationProgram(const WsAutoVictorCalibrationProgram& rc_rhs);
        const WsAutoVictorCalibrationProgram& operator=(const WsAutoVictorCalibrationProgram& rc_rhs);
        
        Timer* ap_stateTimer;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

