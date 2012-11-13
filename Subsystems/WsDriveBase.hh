#ifndef __WsDriveBase_hh__
#define __WsDriveBase_hh__

#include "WsSubsystem.hh"

class Timer;
class WsObserver;
class WsDriveBase : public WsSubsystem
{
    public:
        WsDriveBase(void);
        ~WsDriveBase(void);
        
        typedef enum
        {
            WS_DB_LG_OVERRIDE_STATE_NONE = 0,
            WS_DB_LG_OVERRIDE_STATE_KEEP_DEPLOYED,
            WS_DB_LG_OVERRIDE_STATE_KEEP_RETRACTED,
            WS_DB_LG_OVERRIDE_STATE_NORMAL,
            WS_DB_LG_OVERRIDE_STATE_NUM
        } WsLandingGearOverrideStateType;
        
        void update(void);
        void setThrottle(const float& rc_throttle);
        void setHeading(const float& rc_heading);
        void setLandingGear(const bool& rc_landingGear);
        void getShifterState(bool& r_shifterState);
        void setShifterState(const bool& rc_shifter);
        void setDriveStraight(const bool& rc_driveStraight);
        void setAutoQuickTurn(const bool& rc_quickTurn);
        void checkAutoQuickTurn(bool& r_state);
        void acceptNewAntiturboButtonState(const bool& rc_oldState, const bool& rc_newState);
        void acceptNewShifterButtonState(const bool& rc_oldState, const bool& rc_newState);
        void acceptNewAccelerationEnableFlag(const bool& rc_oldState, const bool& rc_newState);
        void notifyLandingGearInputChange(void);
        
    private:
        void updateDriveMotors(void);
        void updateLandingGear(void);
        
    private:
        WsDriveBase(const WsDriveBase& rc_rhs);
        const WsDriveBase& operator=(const WsDriveBase& rc_rhs);
        
        float a_driveBaseHeading;
        float a_driveBaseThrottle;
        bool  a_landingGearFlag;         // landing gear: true is deployed, false is retracted
        bool  a_shifterFlag;             // shifter: true is high, false is low
        bool  a_driveStraightFlag;       // drive straight: true is drive straight, false is normal halo drive
        bool  a_quickAutoTurnFlag;       // Quick turn true is enabled, false is disabled
        bool  a_antiTurboFlag;           // anti turbo flag: true of on, false is off
        bool  a_accelerationEnableFlag;  // If true, throttle and heading joystick values will be accelerated (smoothed)
        
        WsLandingGearOverrideStateType a_landingGearOverrideState;
        Timer*                         ap_landingGearOverrideTimer;
        WsObserver*                    ap_antiturboButtonObserver;
        WsObserver*                    ap_shifterButtonObserver;
        WsObserver*                    ap_landingGearButtonObserver;
        WsObserver*                    ap_landingGearRetractionSwitchObserver;
        WsObserver*                    ap_accelerationEnableFlagObserver;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------


