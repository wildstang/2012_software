#ifndef _WsAutoShoot3Program_hh__
#define _WsAutoShoot3Program_hh__

#include "WsAutonomousProgram.hh"
#include "WsTypes.hh"

class Timer;

class WsAutoShoot3Program : public WsAutonomousProgram
{
    public:
        WsAutoShoot3Program(void);
        virtual ~WsAutoShoot3Program(void);
        
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
            WS_AUTO_SHOOT_STATE_PRIME_FLY_3,
            WS_AUTO_SHOOT_STATE_SPIN_FLY_3,
            WS_AUTO_SHOOT_STATE_SHOOT_3,
            WS_AUTO_SHOOT_STATE_DONE,
            WS_AUTO_SHOOT_STATE_NUM
        } WsAutoShoot3ProgramStateType;
        
    private:
        WsAutoShoot3Program(const WsAutoShoot3Program& rc_rhs);
        const WsAutoShoot3Program& operator=(const WsAutoShoot3Program& rc_rhs);
        
        bool a_done;
        WsAutoShoot3ProgramStateType a_currentState;
        
        Timer* ap_stateTimer;
        WsTypes::WsAutoShootProgramConfigType a_configVals;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

