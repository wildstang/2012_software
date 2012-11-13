#ifndef _WsAutoShootProgram_hh__
#define _WsAutoShootProgram_hh__

#include "WsAutonomousProgram.hh"
#include "WsTypes.hh"

class Timer;

class WsAutoShootProgram : public WsAutonomousProgram
{
    public:
        WsAutoShootProgram(void);
        virtual ~WsAutoShootProgram(void);
        
        virtual void init(void);
        virtual bool run(void);
        virtual void clear(void);
        virtual bool isDone(void);
        
        typedef enum
        {
            WS_AUTO_SHOOT_STATE_NONE = 0,
            WS_AUTO_SHOOT_STATE_PRIME_FLY_1,
            WS_AUTO_SHOOT_STATE_SPIN_FLY_1,
            WS_AUTO_SHOOT_STATE_SHOOT_1,
            WS_AUTO_SHOOT_STATE_PRIME_FLY_2,
            WS_AUTO_SHOOT_STATE_SPIN_FLY_2,
            WS_AUTO_SHOOT_STATE_SHOOT_2,
            WS_AUTO_SHOOT_STATE_DONE,
            WS_AUTO_SHOOT_STATE_NUM
        } WsAutoShootProgramStateType;
        
    private:
        WsAutoShootProgram(const WsAutoShootProgram& rc_rhs);
        const WsAutoShootProgram& operator=(const WsAutoShootProgram& rc_rhs);
        
        bool a_done;
        WsAutoShootProgramStateType a_currentState;
        
        Timer* ap_stateTimer;
        WsTypes::WsAutoShootProgramConfigType a_configVals;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

