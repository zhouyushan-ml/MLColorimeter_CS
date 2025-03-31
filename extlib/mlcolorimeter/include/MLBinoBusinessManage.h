// Copyright 2024 MLOptic

#ifndef SRC_MLCOLORIMETER_MLBINOBUSINESSMANAGE_H_
#define SRC_MLCOLORIMETER_MLBINOBUSINESSMANAGE_H_

#include <qobject.h>

#include <map>
#include <vector>

#include "MLColorimeterCommon.h"
#include "Result.h"
#include "mlcolorimeter_global.h"

/**
 * @file MLBinoBusinessManage.h
 * @brief Defines the binocular module business logic class and associated functionality.
 */

namespace ML {

namespace MLFilterWheel {
class MLFilterWheelCallback;
}  // namespace MLFilterWheel

namespace MLColorimeter {

class MLMonoBusinessManage;
class MLColorimeterAlgorithms;

/**
 * @class MLBinoBusinessManage
 * @brief Manages the hardware control and business logic for binocular module.
 *
 * The class provides a common interface to manage business logic and hardware control
 * including NDFilter, ColorFilter, RXFilter, Motor and Camera.
 */
class MLCOLORIMETER_EXPORT MLBinoBusinessManage : public QObject {
    Q_OBJECT

 public:
    MLBinoBusinessManage(QObject* parent = nullptr);
    ~MLBinoBusinessManage();

    /**
     * @brief Add a monocular businessmanage instance.
     * @param path  module's config path.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_AddModule(const char* path);

    /**
     * @brief Remove a monocular businessmanage instance added by ML_AddModule().
     * @param moduleID  module's id, from ModuleConfig.json
     * @return the result contains the message, code and status.
     */
    virtual Result ML_RemoveModule(int moduleID);

    /**
     * @brief Add a ipd module instance.
     * @param path  module's config path.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_AddIPDMotion(const char* path);

    /**
     * @brief Remove the ipd module added by ML_AddIPDMotion().
     * @return the result contains the message, code and status.
     */
    virtual Result ML_RemoveIPDMotion();

    /**
     * @brief Create and connect modules added by ML_AddModule() and ML_AddIPDMotion().
     * @return the result contains the message, code and status.
     */
    virtual Result ML_ConnectModules();

    /**
     * @brief Disconnect and destory modules added by ML_AddModule() and
     * ML_AddIPDMotion().
     * @return the result contains the message, code and status.
     */
    virtual Result ML_DisconnectModules();

    /**
     * @brief Check if all module created by ML_CreateModules() are connected.
     * @return true if all are connected, otherwise false.
     */
    virtual bool ML_IsModulesConnect();

    /**
     * @brief Check if all modules created by ML_CreateModules() are moving.
     * @return true if one is moving, otherwise false.
     */
    virtual bool ML_IsModulesMoving();

    /**
     * @brief Wait for all modules' movement finish.
     * @param timeout  time out limit (unit: millisecond).
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_WaitForMovingStop(int timeout = 10000,
                                        ML::MLColorimeter::OperationMode mode =
                                            ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief Stop all movements of modules.
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_StopModulesMovement(ML::MLColorimeter::OperationMode mode =
                                              ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief Get the serial number of the modules.
     * @return a map of modules' serial number.
     */
    virtual std::map<int, std::string> ML_GetModulesSerialNumber();

    /**
     * @brief Get the name of the modules.
     * @return a map of modules' name.
     */
    virtual std::map<int, std::string> ML_GetModulesName();

    /**
     * @brief Get the modules id list.
     * @return a list of modules' id.
     */
    virtual std::vector<int> ML_GetModulesIDList();

    /**
     * @brief Get the modules number.
     * @return the number of module in the MLBinoBusinessManage system.
     */
    virtual int ML_GetModulesNumber();

    /**
     * @brief Get a MLMonoBusinessManage instance by module serial number.
     * @param serialNumber  the serial number of the MLMonoBusinessManage instance.
     * @return the MLMonoBusinessManage instance.
     */
    virtual MLMonoBusinessManage* ML_GetModuleBySN(std::string serialNumber);

    /**
     * @brief Get a MLMonoBusinessManage instance by module name.
     * @param name  the name of the MLMonoBusinessManage instance.
     * @return the MLMonoBusinessManage instance.
     */
    virtual MLMonoBusinessManage* ML_GetModuleByName(std::string name);

    /**
     * @brief Get a MLMonoBusinessManage instance by id.
     * @param ID  the ID of the MLMonoBusinessManage instance.
     * @return the MLMonoBusinessManage instance.
     */
    virtual MLMonoBusinessManage* ML_GetModuleByID(int ID);

    /**
     * @brief Get a MLMonoBusinessManage instance by eyemode.
     * @param eyemode  the eyemode of the MLMonoBusinessManage instance.
     * @return the MLMonoBusinessManage instance.
     */
    virtual MLMonoBusinessManage* ML_GetModuleByEyeMode(
        ML::MLColorimeter::EyeMode eyemode);

    /**
     * @brief Get a MLColorimeterAlgorithms instance by id.
     * @param ID  the ID of the MLColorimeterAlgorithms instance.
     * @return the MLColorimeterAlgorithms instance.
     */
    virtual MLColorimeterAlgorithms* ML_GetCalibrationProcessByID(int ID);

    /**
     * @brief Get modules' config.
     * @return a map of module config of the MLBinoBusinessMange system.
     */
    virtual std::map<int, ML::MLColorimeter::ModuleConfig> ML_GetModulesConfig();

    /**
     * @brief  Switch ND/XYZ filter wheel's channel by enum for all modules
     * asynchronously.
     * @param keyName  the key name of FilterWheel, from the config.
     * @param channelName  the enum of channel to switch
     * @param mode  operation mode between multiple modules.
     * @param cb  callback function
     * @return the result contains the message, code and status.
     */
    virtual Result ML_MoveModulesND_XYZFilterAsync(
        std::string keyName, ML::MLFilterWheel::MLFilterEnum channelName,
        ML::MLColorimeter::OperationMode mode =
            ML::MLColorimeter::OperationMode::Parallel,
        ML::MLFilterWheel::MLFilterWheelCallback* cb = nullptr);

    /**
     * @brief  Switch ND/XYZ filter wheel's channel by enum for all modules
     * synchronously.
     * @param keyName  the key name of FilterWheel, from the config.
     * @param channelName  the enum of channel to switch
     * @param mode  operation mode between multiple modules.
     * @param cb  callback function
     * @return the result contains the message, code and status.
     */
    virtual Result ML_MoveModulesND_XYZFilterSync(
        std::string keyName, ML::MLFilterWheel::MLFilterEnum channelName,
        ML::MLColorimeter::OperationMode mode =
            ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Get the channel of ND/XYZ FilterWheel.
     * @param keyName  the key name of FilterWheel, from the config.
     * @return a map of Filter channel (format: {module id, enum of channel}).
     */
    virtual std::map<int, ML::MLFilterWheel::MLFilterEnum> ML_GetND_XYZFilterChannel(
        std::string keyName);

    /**
     * @brief  Switch RX filter wheel's channel by name and change RX axis degree
     * asynchronously.
     * @param channelName  the name of channel to switch
     * @param degree  the degree of axis to change
     * @param mode  operation mode between multiple modules.
     * @param cb  callback function
     * @return the result contains the message, code and status.
     */
    virtual Result ML_MoveModulesRXFilterAsync(
        std::string channelName, int degree,
        ML::MLColorimeter::OperationMode mode =
            ML::MLColorimeter::OperationMode::Parallel,
        ML::MLFilterWheel::MLFilterWheelCallback* cb = nullptr);

    /**
     * @brief  Switch RX filter wheel's channel by name and change RX axis degree
     * synchronously.
     * @param channelName  the name of channel to switch
     * @param degree  the degree of axis to change
     * @param mode  operation mode between multiple modules.
     * @param cb  callback function
     * @return the result contains the message, code and status.
     */
    virtual Result ML_MoveModulesRXFilterSync(
        std::string channelName, int degree,
        ML::MLColorimeter::OperationMode mode =
            ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Get the channel name of RX FilterWheel.
     * @return a map of Filter channel (format: {module id, name of channel}).
     */
    virtual std::map<int, std::string> ML_GetRXFilterChannel();

    /**
     * @brief  Get  the degree of RXFilterWheel's axis.
     * @return a map of Filter channel (format: {module id, axis degree}).
     */
    virtual std::map<int, int> ML_GetRXFilterAxis();

    /**
     * @brief  Set Focus by vid asynchronously.
     * @param vid  the vid to set (unit: millimeter).
     * @param mode  operation mode between multiple modules.
     * @param method  focus method (default: Inverse)
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetFocusAsync(
        double vid,
        ML::MLColorimeter::OperationMode mode =
            ML::MLColorimeter::OperationMode::Parallel,
        ML::MLColorimeter::FocusMethod method = ML::MLColorimeter::FocusMethod::Inverse);

    /**
     * @brief  Set Focus by vid synchronously.
     * @param vid  the vid to set (unit: millimeter).
     * @param mode  operation mode between multiple modules.
     * @param method  focus method (default: Inverse)
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetFocusSync(
        double vid,
        ML::MLColorimeter::OperationMode mode =
            ML::MLColorimeter::OperationMode::Parallel,
        ML::MLColorimeter::FocusMethod method = ML::MLColorimeter::FocusMethod::Inverse);

    /**
     * @brief  Get modules' vid (unit: millimeter).
     * @param method  focus method (default: Inverse)
     * @return a map of vid (format: {module id, vid value}).
     */
    virtual std::map<int, double> ML_GetVID(
        ML::MLColorimeter::FocusMethod method = ML::MLColorimeter::FocusMethod::Inverse);

    /**
     * @brief  Perform through focus and return the vid and position on best mtf.
     * @param keyName  the key name of Motion to perform through focus, from the config
     * @param VID  the VID on the best mtf
     * @param position  the position on the best mtf
     * @param focusconfig  through focus config, default from ThroughFocus.json
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_ThroughFocus(std::string keyName, std::map<int, double>& VID,
                                   std::map<int, double>& position,
                                   ML::MLColorimeter::ThroughFocusConfig focusconfig =
                                       ML::MLColorimeter::ThroughFocusConfig(),
                                   ML::MLColorimeter::OperationMode mode =
                                       ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Get fine vid curve after calling ML_ThroughFocus().
     * @return a map of fine vid curve (format: {module id, vid curve}).
     */
    virtual std::map<int, std::vector<double>> ML_GetVIDCurve();

    /**
     * @brief  Get fine mtf curve after calling ML_ThroughFocus().
     * @return a map of fine mtf curve (format: {module id, mtf curve}).
     */
    virtual std::map<int, std::vector<double>> ML_GetMTFCurve();

    /**
     * @brief  Get fine motion curve after calling ML_ThroughFocus().
     * @return a map of fine motion curve (format: {module id, motion curve}).
     */
    virtual std::map<int, std::vector<double>> ML_GetMotionCurve();

    /**
     * @brief  Set absolute motion position asynchronously.
     * @param keyName  the key name of Motion, from the config.
     * @param pos  absolute motion position to set (unit: millimeter).
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status
     */
    virtual Result ML_SetPosistionAbsAsync(
        std::string keyName, double pos,
        ML::MLColorimeter::OperationMode mode =
            ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Set absolute motion position synchronously.
     * @param keyName  the key name of Motion, from the config.
     * @param pos  absolute motion position to set (unit: millimeter).
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status
     */
    virtual Result ML_SetPosistionAbsSync(std::string keyName, double pos,
                                          ML::MLColorimeter::OperationMode mode =
                                              ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Set relative motion position asynchronously.
     * @param keyName  the key name of Motion, from the config.
     * @param pos  relative motion position to set (unit: millimeter).
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status
     */
    virtual Result ML_SetPositionRelAsync(std::string keyName, double pos,
                                          ML::MLColorimeter::OperationMode mode =
                                              ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Set relative motion position synchronously.
     * @param keyName  the key name of Motion, from the config.
     * @param pos  relative motion position to set (unit: millimeter).
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status
     */
    virtual Result ML_SetPositionRelSync(std::string keyName, double pos,
                                         ML::MLColorimeter::OperationMode mode =
                                             ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Get motion position (unit: millimeter).
     * @param keyName  the key name of Motion, from the config.
     * @return a map of position for keyName motion (format: {module id, position}).
     */
    virtual std::map<int, double> ML_GetMotionPosition(std::string keyName);

    /**
     * @brief  Stop motion movement.
     * @param keyName  the key name of Motion, from the config.
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_StopMotionMovement(std::string keyName,
                                         ML::MLColorimeter::OperationMode mode =
                                             ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Set Spherical power asynchronously.
     * @param sphere  sphere to set.
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetSphericalAsync(double sphere,
                                        ML::MLColorimeter::OperationMode mode =
                                            ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Set Spherical power synchronously.
     * @param sphere  sphere to set.
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetSphericalSync(double sphere,
                                       ML::MLColorimeter::OperationMode mode =
                                           ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Get Spherical power.
     * @return a map of spherical power (format: {module id, spherical value}).
     */
    virtual std::map<int, double> ML_GetSpherical();

    /**
     * @brief  Set RX asynchronously.
     * @param rx  RXCombination setting containing spherical power, cylindrical power and
     * cylindrical axis.
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetRXAsync(ML::MLColorimeter::RXCombination rx,
                                 ML::MLColorimeter::OperationMode mode =
                                     ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Set RX synchronously.
     * @param rx  RXCombination setting containing spherical power, cylindrical power and
     * cylindrical axis.
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetRXSync(ML::MLColorimeter::RXCombination rx,
                                ML::MLColorimeter::OperationMode mode =
                                    ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Get RX.
     * @return a map of RXCombination (format: {module id, RXCombination}).
     */
    virtual std::map<int, ML::MLColorimeter::RXCombination> ML_GetRX();

    /**
     * @brief  Set interpupillary distance for binocular module asynchronously.
     * @param ipd the interpupillary distance to set (unit: millimeter).
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetIPDAsync(double ipd);

    /**
     * @brief  Set interpupillary distance for binocular system synchronously.
     * @param ipd the interpupillary distance to set (unit: millimeter).
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetIPDSync(double ipd);

    /**
     * @brief  Get the interpupillary distance.
     * @return the interpupillary distance of binocular system.
     */
    virtual double ML_GetIPD();

    /**
     * @brief  Set aperture for MLBinoBusinessManage system.
     * @param aperture  aperture to set, example: 3mm.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetAperture(std::string aperture);

    /**
     * @brief  Get aperture from MLBinoBusinessManage system.
     * @return a map of aperture (format: {module id, aperture}).
     */
    virtual std::map<int, std::string> ML_GetAperture();

    /**
     * @brief  Set lightSource value for MLBinoBusinessManage system.
     * @param aperture  lightSource to set, example: R/G/B/W.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetLightSource(std::string lightSource);

    /**
     * @brief  Get lightSource from MLBinoBusinessManage system.
     * @return a map of lightSource (format: {module id, lightSource}).
     */
    virtual std::map<int, std::string> ML_GetLightSource();

    /**
     * @brief  Set exposure for camera, contain auto and fixed.
     * @param exposure  exposure setting to set, include exposure mode and exposure time
     * (unit: millisecond)
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetExposure(ExposureSetting exposure,
                                  ML::MLColorimeter::OperationMode mode =
                                      ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Get the exposure time from camera.
     * @return a map of the exposure time (format: {module id, exposure time}).
     */
    virtual std::map<int, double> ML_GetExposureTime();

    /**
     * @brief  Set binning for camera.
     * @param binning  binning to set.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetBinning(ML::CameraV2::Binning binning);

    /**
     * @brief  Get the binning from camera.
     * @return a map of the binning (format: {module id, Binning}).
     */
    virtual std::map<int, ML::CameraV2::Binning> ML_GetBinning();

    /**
     * @brief  Set binning mode for camera.
     * @param binningMode  binning mode to set.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetBinningMode(ML::CameraV2::BinningMode binningMode);

    /**
     * @brief  Get the binning mode from camera.
     * @return a map of the binning mode (format: {module id, BinningMode}).
     */
    virtual std::map<int, ML::CameraV2::BinningMode> ML_GetBinningMode();

    /**
     * @brief  Set the pixel format for camera.
     * @param pixelFormat  pixel format to set.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetPixelFormat(ML::CameraV2::MLPixelFormat pixelFormat);

    /**
     * @brief  Get the pixel format from camera.
     * @return a map of the pixel format (format: {module id, MLPixelFormat}).
     */
    virtual std::map<int, ML::CameraV2::MLPixelFormat> ML_GetPixelFormat();

    /**
     * @brief  Capture single image asynchronously.
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_CaptureImageAsync(ML::MLColorimeter::OperationMode mode =
                                            ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Capture single image synchronously.
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_CaptureImageSync(ML::MLColorimeter::OperationMode mode =
                                           ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Get an image after calling ML_CaptureImageAsync() or ML_CaptureImageSync().
     * @return a map of signle channel image for mono camera, or a map a three-channel
     * image for color camera (format: {module id, cv::Mat}).
     */
    virtual std::map<int, cv::Mat> ML_GetImage();

    /**
     * @brief  Get a CaptureData for mono camera.
     * @return a map of CaptureData (format: {module id, CaptureData}).
     */
    virtual std::map<int, ML::MLColorimeter::CaptureData> ML_GetCaptureData();

    /**
     * @brief  Get modules' CaptureData for color camera.
     * @return a map of CaptureData for color camera (map format: {module id, {color
     * filter channel enum, capture data}}).
     */
    virtual std::map<
        int, std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData>>
    ML_GetColorCameraCaptureData();

    /**
     * @brief  Load dark images from exposuretime list of external path.
     * @param path  containing dark images named by exposure time.
     * @return the result contains the message, code and status.
     * @note dark images must named by exposure time.
     */
    virtual Result ML_LoadDarkByExposureTimeList(const char* path);

    /**
     * @brief  Load the corresponding calibration data according to the CalibrationConfig.
     * @param config  config setting to load calibration data.
     * @param mode  operation mode between multiple modules
     * @return the result contains the message, code and status.
     * @note dark images must named by exposure time.
     */
    virtual Result ML_LoadCalibrationData(ML::MLColorimeter::CalibrationConfig config,
                                          ML::MLColorimeter::OperationMode mode =
                                              ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Set CaptureData map to perform calibration process.
     * @param moduleID  select a module to set CaptureData map.
     * @param dataMap  CaptureData map.
     * @param isSubDarkFromList  if set to true, subtract dark image after capturing.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetCaptureDataMap(
        int moduleID,
        std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData> dataMap,
        bool isSubDarkFromList = false);

    /**
     * @brief  Execute calibration process according to the CalibrationConfig.
     * @param config  config setting to do calibration process.
     * @param mode  operation mode between multiple modules.
     * @return the result contains the message, code and status.
     * @note If the parameter isSubDarkFromList is set to true when calling
     * ML_SetCaptureDataMap(). So, when calling ML_Process(), the Dark_flag of the
     * parameter config must be set to false.
     */
    virtual Result ML_Process(ML::MLColorimeter::CalibrationConfig config,
                              ML::MLColorimeter::OperationMode mode =
                                  ML::MLColorimeter::OperationMode::Parallel);

    /**
     * @brief  Get calibration data after calling ML_Process().
     * @param moduleID  select a module to get calibration data.
     * @return the data after calibration process.
     */
    virtual std::map<
        ML::MLColorimeter::CalibrationEnum,
        std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaliProcessData>>
    ML_GetCalibrationData(int moduleID);

    /**
     * @brief  Save calibration data after calling ML_Process().
     * @param caliData  the calibration data to save, get from ML_GetCalibrationData().
     * @param moduleID  select a module to save calibration data.
     * @param saveconfig  save config setting
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SaveCalibrationData(
        std::map<
            ML::MLColorimeter::CalibrationEnum,
            std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaliProcessData>>
            caliData,
        int moduleID, ML::MLColorimeter::SaveDataConfig saveconfig);

 private:
    MLMonoBusinessManage* m_IPD;

    std::map<int, MLMonoBusinessManage*> m_moduleList;
    std::map<int, MLColorimeterAlgorithms*> m_processList;
    std::vector<std::string> m_pathList;
    std::string m_configPath;
};
}  // namespace MLColorimeter
}  // namespace ML

#endif  //  SRC_MLCOLORIMETER_MLBINOBUSINESSMANAGE_H_
