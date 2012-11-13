#ifndef __WsAutoLockInSwitchObserver_hh__
#define __WsAutoLockInSwitchObserver_hh__

#include "WsObserver.hh"

class WsAutonomousProgramManager;
//-----------------------------------------------------------------------------
class WsAutoLockInSwitchObserver : public WsObserver
    //-----------------------------------------------------------------------------
{
    public:
        WsAutoLockInSwitchObserver(WsAutonomousProgramManager* p_autoProgramMan);
        virtual ~WsAutoLockInSwitchObserver(void);
        virtual void update(void);
        
    private:
        WsAutonomousProgramManager* ap_autoProgramMan;
        bool                        a_currentState;
        
    private:
        WsAutoLockInSwitchObserver(const WsAutoLockInSwitchObserver& rc_rhs);
        WsAutoLockInSwitchObserver& operator=(const WsAutoLockInSwitchObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsAutoProgramSelectorObserver : public WsObserver
    //-----------------------------------------------------------------------------
{
    public:
        WsAutoProgramSelectorObserver(WsAutonomousProgramManager* p_autoProgramMan);
        virtual ~WsAutoProgramSelectorObserver(void);
        virtual void update(void);
        
    private:
        WsAutonomousProgramManager* ap_autoProgramMan;
        float                       a_currentValue;
        
    private:
        WsAutoProgramSelectorObserver(const WsAutoProgramSelectorObserver& rc_rhs);
        WsAutoProgramSelectorObserver& operator=(const WsAutoProgramSelectorObserver& rc_rhs);
};

#endif // __WsAutoProgramSelectorObserver

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

