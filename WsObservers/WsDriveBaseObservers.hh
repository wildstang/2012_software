#ifndef __WsDriveBaseObservers_hh__
#define __WsDriveBaseObservers_hh__

#include "WsObserver.hh"

class WsDriveBase;

//-----------------------------------------------------------------------------
class WsAntiturboButtonObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsAntiturboButtonObserver(WsDriveBase* p_subsystem);
        virtual ~WsAntiturboButtonObserver(void);
        virtual void update(void);
        
    private:
        WsDriveBase* ap_subsystem;
        bool         a_currentState;
        
    private:
        WsAntiturboButtonObserver(const WsAntiturboButtonObserver& rc_rhs);
        WsAntiturboButtonObserver& operator=(const WsAntiturboButtonObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsLandingGearButtonObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsLandingGearButtonObserver(WsDriveBase* p_subsystem);
        virtual ~WsLandingGearButtonObserver(void);
        virtual void update(void);
        
    private:
        WsDriveBase* ap_subsystem;
        bool         a_currentState;
        
    private:
        WsLandingGearButtonObserver(const WsLandingGearButtonObserver& rc_rhs);
        WsLandingGearButtonObserver operator=(const WsLandingGearButtonObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsLandingGearRetractionSwitchObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsLandingGearRetractionSwitchObserver(WsDriveBase* p_subsystem);
        virtual ~WsLandingGearRetractionSwitchObserver(void);
        virtual void update(void);
        
    private:
        WsDriveBase* ap_subsystem;
        bool         a_currentState;
        
    private:
        WsLandingGearRetractionSwitchObserver(const WsLandingGearRetractionSwitchObserver& rc_rhs);
        WsLandingGearRetractionSwitchObserver operator=(const WsLandingGearRetractionSwitchObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsShifterButtonObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsShifterButtonObserver(WsDriveBase* p_subsystem);
        virtual ~WsShifterButtonObserver(void);
        virtual void update(void);
        
    private:
        WsDriveBase* ap_subsystem;
        bool         a_currentState;
        
    private:
        WsShifterButtonObserver(const WsShifterButtonObserver& rc_rhs);
        WsShifterButtonObserver& operator=(const WsShifterButtonObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsAccelerationEnableFlagObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsAccelerationEnableFlagObserver(WsDriveBase* p_subsystem);
        virtual ~WsAccelerationEnableFlagObserver(void);
        virtual void update(void);
        
    private:
        WsDriveBase* ap_subsystem;
        bool         a_currentState;
        
    private:
        WsAccelerationEnableFlagObserver(const WsAccelerationEnableFlagObserver& rc_rhs);
        WsAccelerationEnableFlagObserver& operator=(const WsAccelerationEnableFlagObserver& rc_rhs);
};

#endif // __WsDriveBaseObservers_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

