#include "WsPidController.hh"
#include <math.h>
#include "WsLogger.hh"
#include "WsPidInput.hh"
#include "WsPidOutput.hh"
#include "WsTypes.hh"
#include "Timer.h"

//
// Initializes the WsPidController object. All parameters default to 0.
//
WsPidController::WsPidController(WsPidInput* p_source,
                                 WsPidOutput* p_output,
                                 const char* pc_pidControllerName)
    : a_p(0.0f)
    , a_i(0.0f)
    , a_d(0.0f)
    , a_currentError(0.0f)
    , a_previousError(0.0f)
    , a_setPoint(0.0f)
    , a_errorSum(0.0f)
    , a_errorIncrement(1.0f)
    , a_errorEpsilon(0.0f)
    , a_staticEpsilon(0.0f)
    , a_maxIntegral(1.0f)
    , a_integralErrorThresh(-1.0f)
    , a_differentiatorBandLimit(1.0f)
    , a_maxOutput(1.0f)
    , a_minOutput(-1.0f)
    , a_maxInput(1.0f)
    , a_minInput(-1.0f)
    , a_currentState(WS_PID_INITIALIZE_STATE)
    , a_minOnTargetTime(0.2)
    , a_allowStaticEpsilon(false)
    , ap_stabilizationTimer(NULL)
    , ap_pidSource(NULL)
    , ap_pidOutput(NULL)
    , a_enabled(false)
    , a_controllerName(pc_pidControllerName)
{
    this->setErrorIncrementPercentage(a_errorIncrement);
    ap_stabilizationTimer = new Timer();
    
    ap_pidSource = p_source;
    ap_pidOutput = p_output;
}

WsPidController::~WsPidController()
{
}

float WsPidController::getP(void)
{
    return a_p;
}

float WsPidController::getI(void)
{
    return a_i;
}

float WsPidController::getD(void)
{
    return a_d;
}

float WsPidController::getEpsilon(void)
{
    return a_errorEpsilon;
}

float WsPidController::getStaticEpsilon(void)
{
    return a_staticEpsilon;
}


/**
 * Sets the Pid constants to new values.
 */
void
WsPidController::setConstants(float p, float i, float d)
{
    a_p = p;
    a_i = i;
    a_d = d;
}

/**
 * Sets the allowable error range away from the desired value.
 */
void
WsPidController::setErrorEpsilon(float epsilon)
{
    a_errorEpsilon = epsilon;
}


void
WsPidController::setStaticEpsilon(float epsilon)
{
    a_staticEpsilon = epsilon;
}

/**
 * Sets the maximum increment to the error sum used in the I component
 * calculation.
 * This defaults to 1 in the constructor, which has worked well for 1114 the
 * past few years.
 */
void
WsPidController::setErrorIncrementPercentage(float inc)
{
    a_errorIncrement = ((a_maxOutput - a_minOutput) *
                        (inc / 100.0f));
}

void
WsPidController::setMinMaxOutput(const float& rc_min,
                                 const float& rc_max)
{
    if (rc_min < rc_max)
    {
        a_minOutput = rc_min;
        a_maxOutput = rc_max;
    }
    else
    {
        WS_LOG_ERROR("Invalid min/max Output Config Request: %.05f,%.05f", rc_min, rc_max);
        WS_LOG_NOTICE("min/max Output remain%.05f,%.05f", a_minOutput, a_maxOutput);
    }
}

void
WsPidController::setMinMaxInput(const float& rc_min,
                                const float& rc_max)
{
    if (rc_min < rc_max)
    {
        a_minInput = rc_min;
        a_maxInput = rc_max;
    }
    else
    {
        WS_LOG_ERROR("Invalid min/max Input Config Request: %.05f,%.05f", rc_min, rc_max);
        WS_LOG_NOTICE("min/max Input remain%.05f,%.05f", a_minInput, a_maxInput);
    }
}

void
WsPidController::setMinStabilizationTime(const float& rc_time)
{
    a_minOnTargetTime = rc_time;
}

void
WsPidController::setMaxIntegral(float max)
{
    a_maxIntegral = fabs(max);
}

void
WsPidController::setIntegralErrorThresh(float thresh)
{
    a_integralErrorThresh = fabs(thresh);
}

/**
 * Resets the error sum back to zero.
 */
void
WsPidController::resetErrorSum(void)
{
    a_errorSum = 0;
}

/**
 * Calculates the Pid output based on the current value.
 * Pid constants and desired value should be set before calling this
 * function.
 */
void
WsPidController::calcPid(void)
{
    WsPidStateT new_state = a_currentState;
    
    // Read the value of the process variable under control and limit it
    float current_pv = ap_pidSource->pidRead();
    current_pv = this->limitInput(current_pv);
    
    // Calculate the current error term
    a_currentError = a_setPoint - current_pv;
    
    //
    // Adjust our metrics depending on where the process variable is as compared
    //  to the set point.
    //
    switch (a_currentState)
    {
        case WS_PID_DISABLED_STATE:
        {
            // PID controller is disabled, nothing to do here...
            WS_LOG_ERROR("calcPid() called while disabled from %s", a_controllerName.c_str());
            
            // Reset everything now.
            this->Reset();
            return;
        }
        
        case WS_PID_INITIALIZE_STATE:
        {
            // Don't look at the D-term when we're just starting up
            a_previousError = a_currentError;
            
            //
            // Find the next state
            //
            if (a_currentError <= -a_errorEpsilon)
            {
                //
                // Error is negative and outside the epsilon band.
                // Negative errors mean we are above our setpoint
                //
                new_state = WS_PID_ABOVE_TARGET_STATE;
            }
            else if (a_currentError >= a_errorEpsilon)
            {
                //
                // Positive Errors mean we are below our setpoint
                //
                new_state = WS_PID_BELOW_TARGET_STATE;
            }
            else if ((a_currentError >= (-1 * a_errorEpsilon)) &&
                     (a_currentError <= a_errorEpsilon))
            {
                new_state = WS_PID_ON_TARGET_STATE;
                ap_stabilizationTimer->Reset();
                ap_stabilizationTimer->Start();
            }
            else
            {
                // You had better hope this does not happen
            }
            
            break;
        }
        
        case WS_PID_BELOW_TARGET_STATE:
        {
            //
            // In this case, we were above and we switched to below
            //
            if (a_errorSum < 0)
            {
                // If we are fighting away from the point, reset the error.
                a_errorSum = 0;
            }
            
            if (a_currentError < a_errorIncrement)
            {
                // If the error is smaller than the max increment amount, add it.
                a_errorSum += a_currentError;
            }
            else
            {
                // Otherwise, add the maximum increment per cycle.
                a_errorSum += a_errorIncrement;
            }
            if (a_currentError >= a_staticEpsilon)
            {
                a_allowStaticEpsilon = false;
            }
            
            // Find the next state
            // Error is inside the epsilon band.
            if ((a_currentError >= (-1 * a_errorEpsilon)) &&
                (a_currentError <= a_errorEpsilon))
            {
                new_state = WS_PID_ON_TARGET_STATE;
                ap_stabilizationTimer->Reset();
                ap_stabilizationTimer->Start();
            }
            // Error is negative and outside the epsilon band.
            else if (a_currentError <= -a_errorEpsilon)
            {
                new_state = WS_PID_ABOVE_TARGET_STATE;
                
                //Reset the error sum
                a_errorSum = 0.0;
            }
            else
            {
                //Stay here.
                new_state = WS_PID_BELOW_TARGET_STATE;
            }
            
            break;
        }
        
        case WS_PID_ON_TARGET_STATE:
        {
            a_errorSum = 0.0;
            a_allowStaticEpsilon = true;
            
            //Find the next state
            // Error is positive and outside the epsilon band.
            if (a_currentError >= a_errorEpsilon)
            {
                new_state = WS_PID_BELOW_TARGET_STATE;
                ap_stabilizationTimer->Stop();
                ap_stabilizationTimer->Reset();
            }
            else if (a_currentError <= -a_errorEpsilon)
            {
                new_state = WS_PID_ABOVE_TARGET_STATE;
                ap_stabilizationTimer->Stop();
                ap_stabilizationTimer->Reset();
            }
            else if (true == ap_stabilizationTimer->HasPeriodPassed(a_minOnTargetTime))
            {
                new_state = WS_PID_STABILIZED_STATE;
                ap_stabilizationTimer->Stop();
                ap_stabilizationTimer->Reset();
            }
            else
            {
                // Stay right here, we are on target, but not long enough yet...
                new_state = WS_PID_ON_TARGET_STATE;
            }
            break;
        }
        
        case WS_PID_STABILIZED_STATE:
        {
            a_errorSum = 0.0;
            a_allowStaticEpsilon = true;
            
            // Find the next state
            // Error is positive and outside the epsilon band.
            if (a_currentError >= a_errorEpsilon)
            {
                new_state = WS_PID_BELOW_TARGET_STATE;
            }
            else if (a_currentError <= -a_errorEpsilon)
            {
                new_state = WS_PID_ABOVE_TARGET_STATE;
            }
            else
            {
                new_state = WS_PID_STABILIZED_STATE;
            }
            
            break;
        }
        
        case WS_PID_ABOVE_TARGET_STATE:
        {
            //
            // In this case, we were below and we just switched to above
            //
            if (a_errorSum > 0)
            {
                // If we are fighting away from the point, reset the error.
                a_errorSum = 0;
            }
            if (a_currentError > -a_errorIncrement)
            {
                // If the error is smaller than the max increment amount, add it.
                a_errorSum += a_currentError;
            }
            else
            {
                // Otherwise, subtract the maximum increment per cycle.
                a_errorSum += -a_errorIncrement;
            }
            
            if (a_currentError <= -a_staticEpsilon)
            {
                //Check to see if we are outside the static epsilon region
                a_allowStaticEpsilon = false;
            }
            
            //Find the next state
            // Error is inside the epsilon band.
            if ((a_currentError >= (-1 * a_errorEpsilon)) &&
                (a_currentError <= a_errorEpsilon))
            {
                new_state = WS_PID_ON_TARGET_STATE;
                ap_stabilizationTimer->Reset();
                ap_stabilizationTimer->Start();
            }
            // Error is positive and outside the epsilon band.
            else if (a_currentError >= a_errorEpsilon)
            {
                new_state = WS_PID_BELOW_TARGET_STATE;
                //Reset the error sum
                
                a_errorSum = 0.0;
            }
            else
            {
                new_state = WS_PID_ABOVE_TARGET_STATE;
            }
            
            break;
        }
        
        default:
        {
            // Invalid state
            WS_LOG_ERROR("Invalid state encountered: %d", a_currentState);
            WS_LOG_ERROR("Switching to disabled.");
            
            new_state = WS_PID_DISABLED_STATE;
            
            break;
        }
    } // end switch()
    
    // Don't forget to update the FSM
    WS_LOG_DEBUG("WsPid, Current Error = %.05f, Old State = %d, New State = %d",
                 a_currentError,
                 a_currentState,
                 new_state);
    a_currentState = new_state;
    
    // Finally, calculate the PID output
    float output = this->calcProportionalTerm() +
                   this->calcIntegralTerm() +
                   this->calcDerivativeTerm();
                   
    // Handle Static Epsilon
    if ((a_allowStaticEpsilon == true) &&
        fabs(a_currentError) < a_staticEpsilon)
    {
        output = 0;
        a_errorSum = 0;
    }
    
    // Clip the output to the allowable region
    output = this->limitOutput(output);
    
    // Write to the PidOutput object
    ap_pidOutput->pidWrite(output);
    
    // Save the current error for next cycle's D calculation.
    a_previousError = a_currentError;
    
    return;
}

bool
WsPidController::isOnTarget(void) const
{
    bool on_target = ((WS_PID_ON_TARGET_STATE == a_currentState) ||
                      (WS_PID_STABILIZED_STATE == a_currentState));
    return on_target;
}

bool
WsPidController::isStabilized(void) const
{
    bool stabilized = (WS_PID_STABILIZED_STATE == a_currentState);
    return stabilized;
}

float
WsPidController::calcProportionalTerm(void)
{
    float p_term = a_p * a_currentError;
    WS_LOG_DEBUG("p_term = %.05f", p_term);
    return p_term;
}

float
WsPidController::calcIntegralTerm(void)
{
    // Prevent Integral Wind Up.
    if (a_integralErrorThresh != -1)
    {
        if (a_currentError > a_integralErrorThresh ||
            a_currentError < -a_integralErrorThresh)
        {
            a_errorSum = 0;
        }
    }
    
    float i_term = a_i * a_errorSum;
    if (i_term > a_maxIntegral)
    {
        i_term = a_maxIntegral;
    }
    else if (i_term < -a_maxIntegral)
    {
        i_term = -a_maxIntegral;
    }
    else
    {
        // Leave i-term alone
    }
    
    WS_LOG_DEBUG("i_term = %.05f", i_term);
    return i_term;
}

float
WsPidController::calcDerivativeTerm(void)
{
    float d_term = a_d * (a_currentError - a_previousError);
    
    // Band-limit the differential term
    if (fabs(d_term) > a_differentiatorBandLimit)
    {
        d_term = (d_term > 0.0f)
                 ? a_differentiatorBandLimit
                 : -a_differentiatorBandLimit;
    }
    
    WS_LOG_DEBUG("d_term = %.05f", d_term);
    return d_term;
}

float
WsPidController::limitOutput(float output) const
{
    float clipped_output = 0.0;
    if (output > a_maxOutput)
    {
        clipped_output = a_maxOutput;
    }
    else if (output < a_minOutput)
    {
        clipped_output = a_minOutput;
    }
    else
    {
        // Output is already in range, just pass it along
        clipped_output = output;
    }
    
    return clipped_output;
}

float
WsPidController::limitInput(float input) const
{
    float clipped_input = 0.0;
    if (input > a_maxInput)
    {
        clipped_input = a_maxInput;
    }
    else if (input < a_minInput)
    {
        clipped_input = a_minInput;
    }
    else
    {
        // Input is already in range, just pass it along
        clipped_input = input;
    }
    
    return clipped_input;
}

void
WsPidController::setSetPoint(const float& rc_set_point)
{
    a_setPoint = rc_set_point;
}

void
WsPidController::getSetPoint(float& r_set_point)
{
    r_set_point = a_setPoint;
}

void WsPidController::setDifferentiatorBandLimit(const float& rc_band_limit)
{
    a_differentiatorBandLimit = rc_band_limit;
}

void WsPidController::getDifferentiatorBandLimit(float& rc_band_limit)
{
    rc_band_limit = a_differentiatorBandLimit;
}

bool
WsPidController::isEnabled(void) const
{
    return (a_currentState != WS_PID_DISABLED_STATE);
}

void
WsPidController::Enable(void)
{
    if (a_currentState == WS_PID_DISABLED_STATE)
    { a_currentState = WS_PID_INITIALIZE_STATE; }
}

void
WsPidController::Disable(void)
{
    this->Reset();
    a_currentState = WS_PID_DISABLED_STATE;
}

void
WsPidController::Reset(void)
{
    a_errorSum = 0.0f;
    a_currentError = 0.0f;
    a_previousError = 0.0f;
    a_allowStaticEpsilon = true;
}

float
WsPidController::getError(void)
{
    return a_currentError;
}

float WsPidController::getIntegralErrorThresh(void)
{
    return a_integralErrorThresh;
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

