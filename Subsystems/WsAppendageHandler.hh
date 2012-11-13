#ifndef __WsAppendageHandler_hh__
#define __WsAppendageHandler_hh__

#include "WsSubsystem.hh"

class Timer;

class WsObserver;
class WsAppendageHandler : public WsSubsystem
{
    public:
        typedef enum
        {
            WS_ROBOT_GRABBER_DEPLOYED_STATE = 0,
            WS_ROBOT_GRABBER_RETRACTING_STATE,
            WS_ROBOT_GRABBER_RETRACTED_STATE,
            WS_ROBOT_GRABBER_FIRST_DEPLOY_STATE,
            WS_ROBOT_GRABBER_SECOND_DEPLOY_STATE,
            WS_ACCUMULATOR_DEPLOYED_STATE,
            WS_ACCUMULATOR_RETRACTING_STATE,
            WS_ACCUMULATOR_RETRACTED_STATE,
            WS_APPENDAGE_WAIT_STATE,
            WS_APPENDAGE_STATE_NUM
        } WsAppendageControlStateType;
        
    public:
        WsAppendageHandler(void);
        virtual ~WsAppendageHandler(void);
        void update(void);
        
        void acceptNewAccumulatorDeployButtonState(bool oldButtonState, bool newButtonState);
        void acceptNewRobotGrabberButtonState(bool oldButtonState, bool newButtonState);
        
    private:
        WsAppendageHandler(const WsAppendageHandler& rc_rhs);
        const WsAppendageHandler& operator=(const WsAppendageHandler& rc_rhs);
        
        WsAppendageControlStateType  a_currentState;
        bool                         a_robotGrabberProcessedButtonState;
        bool                         a_accumulatorProcessedButtonState;
        WsObserver*                  ap_accumulatorDeployButtonObserver;
        WsObserver*                  ap_robotGrabberButtonObserver;
        Timer*                       ap_retractTimer;
};


#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

