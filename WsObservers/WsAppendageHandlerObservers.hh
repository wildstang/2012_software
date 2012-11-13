#ifndef __WsAppendageHandlerObservers_hh__
#define __WsAppendageHandlerObservers_hh__

#include "WsObserver.hh"

class WsAppendageHandler;
//-----------------------------------------------------------------------------
class WsAccumulatorDeployButtonObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsAccumulatorDeployButtonObserver(WsAppendageHandler* p_appendageHandler);
        virtual ~WsAccumulatorDeployButtonObserver(void);
        virtual void update(void);
        
    private:
        WsAppendageHandler* ap_appendageHandlerSubsystem;
        bool a_accumulatorDeployButtonState;
        
    private:
        WsAccumulatorDeployButtonObserver(const WsAccumulatorDeployButtonObserver& rc_rhs);
        WsAccumulatorDeployButtonObserver operator=(const WsAccumulatorDeployButtonObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsRobotGrabberButtonObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsRobotGrabberButtonObserver(WsAppendageHandler* p_appendageHandler);
        virtual ~WsRobotGrabberButtonObserver(void);
        virtual void update(void);
        
    private:
        WsAppendageHandler* ap_appendageHandlerSubsystem;
        bool                a_robotGrabberButtonState;
        
    private:
        WsRobotGrabberButtonObserver(const WsRobotGrabberButtonObserver& rc_rhs);
        WsRobotGrabberButtonObserver& operator=(const WsRobotGrabberButtonObserver& rc_rhs);
};


#endif // __WsAppendageHandlerObservers_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

