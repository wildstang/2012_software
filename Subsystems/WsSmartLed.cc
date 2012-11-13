#include "WsSmartLed.hh"
#include "WsLogger.hh"
#include "WsOutputFacade.hh"

#include "DigitalModule.h"
#include "I2C.h"
#include "Notifier.h"
#include "DriverStation.h"
#include "Timer.h"


WsSmartLed* WsSmartLed::asp_instance = NULL;
WsSmartLed::WsSmartLed(void)
    : WsSubsystem("WsSmartLed")
    , ap_i2cInterface(NULL)
    , ap_updateLoopNotifier(NULL)
{
    WS_LOG_TRACE("Begin");
    
    //
    // Get an I2C instance on the digital module
    //  The GetI2C() call returns a new I2C object that we are responsible for
    //  de-allocating.
    //
    DigitalModule* p_dm = DigitalModule::GetInstance(1);
    ap_i2cInterface = p_dm->GetI2C(0x52 << 1);
    
    ap_updateLoopNotifier = new Notifier(WsSmartLed::callUpdate, this);
    ap_updateLoopNotifier->StartPeriodic(0.250f);
    WS_LOG_TRACE("End");
}

WsSmartLed::~WsSmartLed(void)
{
    WS_LOG_TRACE("Begin");
    
    // Make sure the notifier doesn't call again
    ap_updateLoopNotifier->Stop();
    
    delete ap_i2cInterface;         ap_i2cInterface = NULL;
    delete ap_updateLoopNotifier;   ap_updateLoopNotifier = NULL;
    
    WS_LOG_TRACE("End");
}

WsSmartLed*
WsSmartLed::instance(void)
{
    if (NULL == asp_instance)
    { asp_instance = new WsSmartLed(); }
    return asp_instance;
}

void
WsSmartLed::callUpdate(void* p_this)
{
    WsSmartLed* p_sm = reinterpret_cast<WsSmartLed*>(p_this);
    p_sm->update();
}

void
WsSmartLed::update(void)
{
    WS_LOG_TRACE("Begin");
    
    // Data buffer to hold I2C bytes
    UINT8 data_bytes[5];
    ::memset(data_bytes, 0x00, sizeof(data_bytes));
    
    UINT8* p_commandByte    = &data_bytes[0];
    UINT8* p_payloadByteOne = &data_bytes[1];
    UINT8* p_payloadByteTwo = &data_bytes[2];
    
    // If we end up in a weird state, don't send anything
    bool should_send_data = true;
    
    // Get Data Handles
    DriverStation* p_ds = DriverStation::GetInstance();
    WsOutputFacade* p_of = WsOutputFacade::instance();
    
    // Get all inputs relevant to the LEDs
    bool is_robot_enabled            = p_ds->IsEnabled();
    bool is_robot_teleop             = p_ds->IsOperatorControl();
    bool is_robot_auton              = p_ds->IsAutonomous();
    DriverStation::Alliance alliance = p_ds->GetAlliance();
    UINT32 station_location          = p_ds->GetLocation();
    bool robot_grabber_state         = false;
    bool pogo_stick_state            = false;
    bool bridge_brake_state          = false;
    
    // Grabber
    WsTypes::WsRobotGrabberCylinderStateType grabber_cyl_1;
    WsTypes::WsRobotGrabberCylinderStateType grabber_cyl_2;
    p_of->getRobotGrabberInnerCylinderState(grabber_cyl_1);
    p_of->getRobotGrabberOuterCylinderState(grabber_cyl_2);
    robot_grabber_state = (WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED == grabber_cyl_1) ||
                          (WsTypes::WS_ROBOT_GRABBER_CYLINDER_EXTENDED == grabber_cyl_2);
                          
    // Bridge Brake
    WsTypes::WsBridgeBrakeCylinderStateType brake_cyl;
    p_of->getBridgeBrakeCylinderState(brake_cyl);
    bridge_brake_state = (WsTypes::WS_BRIDGE_BRAKE_CYLINDER_ENGAGED == brake_cyl);
    
    // Pogo Stick
    WsTypes::WsPogoStickCylinderStateType pogo_cyl;
    p_of->getPogoStickCylinderState(pogo_cyl);
    pogo_stick_state = (WsTypes::WS_POGO_STICK_CYLINDER_DOWN == pogo_cyl);
    
    if (true == is_robot_enabled)
    {
        if (true == is_robot_teleop)
        {
            //-----------------------------------------------------------------
            // Handle TeleOp signalling here
            //-----------------------------------------------------------------
            if ((true == robot_grabber_state) ||
                (true == pogo_stick_state) ||
                (true == bridge_brake_state))
            {
                // Display Robot Grabber
                // 0x01 0x6C 0x0A  RED
                // 0x01 0x6C 0x0B  GREEN
                *p_commandByte = 0x01;
                *p_payloadByteOne = 0x6C;
                *p_payloadByteTwo = (true == robot_grabber_state) ? 0x0B : 0x0A;
                this->sendPayloadData(data_bytes, sizeof(data_bytes));
                Wait(0.02);
                
                // Display Brake Activation
                // 0x01 0x5C 0x6E  RED
                // 0x01 0x5C 0x7E  GREEN
                *p_commandByte = 0x01;
                *p_payloadByteOne = 0x5C;
                *p_payloadByteTwo = (true == bridge_brake_state) ? 0x7E : 0x6E;
                this->sendPayloadData(data_bytes, sizeof(data_bytes));
                Wait(0.02);
                
                // Display Pogo Stick
                // 0x01 0x4C 0x5C  RED
                // 0x01 0x4C 0xE2  GREEN
                *p_commandByte = 0x01;
                *p_payloadByteOne = 0x4C;
                *p_payloadByteTwo = (true == pogo_stick_state) ? 0xE2 : 0x5C;
                this->sendPayloadData(data_bytes, sizeof(data_bytes));
                Wait(0.02);
                
                // All data is sent, don't send the last one again
                should_send_data = false;
            }
            else
            {
                // Display Tie-Dye Mode
                // 0x01 0x1C 0x1D
                *p_commandByte = 0x01;
                *p_payloadByteOne = 0x1C;
                *p_payloadByteTwo = 0x1D;
            }
        }
        else if (true == is_robot_auton)
        {
            // Handle Autonomous signalling here
        }
        else
        {
            // What in the world is this?!
            //  Don't send data
            should_send_data = false;
        }
    }
    else
    {
        //---------------------------------------------------------------------
        // Handle Disabled signalling here
        //---------------------------------------------------------------------
        
        // Display alliance
        // Command:  0x04
        // Byte 1:   0x52 - Red Alliance, 0x47 - Blue Alliance
        // Byte 2:   0x01, 0x02, 0x03 - Station Number
        switch (alliance)
        {
            case DriverStation::kRed:
            {
                *p_commandByte = 0x04;
                *p_payloadByteOne = 0x52;
                *p_payloadByteTwo = station_location;
            } break;
            
            case DriverStation::kBlue:
            {
                *p_commandByte = 0x04;
                *p_payloadByteOne = 0x47;
                *p_payloadByteTwo = station_location;
            } break;
            
            case DriverStation::kInvalid:
            default:
            {
                should_send_data = false;
            } break;
        }
        
        if ((station_location < 1) ||
            (station_location > 3))
        {
            should_send_data = false;
        }
    }
    
    if (true == should_send_data)
    {
        this->sendPayloadData(data_bytes, sizeof(data_bytes));
    }
    
    WS_LOG_TRACE("End");
}

void
WsSmartLed::sendPayloadData(UINT8* p_data, UINT8 size)
{
    if (5 != size)
    { return; }
    
    // Extremely fast and cheap data confirmation algorithm
    p_data[3] = ~p_data[1];
    p_data[4] = ~p_data[2];
    
    // Send the bytes - this is a blocking call
    ap_i2cInterface->Transaction(p_data, size, NULL, 0);
    
    // Log to verify the transaction
    WS_LOG_DEBUG("I2C Transaction:  0x%02x 0x%02x 0x%02x 0x%02x 0x%02x",
                 p_data[0],
                 p_data[1],
                 p_data[2],
                 p_data[3],
                 p_data[4]);
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

