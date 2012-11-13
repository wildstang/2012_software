/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef __AXIS_CAMERA_H__
#define __AXIS_CAMERA_H__

#include <taskLib.h>
#include <vxWorks.h>
#include <sockLib.h>
#include <inetLib.h>

#include "Vision/AxisCameraParams.h"
#if JAVA_CAMERA_LIB != 1
#include "Vision/ColorImage.h"
#include "Vision/HSLImage.h"
#endif
#include "nivision.h"
#include <set>
#include "Task.h"

/**
 * AxisCamera class.
 * This class handles everything about the Axis 206 FRC Camera.
 * It starts up 2 tasks each using a different connection to the camera:
 * - image reading task that reads images repeatedly from the camera
 * - parameter handler task in the base class that monitors for changes to
 *     parameters and updates the camera
 */
class WsAxisCamera : public AxisCameraParams
{
    private:
        explicit WsAxisCamera(const char* cameraIP);
    public:
        virtual ~WsAxisCamera();
        static WsAxisCamera& GetInstance(const char* cameraIP = NULL);
        static void DeleteInstance();
        
        bool IsFreshImage();
        SEM_ID GetNewImageSem();
        
        int GetImage(Image* imaqImage);
#if JAVA_CAMERA_LIB != 1
        int GetImage(ColorImage* image);
        HSLImage* GetImage();
#endif
        
        int CopyJPEG(char** destImage, int& destImageSize, int& destImageBufferSize);
        
    private:
        static int s_ImageStreamTaskFunction(WsAxisCamera* thisPtr);
        int ImageStreamTaskFunction();
        
        int ReadImagesFromCamera();
        void UpdatePublicImageFromCamera(char* imgBuffer, int imgSize);
        
        virtual void RestartCameraTask();
        
        static WsAxisCamera* _instance;
        int m_cameraSocket;
        typedef std::set<SEM_ID> SemSet_t;
        SemSet_t m_newImageSemSet;
        
        char* m_protectedImageBuffer;
        int m_protectedImageBufferLength;
        int m_protectedImageSize;
        SEM_ID m_protectedImageSem;
        bool m_freshImage;
        
        Task m_imageStreamTask;
};

#if JAVA_CAMERA_LIB == 1
#ifdef __cplusplus
extern "C" {
#endif
    void WsAxisCameraStart(const char* IPAddress);
    int WsAxisCameraGetImage(Image* image);
    void WsAxisCameraDeleteInstance();
    int WsAxisCameraFreshImage();
    
    // Mid-stream gets & writes
    void WsAxisCameraWriteBrightness(int brightness);
    int WsAxisCameraGetBrightness();
    void WsAxisCameraWriteWhiteBalance(WsAxisCameraParams::WhiteBalance_t whiteBalance);
    WsAxisCameraParams::WhiteBalance_t WsAxisCameraGetWhiteBalance();
    void WsAxisCameraWriteColorLevel(int colorLevel);
    int WsAxisCameraGetColorLevel();
    void WsAxisCameraWriteExposureControl(WsAxisCameraParams::Exposure_t exposure);
    WsAxisCameraParams::Exposure_t WsAxisCameraGetExposureControl();
    void WsAxisCameraWriteExposurePriority(int exposurePriority);
    int WsAxisCameraGetExposurePriority();
    void WsAxisCameraWriteMaxFPS(int maxFPS);
    int WsAxisCameraGetMaxFPS();
    
    // New-Stream gets & writes
    void WsAxisCameraWriteResolution(WsAxisCameraParams::Resolution_t resolution);
    WsAxisCameraParams::Resolution_t WsAxisCameraGetResolution();
    void WsAxisCameraWriteCompression(int compression);
    int WsAxisCameraGetCompression();
    void WsAxisCameraWriteRotation(WsAxisCameraParams::Rotation_t rotation);
    WsAxisCameraParams::Rotation_t WsAxisCameraGetRotation();
#ifdef __cplusplus
}
#endif
#endif // JAVA_CAMERA_LIB == 1

#endif
