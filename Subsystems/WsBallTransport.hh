#ifndef __WsBallTransport_hh__
#define __WsBallTransport_hh__

#include "WsSubsystem.hh"
#include "WsTypes.hh"

class WsObserver;
class WsBallTransport : public WsSubsystem
{
    public:
        WsBallTransport(void);
        ~WsBallTransport(void);
        
        // All WsSubsystems must implement this method
        virtual void update(void);
        
        void acceptNewAccumulatorMotorButtonState(const bool& rc_newButtonState);
        void acceptNewEscalatorDownButtonState(const bool& rc_newButtonState);
        void acceptNewEscalatorUpButtonState(const bool& rc_newButtonState);
        void acceptNewShooterButtonState(const bool& rc_newButtonState);
        void acceptNewShooterOverrideButtonState(const bool& rc_newButtonState);
        void acceptNewFlywheelSpeed(const float& rc_newSpeed);
        void acceptNewFixedMotorSpeeds(WsTypes::WsBallTransportMotorValueType& r_speeds);
        
    private:
        void updateAccumulator(void);
        void updateEscalator(void);
        void updateElevator(void);
        void updateShooterCylinder(void);
        bool isClearToShoot(void);
        
        WsObserver*     ap_accumulatorMotorButtonObserver;
        WsObserver*     ap_escalatorUpButtonObserver;
        WsObserver*     ap_escalatorDownButtonObserver;
        WsObserver*     ap_shooterButtonObserver;
        WsObserver*     ap_shooterOverrideButtonObserver;
        WsObserver*     ap_flywheelSpeedObserver;
        
        bool            a_accumulatorMotorButtonState;
        bool            a_escalatorUpButtonState;
        bool            a_escalatorDownButtonState;
        bool            a_shooterButtonState;
        bool            a_shooterOverrideButtonState;
        double          a_flywheelSpeed;
        
        WsTypes::WsBallTransportMotorValueType a_motorSpeeds;
        
    private:
        WsBallTransport(const WsBallTransport& rc_rhs);
        const WsBallTransport& operator=(const WsBallTransport& rc_rhs);
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

