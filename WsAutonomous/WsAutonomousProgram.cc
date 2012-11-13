#include "WsAutonomousProgram.hh"
#include "WsInputFacade.hh"
#include "WsOutputFacade.hh"
#include "WsDataIndicationFacade.hh"
#include "WsLogger.hh"

const float OI_FLYWHEEL_POT_MANUAL = 2.79f;
const float OI_FLYWHEEL_POT_PRESET = 1.65;
const float OI_FLYWHEEL_POT_OFF = 0.01f;

WsAutonomousProgram::WsAutonomousProgram(const char* name)
{
    a_programName = name;
}

WsAutonomousProgram::~WsAutonomousProgram(void)
{
}

void WsAutonomousProgram::init(void)
{
    // Ensure the flood light (now flywheel compensation enable) is off
    bool flood_light_state = false;
    WsInputFacade::instance()->setFloodLightButtonState(flood_light_state);
    
    float flywheel_state = OI_FLYWHEEL_POT_OFF;
    WsInputFacade::instance()->setFlywheelStatePotValue(flywheel_state);
    WsInputFacade::instance()->resetDriveEncoderCounts();
    
    //Ensure we are in low gear
    WsTypes::WsShifterStateType shifter_state = WsTypes::WS_S_HIGHGEAR;
    WsOutputFacade::instance()->getShifterState(shifter_state);
    if (WsTypes::WS_S_HIGHGEAR == shifter_state)
    {
        WsInputFacade::instance()->setShifterButtonState(false);
        WsInputFacade::instance()->setShifterButtonState(true);
        WsInputFacade::instance()->setShifterButtonState(false);
    }
    
    //Ensure the accumulator is retracted
    WsTypes::WsAccumulatorStateType accumulator_state = WsTypes::WS_ACCUMULATOR_RETRACTED;
    WsOutputFacade::instance()->getAccumulatorState(accumulator_state);
    if (WsTypes::WS_ACCUMULATOR_EXTENDED == accumulator_state)
    {
        WsInputFacade::instance()->setAccumulatorDeployButtonState(false);
        WsInputFacade::instance()->setAccumulatorDeployButtonState(true);
        WsInputFacade::instance()->setAccumulatorDeployButtonState(false);
    }
    
    //Ensure the brake is disengaged
    WsTypes::WsBridgeBrakeCylinderStateType bridgeBrake_state = WsTypes::WS_BRIDGE_BRAKE_CYLINDER_DISENGAGED;
    WsOutputFacade::instance()->getBridgeBrakeCylinderState(bridgeBrake_state);
    if (WsTypes::WS_BRIDGE_BRAKE_CYLINDER_ENGAGED == bridgeBrake_state)
    {
        WsInputFacade::instance()->setBridgeBrakeActivationButtonState(false);
        WsInputFacade::instance()->setBridgeBrakeActivationButtonState(true);
        WsInputFacade::instance()->setBridgeBrakeActivationButtonState(false);
    }
    
    //Ensure the pogo stick is up
    WsTypes::WsPogoStickCylinderStateType pogoStick_state = WsTypes::WS_POGO_STICK_CYLINDER_INVALID;
    WsOutputFacade::instance()->getPogoStickCylinderState(pogoStick_state);
    if (WsTypes::WS_POGO_STICK_CYLINDER_DOWN == pogoStick_state)
    {
        WsInputFacade::instance()->setPogoStickDeploymentButtonState(false);
        WsInputFacade::instance()->setPogoStickDeploymentButtonState(true);
        WsInputFacade::instance()->setPogoStickDeploymentButtonState(false);
    }
    
    //Ensure the robot grabber is retracted
    WsTypes::WsRobotGrabberCylinderStateType robotGrabber_inner_cylinder_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
    WsTypes::WsRobotGrabberCylinderStateType robotGrabber_outer_cylinder_state = WsTypes::WS_ROBOT_GRABBER_CYLINDER_RETRACTED;
    WsOutputFacade::instance()->getRobotGrabberInnerCylinderState(robotGrabber_inner_cylinder_state);
    WsOutputFacade::instance()->getRobotGrabberOuterCylinderState(robotGrabber_outer_cylinder_state);
    if ((WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED == robotGrabber_inner_cylinder_state)
        || (WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED == robotGrabber_outer_cylinder_state))
    {
        WsInputFacade::instance()->setRobotGrabberButtonState(false);
        WsInputFacade::instance()->setRobotGrabberButtonState(true);
        WsInputFacade::instance()->setRobotGrabberButtonState(false);
    }
    //Ensure the antiturbo is disabled
    bool bAntiTurbo = false;
    WsDataIndicationFacade::instance()->getDriveBaseAntiTurboEnabled(bAntiTurbo);
    if (true == bAntiTurbo)
    {
        WsInputFacade::instance()->setAntiTurboButtonState(false);
        WsInputFacade::instance()->setAntiTurboButtonState(true);
        WsInputFacade::instance()->setAntiTurboButtonState(false);
    }
    
}

std::string WsAutonomousProgram::getName(void)
{
    return a_programName;
}

bool
WsAutonomousProgram::isDone(void)
{
    WS_LOG_WARNING("AUTONOMOUS: Default isDone called");
    return true;
}

