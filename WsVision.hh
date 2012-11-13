#ifndef __WSVISION_H__
#define __WSVISION_H__

#include "WsTypes.hh"

class WsAxisCamera;
class Task;
class Timer;
class WsVision
{
    public:
        ~WsVision(void);
        static WsVision* instance(void);
        
    private:
        // Methods called in runCamera()
        void initCamera(void);
        void pollCamera(void);
        
        // Helper methods to figure out where particles are
        WsTypes::WsTargetImageInfoType* getHighestParticleInArray(WsTypes::WsTargetImageInfoType* p_imgInfoArray,
                                                                  int numParticles);
        WsTypes::WsTargetImageInfoType* getLowestParticleInArray(WsTypes::WsTargetImageInfoType* p_imgInfoArray,
                                                                 int numParticles);
        WsTypes::WsTargetImageInfoType* getLeftMostParticleInArray(WsTypes::WsTargetImageInfoType* p_imgInfoArray,
                                                                   int numParticles);
        WsTypes::WsTargetImageInfoType* getRightMostParticleInArray(WsTypes::WsTargetImageInfoType* p_imgInfoArray,
                                                                    int numParticles);
                                                                    
        // Method called for task
        void runCamera();
        
        // Static instance variable
        static WsVision*      asp_instance;
        static int initCameraTask(WsVision* p_obj);
        
        // Private member variables
        WsAxisCamera*  ap_camera;
        Task*          ap_task;
        
    private:
        WsVision(void);
        WsVision(const WsVision& rc_rhs);
        const WsVision& operator=(const WsVision& rc_rhs);
        
        Timer* ap_cameraWatchdog;
};

#endif

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

