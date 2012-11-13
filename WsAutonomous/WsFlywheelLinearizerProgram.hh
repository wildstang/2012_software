#ifndef __WsFlywheelLinearizerProgram_hh__
#define __WsFlywheelLinearizerProgram_hh__

#include "WsAutonomousProgram.hh"

class Timer;

class WsFlywheelLinearizerProgram : public WsAutonomousProgram
{
    public:
        WsFlywheelLinearizerProgram(void);
        virtual ~WsFlywheelLinearizerProgram(void);
        
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
        WsFlywheelLinearizerProgram(const WsFlywheelLinearizerProgram& rc_rhs);
        const WsFlywheelLinearizerProgram& operator=(const WsFlywheelLinearizerProgram& rc_rhs);
        
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

