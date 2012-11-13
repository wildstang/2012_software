#include "WsVision.hh"
#include "WsLogger.hh"
#include "WsAxisCamera.hh"
#include "WsInputFacade.hh"
#include "Task.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "Vision/MonoImage.h"
#include "nivision.h"
#include "Timer.h"

#include <cstring>

#define CAMERA_WATCH_TIME 2.0
#define WS_CAMERA_IP "10.1.11.11"       // The real camera's IP Address

// Initialize static member data
WsVision* WsVision::asp_instance = NULL;
WsVision::WsVision(void)
    : ap_camera(NULL),
      ap_task(NULL),
      ap_cameraWatchdog(NULL)
{
    ap_task = new Task("WsCameraTask", (FUNCPTR)WsVision::initCameraTask);
    
    // Create task for creating the camera and acquiring the image
    if (false == ap_task->Start((UINT32)this))
    {
        WS_LOG_ERROR("WsVision Camera Task did not start properly");
    }
}

WsVision::~WsVision(void)
{
    // Make dead sure that the pollCamera method won't be called anymore
    ap_task->Stop();
    delete ap_task ;                ap_task = NULL;
    WsAxisCamera::DeleteInstance(); ap_camera = NULL;
    asp_instance = NULL;
}

WsVision*
WsVision::instance(void)
{
    if (NULL == asp_instance)
    {
        asp_instance = new WsVision();
    }
    return asp_instance;
}

int
WsVision::initCameraTask(WsVision* p_obj)
{
    p_obj->runCamera();
    return 0;
}

void
WsVision::runCamera()
{
    // Wait for camera to boot up
    taskDelay(15000);
    
    // Create a camera instance
    this->initCamera();
    
    // Loop forever, and grab/process new images
    while (1)
    {
        this->pollCamera();
        taskDelay(5);
    }
}


void
WsVision::initCamera(void)
{
    WS_LOG_DEBUG("Camera Init Start");
    
    WS_LOG_ERROR("Just Before GetInstance()");
    ap_camera = &WsAxisCamera::GetInstance(WS_CAMERA_IP);
    WS_LOG_ERROR("Just Before GetInstance()");
    
    // @TODO:
    //  Set white balance and other camera parameter config here
    
    // Done
    WS_LOG_DEBUG("Camera Init End");
}

void
WsVision::pollCamera(void)
{
    // Only update the image information if the LEDs are on
    bool flood_light_state = false;
    WsInputFacade::instance()->getFloodLightButtonState(flood_light_state);
    
    // Let interested parties know that there is no image data
    bool no_image = false;
    WsInputFacade::instance()->setFreshImageFlag(no_image);
    
    if (false == flood_light_state)
    {
        //---------------------------------------------------------------------
        // Update Input Facade Below This Line
        //---------------------------------------------------------------------
        int no_targets = 0;
        WsInputFacade::instance()->setNumVisibleTargets(no_targets);
        WsTypes::WsTargetBasketIdxType idx;
        WsTypes::WsTargetImageInfoType img_info_dummy;       ::memset(&img_info_dummy, 0x00, sizeof(img_info_dummy));
        img_info_dummy.has_target = false;
        
        idx = WsTypes::WS_TOP_BASKET_INDEX;
        WsInputFacade::instance()->setTargetImageInfo(idx, img_info_dummy);
        
        idx = WsTypes::WS_LEFT_BASKET_INDEX;
        WsInputFacade::instance()->setTargetImageInfo(idx, img_info_dummy);
        
        idx = WsTypes::WS_RIGHT_BASKET_INDEX;
        WsInputFacade::instance()->setTargetImageInfo(idx, img_info_dummy);
        
        idx = WsTypes::WS_BOTTOM_BASKET_INDEX;
        WsInputFacade::instance()->setTargetImageInfo(idx, img_info_dummy);
        //---------------------------------------------------------------------
        // Update Input Facade Above This Line
        //---------------------------------------------------------------------
        
        return;
    }
    
    if (true == ap_camera->IsFreshImage())
    {
        //---------------------------------------------------------------------
        // Image Processing Below This Line
        //---------------------------------------------------------------------
        
        // Get Image
        WS_LOG_WARNING("Image Fetch Start");
        HSLImage* p_hsl = new HSLImage();
        int fetch_result = ap_camera->GetImage(p_hsl);  // This should clear the "fresh image" bit
        
        taskDelay(5);
        if (1 == fetch_result)
        { WS_LOG_DEBUG("Image Fetch Successful"); }
        else
        { WS_LOG_ERROR("Image Fetch Failed"); }
        WS_LOG_DEBUG("Image Fetch Complete");
        
        
        WS_LOG_DEBUG("Image Filtering Start");
        MonoImage* p_mono = p_hsl->GetLuminancePlane();   // Creates new memory
        
        // Threshold on the luminance plane
        BinaryImage* p_lum_thresh = new BinaryImage();
        Image* p_mono_imaq = p_mono->GetImaqImage();
        if (0 == imaqThreshold(p_lum_thresh->GetImaqImage(), p_mono_imaq, 154, 215, TRUE, 1))
        {
            WS_LOG_ERROR("imaqThreshold() Failed");
        }
        taskDelay(5);
        
        // Convex Hull
        BinaryImage* p_convex_hull = p_lum_thresh->ConvexHull(true);
        taskDelay(5);
        
        // Filter on min bounding rectagle width
        ParticleFilterCriteria2 min_br_width_criteria;
        min_br_width_criteria.parameter = IMAQ_MT_BOUNDING_RECT_WIDTH;
        min_br_width_criteria.lower = 35;
        min_br_width_criteria.upper = 209;    // 189 actual largest + safe zone 10ish %
        min_br_width_criteria.calibrated = 0;
        min_br_width_criteria.exclude = 0;
        BinaryImage* p_wide_rects = p_convex_hull->ParticleFilter(&min_br_width_criteria, 1);
        taskDelay(5);
        
        // Filter on min bounding rectagle height
        ParticleFilterCriteria2 min_br_height_criteria;
        min_br_height_criteria.parameter = IMAQ_MT_BOUNDING_RECT_HEIGHT;
        min_br_height_criteria.lower = 25;
        min_br_height_criteria.upper = 160;    // 143 actual largest + safe zone 10ish %
        min_br_height_criteria.calibrated = 0;
        min_br_height_criteria.exclude = 0;
        BinaryImage* p_targets = p_wide_rects->ParticleFilter(&min_br_height_criteria, 1);
        taskDelay(5);
        
        WS_LOG_DEBUG("Image Filtering Complete");
        //---------------------------------------------------------------------
        // Image Processing Above This Line
        //---------------------------------------------------------------------
        
        
        //---------------------------------------------------------------------
        // Measure Data Below This Line
        //---------------------------------------------------------------------
        // How many particles did we find?
        int num_particles = p_targets->GetNumberParticles();
        WS_LOG_DEBUG("Num Particles = %d", num_particles);
        
        // Grab space for 4 particles (generically)
        WsTypes::WsTargetImageInfoType p_image_info_array[4];
        ::memset(p_image_info_array, 0x00, sizeof(p_image_info_array));
        
        // Grab space for 4 ordered particles
        WsTypes::WsTargetImageInfoType image_info_top;       ::memset(&image_info_top, 0x00, sizeof(image_info_top));
        WsTypes::WsTargetImageInfoType image_info_left;      ::memset(&image_info_left, 0x00, sizeof(image_info_left));
        WsTypes::WsTargetImageInfoType image_info_right;     ::memset(&image_info_right, 0x00, sizeof(image_info_right));
        WsTypes::WsTargetImageInfoType image_info_bottom;    ::memset(&image_info_bottom, 0x00, sizeof(image_info_bottom));
        
        // Did we get any (and not more than we are expecting)?
        if ((num_particles > 0) &&
            (num_particles < 5))
        {
            for (int i = 0; i < num_particles; i++)
            {
                taskDelay(5);
                int success_count = 0;
                int measurement_count = 0;
                double value = 0.0f;
                Image* p_img = p_targets->GetImaqImage();
                WsTypes::WsTargetImageInfoType* p_image_info = &(p_image_info_array[i]);
                p_image_info->has_target = true;
                
                // Get useful measurements
                success_count += imaqMeasureParticle(p_img, i, 0, IMAQ_MT_CENTER_OF_MASS_X, &value);
                measurement_count++;
                p_image_info->cm_x = static_cast<int>(value);
                
                success_count += imaqMeasureParticle(p_img, i, 0, IMAQ_MT_CENTER_OF_MASS_Y, &value);
                measurement_count++;
                p_image_info->cm_y = static_cast<int>(value);
                
                success_count += imaqMeasureParticle(p_img, i, 0, IMAQ_MT_BOUNDING_RECT_TOP, &value);
                measurement_count++;
                p_image_info->br_top = static_cast<int>(value);
                
                success_count += imaqMeasureParticle(p_img, i, 0, IMAQ_MT_BOUNDING_RECT_LEFT, &value);
                measurement_count++;
                p_image_info->br_left = static_cast<int>(value);
                
                success_count += imaqMeasureParticle(p_img, i, 0, IMAQ_MT_BOUNDING_RECT_HEIGHT, &value);
                measurement_count++;
                p_image_info->br_height = static_cast<int>(value);
                
                success_count += imaqMeasureParticle(p_img, i, 0, IMAQ_MT_BOUNDING_RECT_WIDTH, &value);
                measurement_count++;
                p_image_info->br_width = static_cast<int>(value);
                
                success_count += imaqMeasureParticle(p_img, i, 0, IMAQ_MT_COMPACTNESS_FACTOR, &value);
                measurement_count++;
                p_image_info->compactness = value;
                
                success_count += imaqMeasureParticle(p_img, i, 0, IMAQ_MT_AREA, &value);
                measurement_count++;
                p_image_info->target_area = static_cast<int>(value);
                
                // Check to see if anything went wrong
                if (measurement_count != success_count)
                {
                    WS_LOG_ERROR("Image Measurement Problem - success_count = %d", success_count);
                    
                    // Make sure to invalidate this image
                    ::memset(p_image_info, 0x00, sizeof(*p_image_info));
                    p_image_info->has_target = false;
                }
            }
            
            //
            // At this point, p_image_info_array has all the particles that we found.
            //  Now we need to figure out which particles are which and stuff them
            //  into the Input Facade.
            //
            WsTypes::WsTargetImageInfoType* p_img = NULL;
            switch (num_particles)
            {
                case 1:
                {
                    //
                    // This is the easiest case, just assume that the 1 image is
                    //  the center-lower image.  This assumption might turn out to
                    //  be bad later, and we could mitigate this by examining the
                    //  dimensions of the single particle and making sure it's
                    //  "big enough" to be the center lower target.
                    //
                    image_info_bottom = p_image_info_array[0];
                    image_info_bottom.has_target = true;
                    WS_LOG_WARNING("Bottom Particle Only!!");
                    break;
                }
                
                case 2:
                {
                    //
                    // This is a more difficult case... which 2 can we see?  Are
                    //  we on the hairy edge of being able to track anything?
                    //
                    // For now, we will assume these 2 cases:
                    // 1. If right particle is higher, then bottom + right
                    // 2. If left particle is higher, then bottom + left
                    //
                    WsTypes::WsTargetImageInfoType* p_left_img =
                        this->getLeftMostParticleInArray(p_image_info_array, num_particles);
                    WsTypes::WsTargetImageInfoType* p_right_img =
                        this->getRightMostParticleInArray(p_image_info_array, num_particles);
                        
                    if (p_left_img->cm_y < p_right_img->cm_y)
                    {
                        // Bottom & Left
                        image_info_bottom = *p_right_img;
                        image_info_bottom.has_target = true;
                        
                        image_info_left = *p_left_img;
                        image_info_left.has_target = true;
                        WS_LOG_WARNING("Bottom & Left Only!!");
                    }
                    else
                    {
                        // Bottom & Right
                        image_info_bottom = *p_left_img;
                        image_info_bottom.has_target = true;
                        
                        image_info_right = *p_right_img;
                        image_info_right.has_target = true;
                        WS_LOG_WARNING("Bottom & Right Only!!");
                    }
                    
                    break;
                }
                
                case 3:
                {
                    //
                    // This is a slightly easier case... The scenarios that
                    //  shoudl be considered are:
                    //  1. Left, Bottom, Right
                    //  2. Left, Bottom, Top
                    //  3. Left, Top, Right
                    //  4. Top, Right Bottom
                    //
                    
                    // Calculate a combined center of mass for all visible particles
                    int combined_cm_x = 0;
                    int combined_cm_y = 0;
                    
                    for (int i = 0; i < num_particles; i++)
                    {
                        combined_cm_x += p_image_info_array[i].cm_x;
                        combined_cm_y += p_image_info_array[i].cm_y;
                    }
                    combined_cm_x /= num_particles;
                    combined_cm_y /= num_particles;
                    
                    //
                    //     x=0
                    // y=0 +---------------------------+
                    //     |      |             |      |
                    //     |      |             |      |
                    //     |------|-------------|------| y1
                    //     |      |             |      |
                    //     |      |             |      |
                    //     |------|-------------|------| y2
                    //     |      |             |      |
                    //     |      |             |      |
                    //     +---------------------------+ y=240
                    //           x1            x2    x=320
                    //
                    // Depending on where the combined center of mass lies, we
                    //  will make determinations as to which targets we are
                    //  actually seeing.  For now, just look at the center of
                    //  mass on the x-coordinate to determine the collection.
                    //
                    const int X1 = 60;
                    const int X2 = 270;
                    
                    if (combined_cm_x < X1)
                    {
                        // Bottom, Left, Top
                        p_img = this->getLowestParticleInArray(p_image_info_array, num_particles);
                        image_info_bottom = *p_img;
                        image_info_bottom.has_target = true;
                        
                        p_img = this->getRightMostParticleInArray(p_image_info_array, num_particles);
                        image_info_right = *p_img;
                        image_info_right.has_target = true;
                        
                        p_img = this->getHighestParticleInArray(p_image_info_array, num_particles);
                        image_info_top = *p_img;
                        image_info_top.has_target = true;
                        WS_LOG_WARNING("Bottom, Right, Top Only!!");
                    }
                    else if (combined_cm_x > X2)
                    {
                        // Bottom, Right, Top
                        p_img = this->getLowestParticleInArray(p_image_info_array, num_particles);
                        image_info_bottom = *p_img;
                        image_info_bottom.has_target = true;
                        
                        p_img = this->getLeftMostParticleInArray(p_image_info_array, num_particles);
                        image_info_left = *p_img;
                        image_info_left.has_target = true;
                        
                        p_img = this->getHighestParticleInArray(p_image_info_array, num_particles);
                        image_info_top = *p_img;
                        image_info_top.has_target = true;
                        WS_LOG_WARNING("Bottom, Left, Top Only!!");
                    }
                    else
                    {
                        // Left, Bottom, Right
                        p_img = this->getLeftMostParticleInArray(p_image_info_array, num_particles);
                        image_info_left = *p_img;
                        image_info_left.has_target = true;
                        
                        p_img = this->getLowestParticleInArray(p_image_info_array, num_particles);
                        image_info_bottom = *p_img;
                        image_info_bottom.has_target = true;
                        
                        p_img = this->getRightMostParticleInArray(p_image_info_array, num_particles);
                        image_info_right = *p_img;
                        image_info_right.has_target = true;
                        WS_LOG_DEBUG("Left, Bottom, Right, Only!!");
                    }
                    break;
                }
                
                case 4:
                {
                    //
                    // This is the easiest case of all...
                    //  Simply order the particle image info by the corresponding
                    //  center-mass coordinates.
                    //
                    //     x=0
                    // y=0 +---------------------------+
                    //     |            +-+            |
                    //     |            +-+            |
                    //     |                           |
                    //     |       +-+       +-+       |
                    //     |       +-+       +-+       |
                    //     |                           |
                    //     |            +-+            |
                    //     |            +-+            |
                    //     +---------------------------+ y=240
                    //                               x=320
                    //
                    // Bottom, Left, Top, Right
                    p_img = this->getLowestParticleInArray(p_image_info_array, num_particles);
                    image_info_bottom = *p_img;
                    image_info_bottom.has_target = true;
                    
                    p_img = this->getLeftMostParticleInArray(p_image_info_array, num_particles);
                    image_info_left = *p_img;
                    image_info_left.has_target = true;
                    
                    p_img = this->getHighestParticleInArray(p_image_info_array, num_particles);
                    image_info_top = *p_img;
                    image_info_top.has_target = true;
                    
                    p_img = this->getRightMostParticleInArray(p_image_info_array, num_particles);
                    image_info_right = *p_img;
                    image_info_right.has_target = true;
                    
                    break;
                }
                
                default:
                {
                    // Something got really messed up because we shouldn't even
                    //  be in this code if the number of particles is out of range.
                    //
                    WS_LOG_ERROR("Unexpected number of particles: %d", num_particles);
                    break;
                }
            }
        }
        else
        {
            // Nope, no particles...
            WS_LOG_WARNING("No Particles Found!!");
        }
        
        //---------------------------------------------------------------------
        // Measure Data Above This Line
        //---------------------------------------------------------------------
        
        // Free all image memory
        delete p_hsl;
        delete p_mono;
        delete p_lum_thresh;
        delete p_convex_hull;
        delete p_wide_rects;
        delete p_targets;
        
        
        //---------------------------------------------------------------------
        // Update Input Facade Below This Line
        //---------------------------------------------------------------------
        WsInputFacade::instance()->setNumVisibleTargets(num_particles);
        WsTypes::WsTargetBasketIdxType idx;
        
        idx = WsTypes::WS_TOP_BASKET_INDEX;
        WsInputFacade::instance()->setTargetImageInfo(idx, image_info_top);
        WS_LOG_INFO("top basket: br_width = %d, br_height = %d",
                    image_info_top.br_width,
                    image_info_top.br_height);
                    
        idx = WsTypes::WS_LEFT_BASKET_INDEX;
        WsInputFacade::instance()->setTargetImageInfo(idx, image_info_left);
        WS_LOG_INFO("left basket: br_width = %d, br_height = %d",
                    image_info_left.br_width,
                    image_info_left.br_height);
                    
        idx = WsTypes::WS_RIGHT_BASKET_INDEX;
        WsInputFacade::instance()->setTargetImageInfo(idx, image_info_right);
        WS_LOG_INFO("right basket: br_width = %d, br_height = %d",
                    image_info_right.br_width,
                    image_info_right.br_height);
                    
        idx = WsTypes::WS_BOTTOM_BASKET_INDEX;
        WsInputFacade::instance()->setTargetImageInfo(idx, image_info_bottom);
        WS_LOG_INFO("bottom basket: br_width = %d, br_height = %d",
                    image_info_bottom.br_width,
                    image_info_bottom.br_height);
                    
        // Let interested parties know that there is new image data
        bool fresh_image = true;
        WsInputFacade::instance()->setFreshImageFlag(fresh_image);
        
        //---------------------------------------------------------------------
        // Update Input Facade Above This Line
        //---------------------------------------------------------------------
    }
    else
    {
        // No image ready... this probably won't happen
    }
}

WsTypes::WsTargetImageInfoType*
WsVision::getHighestParticleInArray(WsTypes::WsTargetImageInfoType* p_imgInfoArray,
                                    int numParticles)
{
    // Find the lowest particle
    int y_min = 240;
    int y_min_idx = -1;
    for (int i = 0; i < numParticles; i++)
    {
        if (p_imgInfoArray[i].cm_y > y_min)
        {
            y_min = p_imgInfoArray[i].cm_y;
            y_min_idx = i;
        }
    }
    
    return &(p_imgInfoArray[y_min_idx]);
}

WsTypes::WsTargetImageInfoType*
WsVision::getLowestParticleInArray(WsTypes::WsTargetImageInfoType* p_imgInfoArray,
                                   int numParticles)
{
    // Find the lowest particle
    int y_max = 0;
    int y_max_idx = -1;
    for (int i = 0; i < numParticles; i++)
    {
        if (p_imgInfoArray[i].cm_y > y_max)
        {
            y_max = p_imgInfoArray[i].cm_y;
            y_max_idx = i;
        }
    }
    
    return &(p_imgInfoArray[y_max_idx]);
}

WsTypes::WsTargetImageInfoType*
WsVision::getLeftMostParticleInArray(WsTypes::WsTargetImageInfoType* p_imgInfoArray,
                                     int numParticles)
{
    // Find the left-most particle
    int x_min = 320;
    int x_min_idx = -1;
    for (int i = 0; i < numParticles; i++)
    {
        if (p_imgInfoArray[i].cm_x < x_min)
        {
            x_min = p_imgInfoArray[i].cm_x;
            x_min_idx = i;
        }
    }
    
    return &(p_imgInfoArray[x_min_idx]);
}

WsTypes::WsTargetImageInfoType*
WsVision::getRightMostParticleInArray(WsTypes::WsTargetImageInfoType* p_imgInfoArray,
                                      int numParticles)
{
    // Find the right-most particle
    int x_max = 0;
    int x_max_idx = -1;
    for (int i = 0; i < numParticles; i++)
    {
        if (p_imgInfoArray[i].cm_x > x_max)
        {
            x_max = p_imgInfoArray[i].cm_x;
            x_max_idx = i;
        }
    }
    
    return &(p_imgInfoArray[x_max_idx]);
}

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

