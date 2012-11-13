#ifndef _WsAutoDriveProgram
#define _WsAutoDriveProgram

#include "WsAutonomousProgram.hh"
#include "WsTypes.hh"

class Timer;

class WsAutoDriveProgram : public WsAutonomousProgram
{
    public:
        WsAutoDriveProgram(void);
        virtual ~WsAutoDriveProgram(void);
        
        virtual void init(void);
        virtual bool run(void);
        virtual void clear(void);
        virtual bool isDone(void);
        
        typedef enum
        {
            WS_AUTO_DRIVE_STATE_NONE = 0,
            WS_AUTO_DRIVE_STATE_START,
            WS_AUTO_DRIVE_STATE_DRIVE_TO_COOP_BRIDGE,
            WS_AUTO_DRIVE_STATE_WAIT_FOR_COAST,
            WS_AUTO_DRIVE_STATE_PUT_DOWN_COOP_BRIDGE,
            WS_AUTO_DRIVE_STATE_DRIVE_BACK_TO_SHOOTING_POSITION,
            WS_AUTO_DRIVE_STATE_WAIT_FOR_ANGLE_CALC,
            WS_AUTO_DRIVE_STATE_ADJUST_PRESET_FOR_ANGLE,
            WS_AUTO_DRIVE_STATE_WAIT_FOR_TURRET_STATE,
            WS_AUTO_DRIVE_STATE_WAIT_FOR_FLYWHEEL_1,
            WS_AUTO_DRIVE_STATE_SHOOT_1,
            WS_AUTO_DRIVE_STATE_WAIT_FOR_FLYWHEEL_2,
            WS_AUTO_DRIVE_STATE_SHOOT_2,
            WS_AUTO_DRIVE_STATE_WAIT_FOR_FLYWHEEL_3,
            WS_AUTO_DRIVE_STATE_SHOOT_3,
            WS_AUTO_DRIVE_STATE_DONE,
            WS_AUTO_DRIVE_STATE_NUM
        } WsAutoDriveStateType;
        
    private:
        WsAutoDriveProgram(const WsAutoDriveProgram& rc_rhs);
        const WsAutoDriveProgram& operator=(const WsAutoDriveProgram& rc_rhs);
        
        bool a_done;
        float a_gyro_at_start;
        WsAutoDriveStateType a_currentState;
        
        Timer* ap_stateTimer;
        WsTypes::WsAutoDriveProgramConfigType a_configVals;
        
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

