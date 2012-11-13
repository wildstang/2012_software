#ifndef _WsAutoTipShootProgram
#define _WsAutoTipShootProgram

#include "WsAutonomousProgram.hh"
#include "WsTypes.hh"

class Timer;

class WsAutoTipShootProgram : public WsAutonomousProgram
{
    public:
        WsAutoTipShootProgram(void);
        virtual ~WsAutoTipShootProgram(void);
        
        virtual void init(void);
        virtual bool run(void);
        virtual void clear(void);
        virtual bool isDone(void);
        
        typedef enum
        {
            WS_AUTO_TIPSHOOT_STATE_NONE = 0,
            WS_AUTO_TIPSHOOT_STATE_START,
            WS_AUTO_TIPSHOOT_STATE_WAIT_FOR_FLYWHEEL_1,
            WS_AUTO_TIPSHOOT_STATE_SHOOT_1,
            WS_AUTO_TIPSHOOT_STATE_DRIVE_TO_COOP_BRIDGE,
            WS_AUTO_TIPSHOOT_STATE_WAIT_FOR_COAST,
            WS_AUTO_TIPSHOOT_STATE_PUT_DOWN_COOP_BRIDGE,
            WS_AUTO_TIPSHOOT_STATE_DRIVE_ONTO_COOP_BRIDGE,
            WS_AUTO_TIPSHOOT_STATE_ACCUMULATE_BALLS,
            WS_AUTO_TIPSHOOT_STATE_DRIVE_BACK_TO_SHOOTING_POSITION,
            WS_AUTO_TIPSHOOT_STATE_WAIT_FOR_ANGLE_CALC,
            WS_AUTO_TIPSHOOT_STATE_ADJUST_PRESET_FOR_ANGLE,
            WS_AUTO_TIPSHOOT_STATE_WAIT_FOR_TURRET_STATE,
            WS_AUTO_TIPSHOOT_STATE_WAIT_FOR_FLYWHEEL_2,
            WS_AUTO_TIPSHOOT_STATE_SHOOT_2,
            WS_AUTO_TIPSHOOT_STATE_WAIT_FOR_FLYWHEEL_3,
            WS_AUTO_TIPSHOOT_STATE_SHOOT_3,
            WS_AUTO_TIPSHOOT_STATE_WAIT_FOR_FLYWHEEL_4,
            WS_AUTO_TIPSHOOT_STATE_SHOOT_4,
            WS_AUTO_TIPSHOOT_STATE_DONE,
            WS_AUTO_TIPSHOOT_STATE_NUM
        } WsAutoTipShootStateType;
        
    private:
        WsAutoTipShootProgram(const WsAutoTipShootProgram& rc_rhs);
        const WsAutoTipShootProgram& operator=(const WsAutoTipShootProgram& rc_rhs);
        
        bool a_done;
        float a_gyro_at_start;
        WsAutoTipShootStateType a_currentState;
        
        Timer* ap_stateTimer;
        WsTypes::WsAutoTipShootProgramConfigType a_configVals;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

