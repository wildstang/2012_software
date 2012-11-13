#ifndef __WsAutoFenderRightProgram_hh__
#define __WsAutoFenderRightProgram_hh__

#include "WsAutonomousProgram.hh"
#include "WsTypes.hh"

class Timer;

class WsAutoFenderRightProgram : public WsAutonomousProgram
{
    public:
        WsAutoFenderRightProgram(void);
        virtual ~WsAutoFenderRightProgram(void);
        
        virtual void init(void);
        virtual bool run(void);
        virtual void clear(void);
        virtual bool isDone(void);
        
        typedef enum
        {
            WS_AUTO_FENDER_RIGHT_STATE_NONE = 0,
            WS_AUTO_FENDER_RIGHT_STATE_SPIN_FLY_AND_DRIVE,
            WS_AUTO_FENDER_RIGHT_STATE_SQUARE_UP_ON_FENDER,
            WS_AUTO_FENDER_RIGHT_STATE_WAIT_FOR_COAST,
            WS_AUTO_FENDER_RIGHT_STATE_SHOOT_FIRST_BALL,
            WS_AUTO_FENDER_RIGHT_STATE_WAIT_FOR_FLYWHEEL,
            WS_AUTO_FENDER_RIGHT_STATE_SHOOT_SECOND_BALL,
            WS_AUTO_FENDER_RIGHT_STATE_BACK_UP,
            WS_AUTO_FENDER_RIGHT_STATE_WAIT_FOR_TURN,
            WS_AUTO_FENDER_RIGHT_STATE_TURN_TO_ALLIANCE_BRIDGE,
            WS_AUTO_FENDER_RIGHT_STATE_DRIVE_TO_ALLIANCE_BRIDGE,
            WS_AUTO_FENDER_RIGHT_STATE_COAST_TO_ALLIANCE_BRIDGE,
            WS_AUTO_FENDER_RIGHT_STATE_DUMP_ALLIANCE_BRIDGE,
            WS_AUTO_FENDER_RIGHT_STATE_DONE,
            WS_AUTO_FENDER_RIGHT_STATE_NUM
        } WsAutoFenderRightProgramStateType;
        
    private:
        WsAutoFenderRightProgram(const WsAutoFenderRightProgram& rc_rhs);
        const WsAutoFenderRightProgram& operator=(const WsAutoFenderRightProgram& rc_rhs);
        
        bool a_done;
        WsAutoFenderRightProgramStateType a_currentState;
        
        Timer* ap_stateTimer;
        WsTypes::WsAutoFenderRightProgramConfigType a_configVals;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

