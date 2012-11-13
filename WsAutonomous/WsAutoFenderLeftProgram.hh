#ifndef __WsAutoFenderLeftProgram_hh__
#define __WsAutoFenderLeftProgram_hh__

#include "WsAutonomousProgram.hh"
#include "WsTypes.hh"

class Timer;

class WsAutoFenderLeftProgram : public WsAutonomousProgram
{
    public:
        WsAutoFenderLeftProgram(void);
        virtual ~WsAutoFenderLeftProgram(void);
        
        virtual void init(void);
        virtual bool run(void);
        virtual void clear(void);
        virtual bool isDone(void);
        
        typedef enum
        {
            WS_AUTO_FENDER_LEFT_STATE_NONE = 0,
            WS_AUTO_FENDER_LEFT_STATE_SPIN_FLY_AND_DRIVE,
            WS_AUTO_FENDER_LEFT_STATE_SQUARE_UP_ON_FENDER,
            WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_COAST,
            WS_AUTO_FENDER_LEFT_STATE_SHOOT_FIRST_BALL,
            WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_FLYWHEEL,
            WS_AUTO_FENDER_LEFT_STATE_SHOOT_SECOND_BALL,
            WS_AUTO_FENDER_LEFT_STATE_BACK_UP,
            WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_TURN,
            WS_AUTO_FENDER_LEFT_STATE_TURN_TO_BARRIER,
            WS_AUTO_FENDER_LEFT_STATE_DRIVE_TO_BARRIER,
            WS_AUTO_FENDER_LEFT_STATE_COAST_TO_BARRIER,
            WS_AUTO_FENDER_LEFT_STATE_DONE,
            WS_AUTO_FENDER_LEFT_STATE_NUM
        } WsAutoFenderLeftProgramStateType;
        
    private:
        WsAutoFenderLeftProgram(const WsAutoFenderLeftProgram& rc_rhs);
        const WsAutoFenderLeftProgram& operator=(const WsAutoFenderLeftProgram& rc_rhs);
        
        bool a_done;
        WsAutoFenderLeftProgramStateType a_currentState;
        
        Timer* ap_stateTimer;
        WsTypes::WsAutoFenderLeftProgramConfigType a_configVals;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

