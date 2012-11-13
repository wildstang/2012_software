#include "WsFlywheel.hh"
#include "WsLogger.hh"
#include "WsConfigFacade.hh"
#include "WsOutputFacade.hh"
#include "WsInputFacade.hh"
#include "WsDataIndicationFacade.hh"
#include "WsPidController.hh"
#include "WsFlywheelSpeedPidOutput.hh"
#include "WsFlywheelSpeedPidInput.hh"
#include "WsFlywheelObservers.hh"

#include "Timer.h"
#include <math.h>

// float WsFlywheel::s_defaultFlywheelSetpoint = 0.0f;

WsFlywheel::WsFlywheel(void)
    : WsSubsystem("WsFlywheel")
    , a_floodLightState(false)
    , ap_flywheelSpeedPidController(NULL)
    , ap_flywheelSpeedPidInput(NULL)
    , ap_flywheelSpeedPidOutput(NULL)
    , a_flywheelControlType(WsTypes::WS_FLYWHEEL_CTRL_OFF)
    , a_flywheelManualPotValue(0.0f)
    , a_lastMappedMotorOutput(0.0f)
    , a_flywheelLocationSpeedMapping()
    , a_newImageReady(false)
    , a_lastImageHadValidDistance(false)
    , a_lastKnownDistanceToTarget(0.0f)
    , a_presetConfig()
    , ap_currentSetpoint(NULL)
    , a_rawRequestedPreset(WsTypes::WS_SHOOTER_PRESET_NONE)
    , a_defaultFlywheelSetpoint(0.0f)
    , ap_stateSwitchObserver(NULL)
    , ap_manualPotObserver(NULL)
    , ap_floodLightStateObserver(NULL)
    , ap_freshCameraImageObserver(NULL)
    , ap_flywheelPidConfigObserver(NULL)
    , ap_flywheelLocationSpeedMappingObserver(NULL)
    , ap_requestedPresetObserver(NULL)
    , ap_presetConfigObserver(NULL)
    , ap_presetCompensationObserver(NULL)
    , ap_flywheelCoarseSetupTimer(NULL)
{
    WS_LOG_TRACE("Begin");
    
    // Create sources and outputs and store them
    ap_flywheelSpeedPidInput = new WsFlywheelSpeedPidInput();
    ap_flywheelSpeedPidOutput = new WsFlywheelSpeedPidOutput();
    
    // Default to a known state for presets
    ::memset(&a_presetConfig, 0x00, sizeof(a_presetConfig));
    
    // These constants are not right...
    ap_flywheelSpeedPidController =
        new WsPidController(ap_flywheelSpeedPidInput,   // Read process variable from here
                            ap_flywheelSpeedPidOutput,  // Write new outputs here
                            "ap_flywheelSpeed");
    ap_flywheelSpeedPidController->Reset();
    ap_flywheelSpeedPidController->Disable();
    
    // Create our stabilization timer
    ap_flywheelCoarseSetupTimer = new Timer();
    ap_flywheelCoarseSetupTimer->Stop();
    ap_flywheelCoarseSetupTimer->Reset();
    
    ap_currentSetpoint = &a_defaultFlywheelSetpoint;
    
    //
    // Create all of our observers
    //  These must be the last objects created here!!
    //
    ap_stateSwitchObserver = new WsFlywheelStatePotValueObserver(this);
    ap_manualPotObserver = new WsFlywheelManualPotObserver(this);
    ap_floodLightStateObserver = new WsFlywheelFloodLightStateObserver(this);
    ap_freshCameraImageObserver = new WsFlywheelFreshImageObserver(this);
    ap_flywheelPidConfigObserver = new WsFlywheelPidConfigObserver(this);
    ap_flywheelLocationSpeedMappingObserver = new WsFlywheelLocationSpeedMappingObserver(this);
    ap_requestedPresetObserver = new WsFlywheelRequestedPresetObserver(this);
    ap_presetConfigObserver = new WsFlywheelPresetConfigObserver(this);
    ap_presetCompensationObserver = new WsFlywheelSpeedCompensationFactorObserver(this);
    
    WS_LOG_TRACE("End");
}

WsFlywheel::~WsFlywheel(void)
{
    WS_LOG_TRACE("Begin");
    
    delete ap_flywheelSpeedPidController;  ap_flywheelSpeedPidController = NULL;
    delete ap_flywheelSpeedPidInput;       ap_flywheelSpeedPidInput = NULL;
    delete ap_flywheelSpeedPidOutput;      ap_flywheelSpeedPidOutput = NULL;
    delete ap_stateSwitchObserver;         ap_stateSwitchObserver = NULL;
    delete ap_manualPotObserver;           ap_manualPotObserver = NULL;
    delete ap_floodLightStateObserver;     ap_floodLightStateObserver = NULL;
    delete ap_freshCameraImageObserver;    ap_freshCameraImageObserver = NULL;
    delete ap_flywheelPidConfigObserver;   ap_flywheelPidConfigObserver = NULL;
    delete ap_flywheelLocationSpeedMappingObserver;  ap_flywheelLocationSpeedMappingObserver = NULL;
    delete ap_presetConfigObserver;                  ap_presetConfigObserver = NULL;
    delete ap_flywheelCoarseSetupTimer;              ap_flywheelCoarseSetupTimer = NULL;
    delete ap_presetCompensationObserver;  ap_presetCompensationObserver = NULL;
    
    WS_LOG_TRACE("End");
}

void
WsFlywheel::update(void)
{
    switch (a_flywheelControlType)
    {
        case WsTypes::WS_FLYWHEEL_CTRL_OFF:
        {
            // This is the easy case, turn off the flywheel
            float flywheel_motor_off = 0.0f;
            WsOutputFacade::instance()->setFlywheelMotorSpeed(flywheel_motor_off);
            
            // Make sure the PID controller is disabled
            ap_flywheelSpeedPidController->Disable();
        } break;
        
        case WsTypes::WS_FLYWHEEL_CTRL_PRESET:
        {
            //
            // Simply allow the PID to update...
            //  The PID configuration is changed in the acceptNewRequestedPreset() method
            //
            this->processPresets(a_rawRequestedPreset);
            ap_flywheelSpeedPidController->setSetPoint(*ap_currentSetpoint);
            ap_flywheelSpeedPidController->Enable();
            ap_flywheelSpeedPidController->calcPid();
        } break;
        
        case WsTypes::WS_FLYWHEEL_CTRL_MANUAL:
        {
            // Make sure the PID controller is disabled
            ap_flywheelSpeedPidController->Disable();
            
            
            //
            // Constant really should be 0.30303030303
            // Generously chosen constant, so there is a little bit of a dead zone on the
            // upper range of the pot.
            //
            float flywheel_motor_value = a_flywheelManualPotValue * 0.305f;
            
            // Clip the value to an upper limit
            flywheel_motor_value = (flywheel_motor_value >= 1.0f)
                                   ? 1.0f
                                   : flywheel_motor_value;
                                   
            // Invert value of manual output to match electrical config
            flywheel_motor_value *= -1.0f;
            
            // Finally, update the motor output
            WsOutputFacade::instance()->setFlywheelMotorSpeed(flywheel_motor_value);
        } break;
        
        default:
        {
            // Invalid control type, default it to OFF
            WS_LOG_ERROR("Invalid Flywheel Control Type:  %d", a_flywheelControlType);
            a_flywheelControlType = WsTypes::WS_FLYWHEEL_CTRL_OFF;
        } break;
    }
    
    // @TODO:  Update these based on PID state (onTarget, isStabilized)
    float setpoint      = 0.0;  ap_flywheelSpeedPidController->getSetPoint(setpoint);
    bool is_stable      = ap_flywheelSpeedPidController->isStabilized();
    bool is_near_target = ((false == is_stable) && ap_flywheelSpeedPidController->isOnTarget());
    bool is_enabled     = ap_flywheelSpeedPidController->isEnabled();
    WsDataIndicationFacade::instance()->setFlywheelPidEnabled(is_enabled);
    WsDataIndicationFacade::instance()->setFlywheelPidSetpoint(setpoint);
    WsDataIndicationFacade::instance()->setFlywheelNearTargetSpeedFlag(is_near_target);
    WsDataIndicationFacade::instance()->setFlywheelAtTargetSpeedFlag(is_stable);
}

float
WsFlywheel::interpolateFlywheelSpeed(const float& rc_distanceToBottom)
{
    float x_a = 0.0f;
    float x_b = 0.0f;
    float y_a = 0.0f;
    float y_b = 0.0f;
    float speed = 0.0f;
    bool  speed_known = false;
    
    if (rc_distanceToBottom < a_flywheelLocationSpeedMapping.fender_distance)
    {
        // Fender!
        //  Actual distance is 45 inches
        speed = a_flywheelLocationSpeedMapping.fender_fws;
        speed_known = true;
    }
    else if ((rc_distanceToBottom > a_flywheelLocationSpeedMapping.fender_distance) &&
             (rc_distanceToBottom < a_flywheelLocationSpeedMapping.key_bottom_distance))
    {
        // Between Fender and Bottom of Key!
        x_a = a_flywheelLocationSpeedMapping.fender_distance;
        x_b = a_flywheelLocationSpeedMapping.key_bottom_distance;
        y_a = a_flywheelLocationSpeedMapping.fender_fws;
        y_b = a_flywheelLocationSpeedMapping.key_bottom_fws;
    }
    else if ((rc_distanceToBottom > a_flywheelLocationSpeedMapping.key_bottom_distance) &&
             (rc_distanceToBottom < a_flywheelLocationSpeedMapping.key_middle_distance))
    {
        // Between Bottom and Middle of Key!
        x_a = a_flywheelLocationSpeedMapping.key_bottom_distance;
        x_b = a_flywheelLocationSpeedMapping.key_middle_distance;
        y_a = a_flywheelLocationSpeedMapping.key_bottom_fws;
        y_b = a_flywheelLocationSpeedMapping.key_middle_fws;
    }
    else if ((rc_distanceToBottom > a_flywheelLocationSpeedMapping.key_middle_distance) &&
             (rc_distanceToBottom < a_flywheelLocationSpeedMapping.key_top_distance))
    {
        // Between Middle and Top of Key!
        x_a = a_flywheelLocationSpeedMapping.key_middle_distance;
        x_b = a_flywheelLocationSpeedMapping.key_top_distance;
        y_a = a_flywheelLocationSpeedMapping.key_middle_fws;
        y_b = a_flywheelLocationSpeedMapping.key_top_fws;
    }
    else // (rc_distanceToBottom > a_flywheelLocationSpeedMapping.key_top_distance)
    {
        // Beyond Top of Key!
        speed = a_flywheelLocationSpeedMapping.key_top_fws;
        speed_known = true;
    }
    
    // Interpolate if we need to (speed not yet decided)
    if (false == speed_known)
    {
        speed = (float)y_a +
                (((float)(y_b - y_a) / (float)(x_b - x_a)) * (rc_distanceToBottom - x_a));
    }
    
    WS_LOG_NOTICE("Xa=%.05f, Xb=%.05f, Ya=%.05f, Yb=%.05f, DTB=%.05f, SK=%d, speed=%.05f",
                  x_a, x_b,
                  y_a, y_b,
                  rc_distanceToBottom,
                  speed_known,
                  speed);
                  
    return speed;
}

float
WsFlywheel::interpolateFlywheelSpeedControllerSpeed(const float& rc_distanceToBottom)
{
    float x_a = 0.0f;
    float x_b = 0.0f;
    float y_a = 0.0f;
    float y_b = 0.0f;
    float speed = 0.0f;
    bool  speed_known = false;
    
    if (rc_distanceToBottom < a_flywheelLocationSpeedMapping.fender_distance)
    {
        // Fender!
        //  Actual distance is 45 inches
        speed = a_flywheelLocationSpeedMapping.fender_speed;
        speed_known = true;
    }
    else if ((rc_distanceToBottom > a_flywheelLocationSpeedMapping.fender_distance) &&
             (rc_distanceToBottom < a_flywheelLocationSpeedMapping.key_bottom_distance))
    {
        // Between Fender and Bottom of Key!
        x_a = a_flywheelLocationSpeedMapping.fender_distance;
        x_b = a_flywheelLocationSpeedMapping.key_bottom_distance;
        y_a = a_flywheelLocationSpeedMapping.fender_speed;
        y_b = a_flywheelLocationSpeedMapping.key_bottom_speed;
    }
    else if ((rc_distanceToBottom > a_flywheelLocationSpeedMapping.key_bottom_distance) &&
             (rc_distanceToBottom < a_flywheelLocationSpeedMapping.key_middle_distance))
    {
        // Between Bottom and Middle of Key!
        x_a = a_flywheelLocationSpeedMapping.key_bottom_distance;
        x_b = a_flywheelLocationSpeedMapping.key_middle_distance;
        y_a = a_flywheelLocationSpeedMapping.key_bottom_speed;
        y_b = a_flywheelLocationSpeedMapping.key_middle_speed;
    }
    else if ((rc_distanceToBottom > a_flywheelLocationSpeedMapping.key_middle_distance) &&
             (rc_distanceToBottom < a_flywheelLocationSpeedMapping.key_top_distance))
    {
        // Between Middle and Top of Key!
        x_a = a_flywheelLocationSpeedMapping.key_middle_distance;
        x_b = a_flywheelLocationSpeedMapping.key_top_distance;
        y_a = a_flywheelLocationSpeedMapping.key_middle_speed;
        y_b = a_flywheelLocationSpeedMapping.key_top_speed;
    }
    else // (rc_distanceToBottom > a_flywheelLocationSpeedMapping.key_top_distance)
    {
        // Beyond Top of Key!
        speed = a_flywheelLocationSpeedMapping.key_top_speed;
        speed_known = true;
    }
    
    // Interpolate if we need to (speed not yet decided)
    if (false == speed_known)
    {
        speed = (float)y_a +
                (((float)(y_b - y_a) / (float)(x_b - x_a)) * (rc_distanceToBottom - x_a));
    }
    
    WS_LOG_NOTICE("Xa=%.05f, Xb=%.05f, Ya=%.05f, Yb=%.05f, DTB=%.05f, SK=%d, speed=%.05f",
                  x_a, x_b,
                  y_a, y_b,
                  rc_distanceToBottom,
                  speed_known,
                  speed);
                  
    return speed;
}

bool
WsFlywheel::calculateDistanceFromImage(float& r_distance)
{
    bool found_distance_flag = false;
    const int FOV_width_px = 320;
    const int FOV_height_px = 240;
    const float FOV_width_div_height = ((float)FOV_width_px / (float)FOV_height_px);
    const int target_height_inches = 18;
    const int target_width_inches = 24;
    const int target_area = target_height_inches * target_width_inches;
    const int areas_multiplied = (target_area * FOV_width_px * FOV_height_px);
    const int height_from_middle_to_bottom_target_squared = 1089;
    const float error_equation_slope = -0.04855842f;
    const float error_equation_offset = 14.7376703f;
    const float min_bottom_compactness_threshold = 0.935f;
    const float min_middle_compactness_threshold = 0.900f;
    //    const int height_from_bottom_to_top_target_squared = 4900;
    
    // This is the camera view angle divided by two and lowered experimentally
    // const float theta = 24.5f;
    const double tan_theta = 0.455726255532585f;
    
    // Get the bottom target image data from the input facade
    WsTypes::WsTargetBasketIdxType img_idx = WsTypes::WS_BOTTOM_BASKET_INDEX;
    WsTypes::WsTargetImageInfoType img_info_bottom;
    WsInputFacade::instance()->getTargetImageInfo(img_idx, img_info_bottom);
    
    // Is the target visible and not obstructed (must fairly compact!)
    float compactness_factor = img_info_bottom.compactness;
    if ((true == img_info_bottom.has_target) && (compactness_factor > min_bottom_compactness_threshold))
    {
        int particle_area = img_info_bottom.target_area;
        
        //=(G4*E4*F4)/C4
        float area_FOV_inches = ((float)areas_multiplied / (float)particle_area);
        
        //=SQRT(H4*(E4/F4))
        float FOV_width_inches = sqrt((float)area_FOV_inches * (FOV_width_div_height));
        
        float w = (FOV_width_inches / 2.0);
        //Distance is to bottom target
        double distance_to_bottom_target = (w / tan_theta) ;
        
        //
        // double distance_to_top_target =
        //     sqrt((float)(distance_to_bottom_target * distance_to_bottom_target) +
        //          (float)(height_from_bottom_to_top_target_squared));
        //
        
        
        // Give the result back to the caller
        r_distance = (float)distance_to_bottom_target;
        WS_LOG_NOTICE("Bottom Target, calculated distance= %f , compactness %f", r_distance , compactness_factor);
        found_distance_flag = true;
    }
    
    
    // Get the left target image data from the input facade
    WsTypes::WsTargetBasketIdxType img_idx_left = WsTypes::WS_LEFT_BASKET_INDEX;
    WsTypes::WsTargetImageInfoType img_info_left;
    WsInputFacade::instance()->getTargetImageInfo(img_idx_left, img_info_left);
    
    // Get the right target image data from the input facade
    WsTypes::WsTargetBasketIdxType img_idx_right = WsTypes::WS_RIGHT_BASKET_INDEX;
    WsTypes::WsTargetImageInfoType img_info_right;
    WsInputFacade::instance()->getTargetImageInfo(img_idx_right, img_info_right);
    
    float compactness_left = img_info_left.compactness;
    float compactness_right = img_info_right.compactness;
    if ((true == img_info_left.has_target) && (true == img_info_right.has_target) && (compactness_left > min_middle_compactness_threshold) && (compactness_right >  min_middle_compactness_threshold))
    {
        int left_particle_area = img_info_left.target_area;
        int right_particle_area = img_info_right.target_area;
        float avg_particle_area = (float)(right_particle_area + left_particle_area) / 2.0;
        
        //=(G4*E4*F4)/C4
        float area_FOV_inches = ((float)areas_multiplied / (float)avg_particle_area);
        
        //=SQRT(H4*(E4/F4))
        float FOV_width_inches = sqrt((float)area_FOV_inches * (FOV_width_div_height));
        
        float w = (FOV_width_inches / 2.0);
        //Distance is to midpint of middle targets
        double distance_to_middle_target_midpoint = (w / tan_theta) ;
        
        //
        double distance_to_wall =
            sqrt((float)(distance_to_middle_target_midpoint * distance_to_middle_target_midpoint) -
                 (float)(height_from_middle_to_bottom_target_squared));
        //
        
        //Rim obscures image enough to make distances appear farther away
        //Linear error equation reins the value back in to something close to the other measurement
        float calculated_error = (error_equation_slope * distance_to_wall) + error_equation_offset;
        
        double actual_distance_to_wall = distance_to_wall - calculated_error;
        // Give the result back to the caller
        WS_LOG_NOTICE("Left/Right Target, calculated distance= %f , error %f , actual distance %f", distance_to_wall, calculated_error, actual_distance_to_wall);
        //only use left/right target calculation if the bottom target failed
        if (false == found_distance_flag)
        {
            r_distance = (float)actual_distance_to_wall;
            found_distance_flag = true;
        }
    }
    
    return found_distance_flag;
}

void
WsFlywheel::acceptNewFloodlightState(const bool& rc_state)
{
    a_floodLightState = rc_state;
    WsOutputFacade::instance()->setFloodLightState(a_floodLightState);
}

void
WsFlywheel::acceptNewFreshImageFlag(const bool& rc_flag)
{
    //
    // NOTE:
    //  At the time this code was written, the WsVision class (running in a
    //  separate task) is pushing data into the WsInputFacade.  This causes
    //  the interested observers to fire in the context of the WsVision class.
    //  Since this is a likely point for a context switch, we should minimize
    //  the time spent in this function.
    //
    //  It may be tempting to do new image processing code here.
    //  RESIST THIS TEMPATION.  Do the image processing in the ::update() loop.
    //
    WS_LOG_DEBUG("State Change: New = %d", rc_flag);
    
    // Only update this on false --> true transitions
    if (true == rc_flag)
    {
        a_newImageReady = rc_flag;
    }
}

void
WsFlywheel::acceptNewFlywheelStatePotValue(const float& rc_newValue)
{
    //
    // There is a 3-position switch on the OI that determines the type of control
    //  we use to manipulate the flywheel speed.  Those three positions are defined
    //  as follows:
    //  UP:      Manual control, the pot directly controls motor output power
    //  MIDDLE:  Preset control, preset selections will be acknowledged
    //  DOWN:    Off, the flywheel will be turned off
    //
    if ((rc_newValue >= 0.0f) &&
        (rc_newValue <  1.10f))
    {
        a_flywheelControlType = WsTypes::WS_FLYWHEEL_CTRL_OFF;
    }
    else if ((rc_newValue >= 1.10f) &&
             (rc_newValue <  2.20f))
    {
        a_flywheelControlType = WsTypes::WS_FLYWHEEL_CTRL_PRESET;
    }
    else if ((rc_newValue >= 2.20f) &&
             (rc_newValue <  3.30f))
    {
        a_flywheelControlType = WsTypes::WS_FLYWHEEL_CTRL_MANUAL;
    }
    else
    {
        // Oops... default to off
        WS_LOG_ERROR("Flywheel Control Pot Value Out of Range: %f", rc_newValue);
        a_flywheelControlType = WsTypes::WS_FLYWHEEL_CTRL_OFF;
    }
}

void
WsFlywheel::acceptNewFlywheelManualPotValue(const float& rc_newValue)
{
    a_flywheelManualPotValue = rc_newValue;
}

void
WsFlywheel::acceptNewPidConfig(const WsTypes::WsPidConfigurationDataT& rc_new_data)
{
    bool is_enabled = ap_flywheelSpeedPidController->isEnabled();
    
    // Disable Pid Controller if needed
    if (true == is_enabled)
    {
        ap_flywheelSpeedPidController->Disable();
    }
    
    // Push in new config
    ap_flywheelSpeedPidController->setConstants(rc_new_data.k_p, rc_new_data.k_i, rc_new_data.k_d);
    ap_flywheelSpeedPidController->setStaticEpsilon(rc_new_data.static_epsilon);
    ap_flywheelSpeedPidController->setErrorEpsilon(rc_new_data.error_epsilon);
    ap_flywheelSpeedPidController->setErrorIncrementPercentage(rc_new_data.error_increment);
    ap_flywheelSpeedPidController->setMaxIntegral(rc_new_data.max_integral);
    ap_flywheelSpeedPidController->setIntegralErrorThresh(rc_new_data.integral_error_thresh);
    ap_flywheelSpeedPidController->setDifferentiatorBandLimit(rc_new_data.differentiator_band_limit);
    ap_flywheelSpeedPidController->setMinMaxOutput(rc_new_data.min_output, rc_new_data.max_output);
    ap_flywheelSpeedPidController->setMinMaxInput(rc_new_data.min_input, rc_new_data.max_input);
    ap_flywheelSpeedPidController->setMinStabilizationTime(rc_new_data.min_on_target_time);
    
    // Re-enable Pid Controller if needed
    if (true == is_enabled)
    {
        ap_flywheelSpeedPidController->Enable();
    }
}

void
WsFlywheel::acceptNewLocationSpeedMapping(const WsTypes::WsLocationSpeedMappingType& rc_newMapping)
{
    a_flywheelLocationSpeedMapping = rc_newMapping;
}

void
WsFlywheel::acceptNewRequestedPreset(const WsTypes::WsShooterPresetType& rc_newValue)
{
    a_rawRequestedPreset = rc_newValue;
}

void
WsFlywheel::processPresets(const WsTypes::WsShooterPresetType& rc_newValue)
{
    if (WsTypes::WS_SHOOTER_PRESET_NONE != rc_newValue)
    {
        //
        // When presets change, the current value of the PID output should
        //  be updated to the starting "target" position.  This data is determined
        //  empirically.  It may be useful to put this data in the config file.
        //
        ap_currentSetpoint           = &a_presetConfig.flywheel_auton_speed;
        float pid_output_base_value = a_presetConfig.flywheel_auton_motor_val;
        switch (rc_newValue)
        {
            case WsTypes::WS_SHOOTER_PRESET_KEY_FORWARD:
            {
                ap_currentSetpoint = &a_presetConfig.flywheel_key_forward_speed;
                pid_output_base_value = a_presetConfig.flywheel_key_forward_motor_val;
            } break;
            
            case WsTypes::WS_SHOOTER_PRESET_KEY_BACK:
            {
                ap_currentSetpoint = &a_presetConfig.flywheel_key_reverse_speed;
                pid_output_base_value = a_presetConfig.flywheel_key_reverse_motor_val;
            } break;
            
            case WsTypes::WS_SHOOTER_PRESET_FENDER_LEFT:
            {
                ap_currentSetpoint = &a_presetConfig.flywheel_fender_left_speed;
                pid_output_base_value = a_presetConfig.flywheel_fender_left_motor_val;
            } break;
            
            case WsTypes::WS_SHOOTER_PRESET_FENDER_RIGHT:
            {
                ap_currentSetpoint = &a_presetConfig.flywheel_fender_right_speed;
                pid_output_base_value = a_presetConfig.flywheel_fender_right_motor_val;
            } break;
            
            case WsTypes::WS_SHOOTER_PRESET_AUTON:
            {
                ap_currentSetpoint = &a_presetConfig.flywheel_auton_speed;
                pid_output_base_value = a_presetConfig.flywheel_auton_motor_val;
            } break;
            
            default:
            {
                // This shouldn't happen, get back to a good state if it does
                WS_LOG_ERROR("Invalid Requested Preset: %d", rc_newValue);
                a_currentRequestedPreset = WsTypes::WS_SHOOTER_PRESET_NONE;
                a_flywheelControlType = WsTypes::WS_FLYWHEEL_CTRL_OFF;
                ap_flywheelSpeedPidController->Disable();
                
                // We don't want to do anything else in this case, bail out now
                return;
            }
        }
        
        // Update the new controlled speed
        ap_flywheelSpeedPidOutput->overwriteCumulativeOutput(pid_output_base_value);
        ap_flywheelSpeedPidController->Enable();
        
        // Capture the new requested preset
        a_currentRequestedPreset = rc_newValue;
    }
    else
    {
        //
        // Leave the pre-set alone.  We only change the current preset position
        //  if the user changes the requested position.  In this case, the user
        //  selected a preset last time, and this time he let the button go.
        //
    }
}

void
WsFlywheel::acceptNewPresetConfig(const WsTypes::WsShooterPresetConfigT& rc_newConfig)
{
    a_presetConfig = rc_newConfig;
}

void
WsFlywheel::acceptNewCompensationFactor(const float& r_newValue)
{
    //
    // The value passed into this method is a scaling factor for the flywheel
    //  target RPM.  This value is allowed to range from 80% to 120% of the
    //  pre-configured values.
    //
    if ((r_newValue >= 0.80f) &&
        (r_newValue <= 1.20f))
    {
        float percent_change = r_newValue;
        //
        // Re-read the preset values from the config facade... these are the default
        //  values that we have on startup, as defined in the config file.
        //
        WsConfigFacade::instance()->getShooterPresetConfig(a_presetConfig);
        
        //
        // Scale the raw motor output and flywheel speeds (RPM) according to the
        //  compensation factor we just got.
        //
        a_presetConfig.flywheel_key_forward_speed *= percent_change;
        a_presetConfig.flywheel_key_forward_motor_val *= percent_change;
        a_presetConfig.flywheel_key_reverse_speed *= percent_change;
        a_presetConfig.flywheel_key_reverse_motor_val *= percent_change;
        a_presetConfig.flywheel_fender_left_speed *= percent_change;
        a_presetConfig.flywheel_fender_left_motor_val *= percent_change;
        a_presetConfig.flywheel_fender_right_speed *= percent_change;
        a_presetConfig.flywheel_fender_right_motor_val *= percent_change;
    }
    else
    {
        WS_LOG_ERROR("Invalid Flywheel Compensation Factor Found: %f", r_newValue);
    }
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

