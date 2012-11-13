#ifndef __WsGyro_hh__
#define __WsGyro_hh__

#include "SensorBase.h"
#include "PIDSource.h"

class AnalogChannel;
class AnalogModule;

// Stolen from WPI Lib
class WsGyro : public SensorBase, public PIDSource
{
    public:
        static const UINT32 kOversampleBits = 10;
        static const UINT32 kAverageBits = 0;
        static const float kSamplesPerSecond = 50.0;
        static const float kCalibrationSampleTime = 15.0;
        static const float kDefaultVoltsPerDegreePerSecond = 0.007;
        
        WsGyro(UINT8 moduleNumber, UINT32 channel);
        explicit WsGyro(UINT32 channel);
        explicit WsGyro(AnalogChannel* channel);
        explicit WsGyro(AnalogChannel& channel);
        virtual ~WsGyro();
        virtual float GetAngle();
        void SetSensitivity(float voltsPerDegreePerSecond);
        virtual void Reset();
        
        // PIDSource interface
        double PIDGet();
        
    private:
        void InitGyro();
        
        AnalogChannel* m_analog;
        float m_voltsPerDegreePerSecond;
        float m_offset;
        bool m_channelAllocated;
};

#endif // __WsGyro_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

