#ifndef __header_WsRobot__
#define __header_WsRobot__

#include "WPILib.h"

class WsRobot : public IterativeRobot
{
    public:
        WsRobot(void);
        virtual ~WsRobot(void);
        
        virtual void RobotInit(void);
        virtual void DisabledInit(void);
        virtual void AutonomousInit(void);
        virtual void TeleopInit(void);
        virtual void DisabledPeriodic(void);
        virtual void AutonomousPeriodic(void);
        virtual void TeleopPeriodic(void);
        virtual void DisabledContinuous(void);
        virtual void AutonomousContinuous(void);
        virtual void TeleopContinuous(void);
        
    private:
        WsRobot(const WsRobot& rc_rhs);
        const WsRobot& operator=(const WsRobot& rc_rhs);
        
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

