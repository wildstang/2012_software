#ifndef __WsConfigFacade_hh__
#define __WsConfigFacade_hh__

#include "WsFacadeElement.hh"
#include "WsTypes.hh"
#include "semlib.h"
#include <string>
#include <map>

class Subject;
class WsConfigFacade
{
    public:
        static WsConfigFacade* instance(void);
        
        // This method refreshes the master data from the config file
        void updateCache(void);
        
        // Accessor methods
        WsSubject* getTurretPidConfig(WsTypes::WsPidConfigurationDataT& r_config);
        void       setTurretPidConfig(const WsTypes::WsPidConfigurationDataT& rc_config);
        WsSubject* getFlywheelPidConfig(WsTypes::WsPidConfigurationDataT& r_config);
        void       setFlywheelPidConfig(const WsTypes::WsPidConfigurationDataT& rc_config);
        WsSubject* getFlywheelLocationSpeedMapping(WsTypes::WsLocationSpeedMappingType& r_mappingData);
        void       setFlywheelLocationSpeedMapping(const WsTypes::WsLocationSpeedMappingType& rc_mappingData);
        WsSubject* getAutoShoot6Config(WsTypes::WsAutoShootProgramConfigType& r_config);
        void       setAutoShoot6Config(const WsTypes::WsAutoShootProgramConfigType& rc_config);
        WsSubject* getCameraProcessingImaqData(WsTypes::WsCameraProcessingImaqData& r_imaqData);
        void       setCameraProcessingImaqData(const WsTypes::WsCameraProcessingImaqData& rc_imaqData);
        WsSubject* getBallTransportMotorValsConfig(WsTypes::WsBallTransportMotorValueType& r_bt_vals);
        void       setBallTransportMotorValsConfig(const WsTypes::WsBallTransportMotorValueType& rc_bt_vals);
        WsSubject* getAutoTipShootConfig(WsTypes::WsAutoTipShootProgramConfigType& r_config);
        void       setAutoTipShootConfig(const WsTypes::WsAutoTipShootProgramConfigType& rc_config);
        WsSubject* getAutoShoot3Config(WsTypes::WsAutoShootProgramConfigType& r_config);
        void       setAutoShoot3Config(const WsTypes::WsAutoShootProgramConfigType& rc_config);
        WsSubject* getAutoShootTipConfig(WsTypes::WsAutoShootTipProgramConfigType& r_config);
        void       setAutoShootTipConfig(const WsTypes::WsAutoShootTipProgramConfigType& rc_config);
        WsSubject* getShooterPresetConfig(WsTypes::WsShooterPresetConfigT& r_config);
        void       setShooterPresetConfig(const WsTypes::WsShooterPresetConfigT& rc_config);
        WsSubject* getAutoDriveConfig(WsTypes::WsAutoDriveProgramConfigType& r_config);
        void       setAutoDriveConfig(const WsTypes::WsAutoDriveProgramConfigType& rc_config);
        WsSubject* getAutoFenderRightConfig(WsTypes::WsAutoFenderRightProgramConfigType& r_config);
        void       setAutoFenderRightConfig(const WsTypes::WsAutoFenderRightProgramConfigType& rc_config);
        WsSubject* getAutoFenderLeftConfig(WsTypes::WsAutoFenderLeftProgramConfigType& r_config);
        void       setAutoFenderLeftConfig(const WsTypes::WsAutoFenderLeftProgramConfigType& rc_config);
        
    protected:
        WsConfigFacade(void);
        
    private:
        ~WsConfigFacade(void);
        
        // Singleton Instance
        static WsConfigFacade* asp_instance;
        SEM_ID                 ap_mutexSemaphore;
        
        // These subjects can be observed by many areas in code
        WsFacadeElement<WsTypes::WsPidConfigurationDataT>*         ap_turretPidConfigElement;
        WsFacadeElement<WsTypes::WsPidConfigurationDataT>*         ap_flywheelPidConfigElement;
        WsFacadeElement<WsTypes::WsLocationSpeedMappingType>*      ap_flywheelLocationSpeedMappingElement;
        WsFacadeElement<WsTypes::WsAutoShootProgramConfigType>*    ap_autoShoot6ConfigElement;
        WsFacadeElement<WsTypes::WsCameraProcessingImaqData>*      ap_cameraProcessingImaqData;
        WsFacadeElement<WsTypes::WsBallTransportMotorValueType>*   ap_ballTransportMotorValsElement;
        WsFacadeElement<WsTypes::WsAutoTipShootProgramConfigType>* ap_autoTipShootConfigElement;
        WsFacadeElement<WsTypes::WsAutoShootProgramConfigType>*    ap_autoShoot3ConfigElement;
        WsFacadeElement<WsTypes::WsAutoShootTipProgramConfigType>* ap_autoShootTipConfigElement;
        WsFacadeElement<WsTypes::WsShooterPresetConfigT>*          ap_shooterPresetConfigElement;
        WsFacadeElement< WsTypes::WsAutoDriveProgramConfigType >*  ap_autoDriveProgramConfigElement;
        WsFacadeElement< WsTypes::WsAutoFenderRightProgramConfigType >*  ap_autoFenderRightProgramConfigElement;
        WsFacadeElement< WsTypes::WsAutoFenderLeftProgramConfigType >*  ap_autoFenderLeftProgramConfigElement;
        
        // This is the master data structure
        typedef std::map<std::string, std::string>  WsConfigMapType;
        WsConfigMapType                            a_configurationDatabase;
        
    private:
        // Helper methods to convert string data to various POD types
        float stringToFloat(const std::string& rc_string, float defaultValue = 0.0f);
        bool stringToBoolean(const std::string& rc_string, bool defaultValue = false);
        int stringToInt(const std::string& rc_string, int defaultValue = 0);
        
        // Helper methods to extract specific data sets from the master DB (used in init/update)
        void extractTurretPidConfig(WsTypes::WsPidConfigurationDataT& r_config);
        void extractFlywheelPidConfig(WsTypes::WsPidConfigurationDataT& r_config);
        void extractFlywheelLocationSpeedMapping(WsTypes::WsLocationSpeedMappingType& r_config);
        void extractAutoShoot6Config(WsTypes::WsAutoShootProgramConfigType& r_config);
        void extractCameraProcessingImaqData(WsTypes::WsCameraProcessingImaqData& r_config);
        void extractBallTransportMotorValsConfig(WsTypes::WsBallTransportMotorValueType& r_config);
        void extractAutoTipShootConfig(WsTypes::WsAutoTipShootProgramConfigType& r_config);
        void extractAutoShoot3Config(WsTypes::WsAutoShootProgramConfigType& r_config);
        void extractAutoShootTipConfig(WsTypes::WsAutoShootTipProgramConfigType& r_config);
        void extractShooterPresetConfig(WsTypes::WsShooterPresetConfigT& r_config);
        void extractAutoDriveProgramConfig(WsTypes::WsAutoDriveProgramConfigType& r_config);
        void extractAutoFenderRightProgramConfig(WsTypes::WsAutoFenderRightProgramConfigType& r_config);
        void extractAutoFenderLeftProgramConfig(WsTypes::WsAutoFenderLeftProgramConfigType& r_config);
        
        // Helper method to dump the current configuration to a series of engineering logs
        void dumpDatabaseToLog(void);
};

#endif // __WsConfigFacade_hh__

//-----------------------------------------------------------------------------
// END OF FILE
//-----------------------------------------------------------------------------

