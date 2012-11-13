#ifndef __WsVictorLinearizerProgram_hh__
#define __WsVictorLinearizerProgram_hh__

#include "WsAutonomousProgram.hh"

class Timer;

class WsVictorLinearizerProgram : public WsAutonomousProgram
{
    public:
        WsVictorLinearizerProgram(void);
        virtual ~WsVictorLinearizerProgram(void);
        
        typedef enum
        {
            VICTOR_STATE_SETUP = 0,
            VICTOR_STATE_STABILIZE,
            VICTOR_STATE_TEST,
            VICTOR_STATE_NUM
        } VictorLinearizerStateT;
        
        virtual void init(void);
        virtual bool run(void);
        virtual bool isDone(void);
        virtual void clear(void);
        
    private:
        WsVictorLinearizerProgram(const WsVictorLinearizerProgram& rc_rhs);
        const WsVictorLinearizerProgram& operator=(const WsVictorLinearizerProgram& rc_rhs);
        
        int a_testIteration;
        VictorLinearizerStateT a_testingState;
        Timer* ap_victorTimer;
        float a_output;
        bool a_finished;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

