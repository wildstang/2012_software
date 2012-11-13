#ifndef _WsAutonomousProgramManager_hh__
#define _WsAutonomousProgramManager_hh__

#include "WsTypes.hh"

class WsAutonomousProgram;
class SendableChooser;
class Timer;
class WsObserver;

class WsAutonomousProgramManager
{
    public:
        static WsAutonomousProgramManager* instance(void);
        virtual ~WsAutonomousProgramManager(void);
        
        void lockAutonomousConfiguration(void);
        float getLockedDelay(void);
        WsAutonomousProgram* getLatchedProgram(void);
        WsTypes::WsAutonomousProgramIdType getSelectedProgramId(void);
        WsTypes::WsAutonomousProgramIdType getLockedProgramId(void);
        void run(void);
        void clear(void);
        
        void acceptNewLockInState(const bool& rc_newState, const bool& rc_oldState);
        void acceptNewProgramSelectorValue(const float& rc_pot);
        
        typedef enum
        {
            WS_AUTO_PROGRAM_STATE_NONE = 0,
            WS_AUTO_PROGRAM_STATE_INIT,
            WS_AUTO_PROGRAM_STATE_DELAY,
            WS_AUTO_PROGRAM_STATE_RUN,
            WS_AUTO_PROGRAM_STATE_DONE,
            WS_AUTO_PROGRAM_STATE_NUM
        } WsAutonomousProgramStateType;
        
    protected:
        WsAutonomousProgramManager(void);
        
    private:
        WsAutonomousProgramManager(const WsAutonomousProgramManager& rc_rhs);
        const WsAutonomousProgramManager& operator=(const WsAutonomousProgramManager& rc_rhs);
        
        static WsAutonomousProgramManager*   asp_instance;
        SendableChooser*                     ap_autoProgramChooser;
        SendableChooser*                     ap_autoDelayChooser;
        
        // These are locked in during autonomous init
        WsAutonomousProgram*                 ap_latchedProgram;
        WsTypes::WsAutonomousProgramIdType   a_latchedProgramId;
        float                                a_lockedDelay;
        bool                                 a_lockInState;
        float                                a_selectorPot;
        
        WsAutonomousProgram*                 a_autonomousProgramContainer[WsTypes::WS_AUTO_PROGRAM_NUM];
        
        // Timer to implement autonomous delay
        Timer* ap_autonomousDelayTimer;
        
        WsAutonomousProgramStateType a_currentState;
        
        WsObserver* ap_lockInSwitchObserver;
        WsObserver* ap_programSelectorPotValueObserver;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

