#include "WsOutputFacade.hh"
#include "WsRobot.hh"
#include "WsLogger.hh"
#include "WsVersion.hh"
#include "DriverStationLCD.h"
#include "WsInputFacade.hh"
#include "WsDataIndicationFacade.hh"
#include "WsVision.hh"
#include "WsOutputFacade.hh"
#include "WsConfigFacade.hh"
#include "WsAutonomousProgramManager.hh"
#include "WsAutonomousProgram.hh"
#include "WsSubsystemContainer.hh"
#include "WsSmartLed.hh"

WsRobot::WsRobot(void)
{
    // Create our logger instance
    WsLogger::instance();
    
    // Control which logs are enabled...
    WsLogger::instance()->enableLoggingForAllLevels();
    //WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_FATAL);
    //WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_ERROR);
    //WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_WARNING);
//    WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_NOTICE);
    WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_INFO);
    WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_DEBUG);
    WsLogger::instance()->disableLoggingForLevel(WsLogger::LEVEL_TRACE);
    
    // This log cannot be moved any higher in the code
    WS_LOG_TRACE("Begin");
    WS_LOG_ALWAYS("WsRobot Constructor!!");
    
    //Shows version information on Driver Station LCD
    DriverStationLCD* p_driver_station_lcd = DriverStationLCD::GetInstance();
    
    p_driver_station_lcd->PrintfLine(DriverStationLCD::kUser_Line1, "Wildstang Software");
    p_driver_station_lcd->PrintfLine(DriverStationLCD::kUser_Line2, " Version: %s", WS_VERSION);
    p_driver_station_lcd->PrintfLine(DriverStationLCD::kUser_Line3, " Date: %s", WS_BUILD_DATE);
    p_driver_station_lcd->PrintfLine(DriverStationLCD::kUser_Line4, " Time: %s", WS_BUILD_TIME);
    
    p_driver_station_lcd->UpdateLCD();
    
    // Show Version information in the engineering log.
    WS_LOG_ALWAYS("Version: %s, Built on: %s %s",
                  WS_VERSION,
                  WS_BUILD_DATE,
                  WS_BUILD_TIME);
                  
    //
    // WsVision spawns a new task to create the Camera object and get images.
    //  This call must be before other blocking calls such as the gyro create &
    //  wait call (WsInputFacade)
    //
    // WsVision::instance();
    
    // Create Facades & containers
    WsConfigFacade::instance();
    WsInputFacade::instance();  // @TODO: Blocks until all input sources are created (gyro)
    WsOutputFacade::instance();
    
    // Finally, create all subsystems
    WsSubsystemContainer::instance();
    
    // Lastly, create a place to store human-feedback data
    WsDataIndicationFacade::instance();
    
    // Instantiate Autonomous Program Manager
    WsAutonomousProgramManager::instance();
    
    WS_LOG_TRACE("End");
}

WsRobot::~WsRobot(void)
{
    WS_LOG_TRACE("Begin");
    WS_LOG_ALWAYS("Robot Destructor!!");
    
    //
    // @TODO
    //  This should never get called, but if it does, clean up!
    //
    
    WS_LOG_TRACE("End");
}

void
WsRobot::RobotInit(void)
{
    WS_LOG_TRACE("Begin");
    WS_LOG_ALWAYS("Robot Init!!");
    
    // Let's slow down now for testing...
    this->SetPeriod(0.02);
    
    // Use the watchdog, if our task runs away for 1 second, we're dead!
    this->GetWatchdog().SetExpiration(1.0);
    this->GetWatchdog().SetEnabled(true);
    
    WsAutonomousProgramManager::instance()->clear();
    WS_LOG_TRACE("End");
}

void
WsRobot::DisabledInit(void)
{
    WS_LOG_TRACE("Begin");
    WS_LOG_ALWAYS("Robot Disabled!!");
    
    // Create smart led, if not already created...
    //  We are doing this here to delay data sending during bootup
    WsSmartLed::instance();
    
    //
    // Re-read configuration data from the config file
    //  This call will cause any interested subsystem to re-load its config
    //  data such as PID constants, desired set points, etc.
    //
    WsConfigFacade::instance()->updateCache();
    
    WsAutonomousProgramManager::instance()->clear();
    
    //
    // Autonomous requires input acceleration to be disabled.
    //  We re-enable it here for the driver
    //
    bool is_acceleration_enabled = false;
    WsDataIndicationFacade::instance()->getDriveBaseAccelerationEnabled(is_acceleration_enabled);
    if (false == is_acceleration_enabled)
    {
        bool use_accel_toggle = false;
        WsInputFacade::instance()->setDriveBaseAccelerationButtonState(use_accel_toggle);
        use_accel_toggle = true;
        WsInputFacade::instance()->setDriveBaseAccelerationButtonState(use_accel_toggle);
    }
    
    // @TODO: Make sure that we clear out the old autonomous program
    
    WS_LOG_TRACE("End");
}

void
WsRobot::AutonomousInit(void)
{
    WS_LOG_TRACE("Begin");
    WS_LOG_ALWAYS("Autonomous!!");
    
    //
    // @TODO:  Remove this code later...
    //  We should start the auto-program only after the configured delay has
    //  passed.  We cannot wait in AutonomousInit() for that to happen...
    //  Eventually, we will simply check a timer to see if the configured
    //  delay period has elapsed.
    //
    
    //@TODO: Start the autonomous program
    
    // Autonomous requires input acceleration to be disabled.
    bool is_acceleration_enabled = false;
    WsDataIndicationFacade::instance()->getDriveBaseAccelerationEnabled(is_acceleration_enabled);
    if (true == is_acceleration_enabled)
    {
        bool use_accel_toggle = false;
        WsInputFacade::instance()->setDriveBaseAccelerationButtonState(use_accel_toggle);
        use_accel_toggle = true;
        WsInputFacade::instance()->setDriveBaseAccelerationButtonState(use_accel_toggle);
    }
    
    WS_LOG_TRACE("End");
}

void
WsRobot::TeleopInit(void)
{
    WS_LOG_TRACE("Begin");
    WS_LOG_ALWAYS("Teleop!!");
    
    //@TODO: Remove the previous autonomous program so that there's no leftover data
    WsAutonomousProgramManager::instance()->clear();
    
    WS_LOG_TRACE("End");
}

void
WsRobot::DisabledPeriodic(void)
{
    // WS_LOG_TRACE("Begin");
    
    // Refresh the inputs
    WsInputFacade::instance()->updateOiData();
    WsInputFacade::instance()->updateSensorData();
    
    // Pushing data so the dashboard can read it
    WsTypes::WsAutonomousProgramIdType floating_id = WsTypes::WS_AUTO_PROGRAM_SLEEPER;
    floating_id = WsAutonomousProgramManager::instance()->getSelectedProgramId();
    WsInputFacade::instance()->setAutonomousProgramId(floating_id);
    
    WsTypes::WsAutonomousProgramIdType locked_id = WsTypes::WS_AUTO_PROGRAM_SLEEPER;
    locked_id = WsAutonomousProgramManager::instance()->getLockedProgramId();
    WsDataIndicationFacade::instance()->setLockedAutoProgramId(locked_id);
    
    // This updates the dashboard - we don't want all subsystems updating
    WsSubsystem* p_ss =
        WsSubsystemContainer::instance()->getSubsystemById(WsSubsystemContainer::WS_SS_DASHBOARD_UPDATER_ID);
    p_ss->update();
    
    this->GetWatchdog().Feed();
    //WS_LOG_TRACE("End");
}

void
WsRobot::AutonomousPeriodic(void)
{
    WS_LOG_TRACE("Begin");
    
    // Refresh the inputs
    WsInputFacade::instance()->updateSensorData();
    
    // Call all current Programs
    WsAutonomousProgramManager::instance()->run();
    
    // Update all the subsystems from the inputs
    WsSubsystemContainer::instance()->update();
    
    // Flush all the outputs
    WsOutputFacade::instance()->update();
    
    // Make sure to feed the watchdog
    this->GetWatchdog().Feed();
    
    WS_LOG_TRACE("End");
}

void
WsRobot::TeleopPeriodic(void)
{
    WS_LOG_TRACE("Begin");
    
    // Refresh the inputs
    WsInputFacade::instance()->updateOiData();
    WsInputFacade::instance()->updateSensorData();
    
    // Update all the subsystems from the inputs
    WsSubsystemContainer::instance()->update();
    
    // Flush all the outputs
    WsOutputFacade::instance()->update();
    
    // Make sure to feed the watchdog
    this->GetWatchdog().Feed();
    
    WS_LOG_TRACE("End");
}

void
WsRobot::DisabledContinuous(void)
{
    taskDelay(5);
}

void
WsRobot::AutonomousContinuous(void)
{
    taskDelay(5);
}

void
WsRobot::TeleopContinuous(void)
{
    taskDelay(5);
}

START_ROBOT_CLASS(WsRobot);

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

