
#ifndef __WsPidController_hh__
#define __WsPidController_hh__

#include <string>

class Timer;
class WsPidInput;
class WsPidOutput;

class WsPidController
{
    public:
        WsPidController(WsPidInput* p_source,
                        WsPidOutput* p_output,
                        const char* pc_pidControllerName);
        virtual ~WsPidController();
        
        typedef enum
        {
            WS_PID_DISABLED_STATE = 0,
            WS_PID_INITIALIZE_STATE,
            WS_PID_BELOW_TARGET_STATE,
            WS_PID_ON_TARGET_STATE,
            WS_PID_STABILIZED_STATE,
            WS_PID_ABOVE_TARGET_STATE
        } WsPidStateT;
        
        // Accessor methods
        float getP(void);
        float getI(void);
        float getD(void);
        float getEpsilon(void);
        float getStaticEpsilon(void);
        
        void setConstants(float p, float i, float d);
        void setStaticEpsilon(float epsilon);
        void setErrorEpsilon(float epsilon);
        void setErrorIncrementPercentage(float inc);
        void setMinMaxOutput(const float& rc_min, const float& rc_max);
        void setMinMaxInput(const float& rc_min, const float& rc_max);
        void setMaxIntegral(float max);
        void setIntegralErrorThresh(float thresh);
        float getIntegralErrorThresh(void);
        void setMinStabilizationTime(const float& rc_time);
        void setSetPoint(const float& rc_set_point);
        void getSetPoint(float& r_set_point);
        void setDifferentiatorBandLimit(const float& rc_band_limit);
        void getDifferentiatorBandLimit(float& rc_band_limit);
        
        // Behavioral methods
        void resetErrorSum(void);
        void calcPid(void);
        bool isOnTarget(void) const;
        bool isStabilized(void) const;
        bool isEnabled(void) const;
        
        //WPI PIDController-like functionality
        void Enable(void);
        void Disable(void);
        void Reset(void);
        
        //GetError
        float getError(void);
        
    private:
        float a_p;   // P coefficient
        float a_i;   // I coefficient
        float a_d;   // D coefficient
        
        float a_currentError;  //Current error I guess
        float a_previousError; //Previous error
        float a_setPoint; //The set point
        float a_errorSum; // Sum of previous errors (for I calculation)
        float a_errorIncrement; // Max increment to error sum each call
        float a_errorEpsilon; // Allowable error in determining when done
        float a_staticEpsilon; // Allowable error in steady state.  Used to account for small acceptable motor backdrive
        float a_maxIntegral;
        float a_integralErrorThresh;
        float a_differentiatorBandLimit;
        
        float a_maxOutput; // Ceiling on calculation output
        float a_minOutput;
        float a_maxInput; // Ceiling on calculation output
        float a_minInput;
        WsPidStateT a_currentState; //State of the pid for calculating I
        
        double a_minOnTargetTime; // Minimum number of cycles in epsilon range to be done
        bool a_allowStaticEpsilon;
        Timer*  ap_stabilizationTimer;
        
        WsPidInput* ap_pidSource;
        WsPidOutput* ap_pidOutput;
        
        bool a_enabled;
        std::string a_controllerName;
        
    private:
        float calcProportionalTerm(void);
        float calcIntegralTerm(void);
        float calcDerivativeTerm(void);
        float limitOutput(float output) const;
        float limitInput(float input) const;
        
    private:
        WsPidController(const WsPidController& rc_rhs);
        const WsPidController& operator=(const WsPidController& rc_rhs);
        
};

#endif // __WsPidController_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

