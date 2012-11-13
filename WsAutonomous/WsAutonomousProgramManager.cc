#include "WsAutonomousProgram.hh"
#include "WsAutonomousProgramManager.hh"
#include "WsLogger.hh"
#include "Timer.h"
#include "WsAutonomousProgramManagerObservers.hh"
#include "WsInputFacade.hh"
#include "WsTypes.hh"

//Autonomous Programs
#include "WsAutoSleeperProgram.hh"
#include "WsVictorLinearizerProgram.hh"
#include "WsFlywheelLinearizerProgram.hh"
#include "WsAutoVictorCalibrationProgram.hh"
#include "WsAutoShootProgram.hh"
#include "WsAutoShoot3Program.hh"
#include "WsAutoDriveProgram.hh"
#include "WsAutoShootTipProgram.hh"
#include "WsAutoTipShootProgram.hh"
#include "WsAutoDriveProgram.hh"
#include "WsAutoFenderRightProgram.hh"
#include "WsAutoFenderLeftProgram.hh"

#define POT_MAX 3.3f
#define NUM_PROGRAMS WsTypes::WS_AUTO_PROGRAM_NUM
#define POT_PROGRAM_LENGTH ( POT_MAX / NUM_PROGRAMS )

// Initialize static data
WsAutonomousProgramManager* WsAutonomousProgramManager::asp_instance = NULL;

// Defines for delay
static const float AUTO_PRGM_DELAY_NONE = 0.0f;

WsAutonomousProgramManager::WsAutonomousProgramManager(void)
    : ap_latchedProgram(NULL)
    , a_latchedProgramId(WsTypes::WS_AUTO_PROGRAM_SLEEPER)
    , a_lockedDelay(0.0)
    , a_lockInState(false)
    , ap_autonomousDelayTimer(NULL)
    , a_currentState(WS_AUTO_PROGRAM_STATE_INIT)
    , ap_lockInSwitchObserver(NULL)
    , ap_programSelectorPotValueObserver(NULL)
{
    WS_LOG_TRACE("Begin");
    
    ::memset(&a_autonomousProgramContainer, 0x00, sizeof(a_autonomousProgramContainer));
    
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_SLEEPER ] = new WsAutoSleeperProgram();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_VICTOR_CALIBRATION ] = new WsAutoVictorCalibrationProgram();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_FLYWHEEL_LINEARIZER ] = new WsFlywheelLinearizerProgram();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_VICTOR_LINEARIZER ] = new WsVictorLinearizerProgram();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_SHOOT_6 ] = new WsAutoShootProgram();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_SHOOT_3 ] = new WsAutoShoot3Program();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_EMPTY ] = new WsAutoSleeperProgram();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_DRIVE] = new WsAutoDriveProgram();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_FENDER_LEFT] = new WsAutoFenderLeftProgram();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_FENDER_RIGHT] = new WsAutoFenderRightProgram();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_SHOOT_TIP] = new WsAutoShootTipProgram();
    a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_TIP_SHOOT ] = new WsAutoTipShootProgram();
    ap_latchedProgram = a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_SLEEPER ];
    
    ap_autonomousDelayTimer = new Timer();
    ap_autonomousDelayTimer->Stop();
    ap_autonomousDelayTimer->Reset();
    
    ap_lockInSwitchObserver = new WsAutoLockInSwitchObserver(this);
    ap_programSelectorPotValueObserver = new WsAutoProgramSelectorObserver(this);
    
    WS_LOG_TRACE("End");
}

WsAutonomousProgramManager::~WsAutonomousProgramManager(void)
{
    WS_LOG_TRACE("Begin");
    int i = (int)WsTypes::WS_AUTO_PROGRAM_SLEEPER;
    
    // Free up all allocated memory
    delete ap_autonomousDelayTimer;             ap_autonomousDelayTimer = NULL;
    delete ap_lockInSwitchObserver;             ap_lockInSwitchObserver = NULL;
    delete ap_programSelectorPotValueObserver;  ap_programSelectorPotValueObserver = NULL;
    
    for (; i < WsTypes::WS_AUTO_PROGRAM_NUM; i++)
    {
        delete a_autonomousProgramContainer[ i ];
        a_autonomousProgramContainer[ i ] = NULL;
    }
    
    // Don't forget to reset asp_instance
    asp_instance = NULL;
    
    WS_LOG_TRACE("End");
}

WsAutonomousProgramManager*
WsAutonomousProgramManager::instance(void)
{
    if (NULL == asp_instance)
    { asp_instance = new WsAutonomousProgramManager(); }
    return asp_instance;
}

void
WsAutonomousProgramManager::run(void)
{
    WsAutonomousProgramStateType new_state = a_currentState;
    
    switch (a_currentState)
    {
        case WS_AUTO_PROGRAM_STATE_INIT:
        {
            //this->lockAutonomousConfiguration();
            
            ap_autonomousDelayTimer->Start();
            ap_autonomousDelayTimer->Reset();
            
            if (NULL == ap_latchedProgram)
            {
                WS_LOG_ERROR("Latched program NULL!!");
                return;
            }
            WS_LOG_ERROR("Running Autonomous Program '%s'", ap_latchedProgram->getName().c_str());
            
            // This should only run once
            new_state = WS_AUTO_PROGRAM_STATE_DELAY;
            break;
        }
        case WS_AUTO_PROGRAM_STATE_DELAY:
        {
            // Do nothing until the delay timer expires
            if (ap_autonomousDelayTimer->HasPeriodPassed(a_lockedDelay))
            {
                // Initialize the program here I guess
                if (NULL == ap_latchedProgram)
                {
                    WS_LOG_ERROR("Latched program NULL!!");
                    return;
                }
                ap_latchedProgram->init();
                
                ap_autonomousDelayTimer->Stop();
                ap_autonomousDelayTimer->Reset();
                
                new_state = WS_AUTO_PROGRAM_STATE_RUN;
            }
            break;
        }
        case WS_AUTO_PROGRAM_STATE_RUN:
        {
            if (NULL == ap_latchedProgram)
            {
                WS_LOG_ERROR("Latched program NULL!!");
                return;
            }
            ap_latchedProgram->run();
            
            if (ap_latchedProgram->isDone())
            {
                // If we're done we're done. Simple.
                new_state = WS_AUTO_PROGRAM_STATE_DONE;
            }
            break;
        }
        case WS_AUTO_PROGRAM_STATE_DONE:
        {
            //Dead state. We're done.
            break;
        }
        case WS_AUTO_PROGRAM_STATE_NUM:
        case WS_AUTO_PROGRAM_STATE_NONE:
        default:
        {
            WS_LOG_ERROR("BAD AUTONOMOUS PROGRAM STATE!");
            
            new_state = WS_AUTO_PROGRAM_STATE_DONE;
            break;
        }
    }
    
    a_currentState = new_state;
}

void
WsAutonomousProgramManager::clear(void)
{
    // int i = (int)WS_AUTO_PROGRAM_SLEEPER;
    
    
    // Is this necessary?
    // for(; i < WS_AUTO_PROGRAM_NUM; i++ )
    // {
    //     a_autonomousProgramContainer[ i ]->clear();
    // }
    
    if (ap_latchedProgram != NULL)
    { ap_latchedProgram->clear(); }
    
    a_currentState = WS_AUTO_PROGRAM_STATE_INIT;
    
    ap_autonomousDelayTimer->Stop();
    ap_autonomousDelayTimer->Reset();
    
    a_lockInState = false;
    
    this->lockAutonomousConfiguration();
}

void
WsAutonomousProgramManager::lockAutonomousConfiguration(void)
{
    WS_LOG_TRACE("Begin");
    
    // Lock in the right program based on the Lock-in switch and
    // The program chooser
    float selected_delay = AUTO_PRGM_DELAY_NONE;
    WsTypes::WsAutonomousProgramIdType prgm_id = WsTypes::WS_AUTO_PROGRAM_SLEEPER;
    prgm_id = this->getSelectedProgramId();
    WsAutonomousProgram* p_selected_prgm = a_autonomousProgramContainer[ prgm_id ];
    
    WS_LOG_INFO("LockState: %d", a_lockInState);
    a_lockedDelay = AUTO_PRGM_DELAY_NONE;
    
    if (true == a_lockInState)
    {
        WS_LOG_TRACE("GOT HERE");
        // We only update the running auto-program info if the user "locked in"
        ap_latchedProgram = p_selected_prgm;
        a_latchedProgramId = prgm_id;
        
        if (NULL == ap_latchedProgram)
        {
            WS_LOG_ERROR("Latched program NULL!!");
            return;
        }
        
        a_lockedDelay = selected_delay;
    }
    else
    {
        WS_LOG_TRACE("GOT HERE");
        ap_latchedProgram = a_autonomousProgramContainer[ WsTypes::WS_AUTO_PROGRAM_SLEEPER ];
        a_latchedProgramId = WsTypes::WS_AUTO_PROGRAM_SLEEPER;
        
        if (NULL == ap_latchedProgram)
        {
            WS_LOG_ERROR("Latched program NULL!!");
            return;
        }
        
        a_lockedDelay = AUTO_PRGM_DELAY_NONE;
    }
    
    if (NULL == ap_latchedProgram)
    {
        WS_LOG_ERROR("Latched program NULL!!");
        return;
    }
    // Lock in the delay selection
    WS_LOG_ERROR("Locked Delay: %.2f", a_lockedDelay);
    WS_LOG_ERROR("Program To Run: %s " , ap_latchedProgram->getName().c_str());
    
    WS_LOG_TRACE("End");
    return;
}

WsAutonomousProgram*
WsAutonomousProgramManager::getLatchedProgram(void)
{
    if (NULL == ap_latchedProgram)
    {
        WS_LOG_ERROR("Latched program NULL!!");
    }
    return ap_latchedProgram;
}

float
WsAutonomousProgramManager::getLockedDelay(void)
{
    return a_lockedDelay;
}

void
WsAutonomousProgramManager::acceptNewLockInState(const bool& rc_newState, const bool& rc_oldState)
{
    if (a_currentState == WS_AUTO_PROGRAM_STATE_INIT)
    {
        a_lockInState = ((false == rc_oldState) && (true == rc_newState));
        this->lockAutonomousConfiguration();
    }
    else
    {
        WS_LOG_INFO("Lock-In Switch flipped while running autonomous");
    }
}

WsTypes::WsAutonomousProgramIdType
WsAutonomousProgramManager::getSelectedProgramId(void)
{
    WsTypes::WsAutonomousProgramIdType id = WsTypes::WS_AUTO_PROGRAM_SLEEPER;
    
    for (int i = 0; i < WsTypes::WS_AUTO_PROGRAM_NUM; i++)
    {
        WS_LOG_INFO("a_selector = %f, i = %d", a_selectorPot, i);
        if ((a_selectorPot >= (POT_PROGRAM_LENGTH * i)) &&
            (a_selectorPot < (POT_PROGRAM_LENGTH * (i + 1))))
        {
            WS_LOG_INFO("Found Auto Program", a_selectorPot, i);
            id = (WsTypes::WsAutonomousProgramIdType)i;
            break;
        }
    }
    
    return id;
}

WsTypes::WsAutonomousProgramIdType
WsAutonomousProgramManager::getLockedProgramId(void)
{
    return a_latchedProgramId;
}

void
WsAutonomousProgramManager::acceptNewProgramSelectorValue(const float& rc_pot)
{
    a_selectorPot = rc_pot;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

