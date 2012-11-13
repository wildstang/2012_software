#ifndef __WsBallTransportObservers_hh__
#define __WsBallTransportObservers_hh__

#include "WsObserver.hh"

class WsBallTransport;
//-----------------------------------------------------------------------------
class WsBtAccumulatorMotorButtonObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsBtAccumulatorMotorButtonObserver(WsBallTransport* p_ballTransport);
        virtual ~WsBtAccumulatorMotorButtonObserver(void);
        virtual void update(void);
        
    private:
        WsBallTransport* ap_ballTransportSubsystem;
        bool             a_currentButtonState;
        
    private:
        WsBtAccumulatorMotorButtonObserver(const WsBtAccumulatorMotorButtonObserver& rc_rhs);
        WsBtAccumulatorMotorButtonObserver& operator=(const WsBtAccumulatorMotorButtonObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsBtEscalatorDownButtonObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsBtEscalatorDownButtonObserver(WsBallTransport* p_ballTransport);
        virtual ~WsBtEscalatorDownButtonObserver(void);
        virtual void update(void);
        
    private:
        WsBallTransport* ap_ballTransportSubsystem;
        bool             a_currentButtonState;
        
    private:
        WsBtEscalatorDownButtonObserver(const WsBtEscalatorDownButtonObserver& rc_rhs);
        WsBtEscalatorDownButtonObserver& operator=(const WsBtEscalatorDownButtonObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsBtEscalatorUpButtonObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsBtEscalatorUpButtonObserver(WsBallTransport* p_ballTransport);
        virtual ~WsBtEscalatorUpButtonObserver(void);
        virtual void update(void);
        
    private:
        WsBallTransport* ap_ballTransportSubsystem;
        bool             a_currentButtonState;
        
    private:
        WsBtEscalatorUpButtonObserver(const WsBtEscalatorUpButtonObserver& rc_rhs);
        WsBtEscalatorUpButtonObserver& operator=(const WsBtEscalatorUpButtonObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsBtFlywheelSpeedObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsBtFlywheelSpeedObserver(WsBallTransport* p_ballTransport);
        virtual ~WsBtFlywheelSpeedObserver(void);
        virtual void update(void);
        
    private:
        WsBallTransport* ap_ballTransportSubsystem;
        float            a_currentSpeed;
        
    private:
        WsBtFlywheelSpeedObserver(const WsBtFlywheelSpeedObserver& rc_rhs);
        WsBtFlywheelSpeedObserver& operator=(const WsBtFlywheelSpeedObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsBtShooterButtonObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsBtShooterButtonObserver(WsBallTransport* p_ballTransport);
        virtual ~WsBtShooterButtonObserver(void);
        virtual void update(void);
        
    private:
        WsBallTransport* ap_ballTransportSubsystem;
        bool             a_currentButtonState;
        
    private:
        WsBtShooterButtonObserver(const WsBtShooterButtonObserver& rc_rhs);
        WsBtShooterButtonObserver& operator=(const WsBtShooterButtonObserver& rc_rhs);
};

//-----------------------------------------------------------------------------
class WsBtShooterOverrideButtonObserver : public WsObserver
//-----------------------------------------------------------------------------
{
    public:
        WsBtShooterOverrideButtonObserver(WsBallTransport* p_ballTransport);
        virtual ~WsBtShooterOverrideButtonObserver(void);
        virtual void update(void);
        
    private:
        WsBallTransport* ap_ballTransportSubsystem;
        bool             a_currentButtonState;
        
    private:
        WsBtShooterOverrideButtonObserver(const WsBtShooterOverrideButtonObserver& rc_rhs);
        WsBtShooterOverrideButtonObserver& operator=(const WsBtShooterOverrideButtonObserver& rc_rhs);
};


#endif // __WsBallTransportObservers_hh__
//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

