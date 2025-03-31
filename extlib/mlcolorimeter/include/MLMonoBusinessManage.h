// Copyright 2024 MLOptic

#ifndef SRC_MLCOLORIMETER_MLMONOBUSINESSMANAGE_H_
#define SRC_MLCOLORIMETER_MLMONOBUSINESSMANAGE_H_

#include <qobject.h>

#include "MLCamaraCommon.h"
#include "MLColorimeterCommon.h"
#include "Result.h"
#include "mlcolorimeter_global.h"

/**
 * @file MLMonoBusinessManage.h
 * @brief Defines the monocular module business logic class and associated functionality.
 */

class ML_addInBase;

namespace ML {
namespace MLFilterWheel {
class MLFilterWheelCallback;
}  // namespace MLFilterWheel

namespace MLColorimeter {
class MLColorimeterConfig;

/**
 * @class MLMonoBusinessManage
 * @brief Manages the hardware control and business logic for monocular module.
 *
 * The class provides a common interface to manage business logic and hardware control
 * including NDFilter, ColorFilter, RXFilter, Motor and Camera.
 */
class MLCOLORIMETER_EXPORT MLMonoBusinessManage : public QObject {
    Q_OBJECT

 public:
    MLMonoBusinessManage(QObject* parent = nullptr);
    ~MLMonoBusinessManage();

    /**
     * @brief Initialize config profile of MLMonoBusinessManage.
     * @param path  path of config profile.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetConfigPath(const char* path);

    /**
     * @brief Get module config path.
     * @return module config path.
     */
    virtual std::string ML_GetConfigPath();

    /**
     * @brief Create a module for MLMonoBusinessManage system.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_CreateModule();

    /**
     * @brief Connect the module created by ML_CreateModule().
     * @return the result contains the message, code and status.
     */
    virtual Result ML_ConnectModule();

    /**
     * @brief DisConnect the module created by ML_CreateModule().
     * @return the result contains the message, code and status.
     */
    virtual Result ML_DisconnectModule();

    /**
     * @brief Destroy the module created by ML_CreateModule().
     * @return the result contains the message, code and status.
     */
    virtual Result ML_DestroyModule();

    /**
     * @brief Check if all members in the module created by ML_CreateModule() are
     * connected.
     * @return true if all are connected, otherwise false.
     */
    virtual bool ML_IsModuleConnect();

    /**
     * @brief Check if all members in the module created by ML_CreateModule() are moving.
     * @return true if one is moving, otherwise false.
     */
    virtual bool ML_IsModuleMotorsMoving();

    /**
     * @brief Wait for all members' movement finish in the module created by
     * ML_CreateModule().
     * @param timeout  time out limit (unit: millisecond).
     * @return the result contains the message, code and status.
     */
    virtual Result ML_WaitForMovingStop(int timeout = 10000);

    /**
     * @brief Stop all movements of module.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_StopModuleMovement();

    /**
     * @brief Get the serial number of the module.
     * @return module's serial number.
     */
    virtual std::string ML_GetModuleSerialNumber();

    /**
     * @brief Get the name of the module.
     * @return  module's name.
     */
    virtual std::string ML_GetModuleName();

    /**
     * @brief Obtain the id of the module.
     * @return module's id.
     */
    virtual int ML_GetModuleID();

    /**
     * @brief Obtain the eyemode of the module.
     * @return module's eyemode.
     */
    virtual ML::MLColorimeter::EyeMode ML_GetModuleEyeMode();

    /**
     * @brief  Switch ND/XYZ filter wheel's channel by enum asynchronously.
     * @param keyName  the key name of FilterWheel, from the config.
     * @param channel  the enum of channel to switch
     * @param cb  callback function
     * @return the result contains the message, code and status.
     */
    virtual Result ML_MoveND_XYZFilterByEnumAsync(
        std::string keyName, ML::MLFilterWheel::MLFilterEnum channel,
        ML::MLFilterWheel::MLFilterWheelCallback* cb = nullptr);

    /**
     * @brief  Switch ND/XYZ filter wheel's channel by enum synchronously.
     * @param keyName  the key name of FilterWheel, from the config.
     * @param channel  the enum of channel to switch
     * @return the result contains the message, code and status.
     */
    virtual Result ML_MoveND_XYZFilterByEnumSync(std::string keyName,
                                                 ML::MLFilterWheel::MLFilterEnum channel);

    /**
     * @brief  Get the channel of ND/XYZ FilterWheel.
     * @param keyName  the key name of FilterWheel, from the config.
     * @return the channel enum of ND/XYZ FilterWheel.
     */
    virtual ML::MLFilterWheel::MLFilterEnum ML_GetND_XYZFilterChannel(
        std::string keyName);

    /**
     * @brief  Switch RX filter wheel's channel by name and change RX axis degree
     * asynchronously.
     * @param channel  the channel to switch
     * @param degree  the degree of axis to change
     * @param cb  callback function
     * @return the result contains the message, code and status.
     */
    virtual Result ML_MoveRXFilterByNameAsync(
        std::string channel, int degree,
        ML::MLFilterWheel::MLFilterWheelCallback* cb = nullptr);

    /**
     * @brief  Switch RX filter wheel's channel by name and change RX axis degree
     * synchronously.
     * @param channel  the channel to switch
     * @param degree  the degree of axis to change
     * @return the result contains the message, code and status.
     */
    virtual Result ML_MoveRXFilterByNameSync(std::string channel, int degree);

    /**
     * @brief  Get the channel of RXFilterWheel.
     * @return RXFilter's channel
     */
    virtual std::string ML_GetRXFilterChannel();

    /**
     * @brief  Get the degree of RXFilterWheel's axis.
     * @return RXFilter's axis degree
     */
    virtual int ML_GetRXFilterAxis();

    /**
     * @brief  Set Focus by vid asynchronously.
     * @param vid  the vid to set (unit: millimeter)
     * @param method  focus method (default: Inverse)
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetFocusAsync(
        double vid,
        ML::MLColorimeter::FocusMethod method = ML::MLColorimeter::FocusMethod::Inverse);

    /**
     * @brief  Set Focus by vid synchronously.
     * @param vid  the vid to set (unit: millimeter)
     * @param method  focus method (default: Inverse)
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetFocusSync(
        double vid,
        ML::MLColorimeter::FocusMethod method = ML::MLColorimeter::FocusMethod::Inverse);

    /**
     * @brief  Get vid (unit: millimeter).
     * @param method  focus method (default: Inverse)
     * @return vid of focus
     */
    virtual double ML_GetVID(
        ML::MLColorimeter::FocusMethod method = ML::MLColorimeter::FocusMethod::Inverse);

    /**
     * @brief  Perform through focus and return the vid and position on best mtf.
     * @param keyName  the key name of Motion to perform through focus, from the config
     * @param VID  the VID on the best mtf
     * @param position  the position on the best mtf
     * @param focusconfig  through focus config, default from ThroughFocus.json
     * @return the result contains the message, code and status.
     */
    virtual Result ML_ThroughFocus(std::string keyName, double& VID, double& position,
                                   ML::MLColorimeter::ThroughFocusConfig focusconfig =
                                       ML::MLColorimeter::ThroughFocusConfig());

    /**
     * @brief  Save through focus result to csv file after calling ML_ThroughFocus().
     * @param filename  file path to save mtf curve.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SaveThroughFocusResult(std::string filename);

    /**
     * @brief  Get fine vid curve after calling ML_ThroughFocus().
     * @return fine vid curve.
     */
    virtual std::vector<double> ML_GetVIDCurve();

    /**
     * @brief  Get fine mtf curve after calling ML_ThroughFocus().
     * @return fine mtf curve.
     */
    virtual std::vector<double> ML_GetMTFCurve();

    /**
     * @brief  Get fine motion curve after calling ML_ThroughFocus().
     * @return fine motion curve.
     */
    virtual std::vector<double> ML_GetMotionCurve();

    /**
     * @brief  Get rough std curve after calling ML_ThroughFocus().
     * @return rough std curve
     */
    virtual std::vector<double> ML_GetRoughstdCurve();

    /**
     * @brief  Get rough vid curve after calling ML_ThroughFocus().
     * @return rough vid curve
     */
    virtual std::vector<double> ML_GetRoughVIDCurve();

    /**
     * @brief  Get rough motion curve after calling ML_ThroughFocus().
     * @return rough motion curve
     */
    virtual std::vector<double> ML_GetRoughMotionCurve();

    /**
     * @brief  Set absolute motion position asynchronously.
     * @param keyName  the key name of Motion, from the config.
     * @param pos  absolute motion position to set (unit: millimeter).
     * @return the result contains the message, code and status
     */
    virtual Result ML_SetPosistionAbsAsync(std::string keyName, double pos);

    /**
     * @brief  Set absolute motion position synchronously.
     * @param keyName  the key name of Motion, from the config.
     * @param pos  absolute motion position to set (unit: millimeter).
     * @return the result contains the message, code and status
     */
    virtual Result ML_SetPosistionAbsSync(std::string keyName, double pos);

    /**
     * @brief  Set relative motion position asynchronously.
     * @param keyName  the key name of Motion, from the config.
     * @param pos  relative motion position to set (unit: millimeter).
     * @return the result contains the message, code and status
     */
    virtual Result ML_SetPositionRelAsync(std::string keyName, double pos);

    /**
     * @brief  Set relative motion position synchronously.
     * @param keyName  the key name of Motion, from the config.
     * @param pos  relative motion position to set (unit: millimeter).
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetPositionRelSync(std::string keyName, double pos);

    /**
     * @brief  Get motion position (unit: millimeter).
     * @param keyName  the key name of Motion, from the config.
     * @return position of keyName motion (unit: millimeter).
     */
    virtual double ML_GetMotionPosition(std::string keyName);

    /**
     * @brief  Stop motion movement.
     * @param keyName  the key name of Motion, from the config.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_StopMotionMovement(std::string keyName);

    /**
     * @brief  Set Spherical power asynchronously.
     * @param sphere  sphere to set.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetSphericalAsync(double sphere);

    /**
     * @brief  Set Spherical power synchronously.
     * @param sphere  sphere to set.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetSphericalSync(double sphere);

    /**
     * @brief  Get Spherical power.
     * @return spherical power.
     */
    virtual double ML_GetSpherical();

    /**
     * @brief  Set RX asynchronously.
     * @param rx  RXCombination setting containing spherical power, cylindrical power and
     * cylindrical axis.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetRXAsync(ML::MLColorimeter::RXCombination rx);

    /**
     * @brief  Set RX synchronously.
     * @param rx  RXCombination setting containing spherical power, cylindrical power and
     * cylindrical axis.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetRXSync(ML::MLColorimeter::RXCombination rx);

    /**
     * @brief  Get RX..
     * @return RXCombination containing spherical power,
     * cylindrical power and cylindrical axis.
     */
    virtual ML::MLColorimeter::RXCombination ML_GetRX();

    /**
     * @brief  Set aperture for MLMonoBusinessManage system.
     * @param aperture  aperture to set, example: 3mm.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetAperture(std::string aperture);

    /**
     * @brief  Get aperture from MLMonoBusinessManage system.
     * @return aperture of MLMonoBusinessManage system.
     */
    virtual std::string ML_GetAperture();

    /**
     * @brief  Set lightSource value for MLMonoBusinessManage system.
     * @param aperture  lightSource to set, example: R/G/B/W.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetLightSource(std::string lightSource);

    /**
     * @brief  Get lightSource from MLMonoBusinessManage system.
     * @return lightSource of MLMonoBusinessManage system.
     */
    virtual std::string ML_GetLightSource();

    /**
     * @brief  Set exposure for camera, contain auto and fixed.
     * @param exposure  exposure setting to set, include exposure mode and exposure time
     * (unit: millisecond)
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetExposure(ExposureSetting exposure);

    /**
     * @brief  Get the exposure time from camera.
     * @return the exposure time of camera.
     */
    virtual double ML_GetExposureTime();

    /**
     * @brief  Set binning for camera.
     * @param binning  binning to set.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetBinning(ML::CameraV2::Binning binning);

    /**
     * @brief  Get the binning from camera.
     * @return the binning of camera.
     */
    virtual ML::CameraV2::Binning ML_GetBinning();

    /**
     * @brief  Set binning mode for camera.
     * @param binningMode  binning mode to set.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetBinningMode(ML::CameraV2::BinningMode binningMode);

    /**
     * @brief  Get the binning mode from camera.
     * @return the binning mode of camera.
     */
    virtual ML::CameraV2::BinningMode ML_GetBinningMode();

    /**
     * @brief  Set the pixel format for camera.
     * @param pixelFormat  pixel format to set.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetPixelFormat(ML::CameraV2::MLPixelFormat pixelFormat);

    /**
     * @brief  Get the pixel format from camera.
     * @return the pixel format of camera.
     */
    virtual ML::CameraV2::MLPixelFormat ML_GetPixelFormat();

    /**
     * @brief  Capture single image asynchronously.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_CaptureImageAsync();

    /**
     * @brief  Capture single image synchronously.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_CaptureImageSync();

    /**
     * @brief  Get an image after calling ML_CaptureImageAsync() or ML_CaptureImageSync().
     * @return a signle channel image for mono camera, or a three-channel image
     * for color camera.
     */
    virtual cv::Mat ML_GetImage();

    /**
     * @brief  Get a CaptureData for mono camera.
     * @return CaptureData contains module,
     * hardware, and camera info.
     */
    virtual ML::MLColorimeter::CaptureData ML_GetCaptureData();

    /**
     * @brief  Get a map of CaptureData object for color camera.
     * @return a map of CaptureData (map format: {color filter channel enum, capture
     * data}).
     */
    virtual std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData>
    ML_GetColorCameraCaptureData();

    /**
     * @brief  Get a ML_addInBase object by name.
     * @param name  the Key field of module, from the config file.
     * @return a ML_addInbase object of the MLMonoBusinessManage system.
     */
    virtual ML_addInBase* ML_GetOneModuleByName(std::string name);

    /**
     * @brief  Get a MLColorimeterConfig object.
     * @return the MLColorimeterConfig object of the MLMonoBusinessManage system..
     */
    virtual MLColorimeterConfig* ML_GetBusinessManageConfig();

 private:
    Result CreateFilterWheel(ML::MLFilterWheel::MLNDFilterConfiguation configuration);

    Result CreateRXFilterWheel(ML::MLFilterWheel::MLRXFilterConfiguation configuration);

    Result CreateMotion(ML::MLColorimeter::MotionConfig config);

    Result CreateCamera(ML::MLColorimeter::CameraConfig config);

    Result ConnectFilterWheel();

    Result ConnectRXFilterWheel();

    Result ConnectMotion();

    Result ConnectCamera();

    Result DisconnectFilterWheel();

    Result DisconnectRXFilterWheel();

    Result DisconnectMotion();

    Result DisconnectCamera();

    bool IsFilterWheelConnect();

    bool IsRXFilterWheelConnect();

    bool IsMotionConnect();

    bool IsCameraConnect();

    bool IsFilterWheelMoving();

    bool IsRXFilterWheelMoving();

    bool IsMotionMoving();

    /**
     * @brief  Set fixed exposure time for camera.
     * @param expTime  exposure time to set (unit: millisecond).
     * @return the result contains the message, code and status.
     */
    virtual Result SetExposureTime(double expTime);

    /**
     * @brief  Set auto exposure time for camera.
     * @param initialTime  initial time of auto exposure (unit: millisecond).
     * @return the result contains the message, code and status.
     */
    virtual Result AutoExposure(double initialTime = 0.0);

 private:
    bool startsWithNumber(std::string str);

 signals:
    void connectStatus(bool);
    void filterWheelStatus(bool);
    void RXFilterWheelStatus(bool);
    void motionStatus(bool);

 private:
    std::map<std::string, ML_addInBase*> m_addInBase;
    MLColorimeterConfig* m_BMConfig = nullptr;
    std::string m_ConfigPath;
    std::map<std::string, ML::MLFilterWheel::MLFilterEnum> m_Filter;
    int m_AxisDeg = 0;
    std::string m_RXFilter = "0d";
    std::string m_Aperture;
    std::string m_LightSource;
    ML::MLColorimeter::RXCombination m_inputRX;
    ML::MLColorimeter::RXCombination m_movementRX;

    std::vector<double> m_roughFocusmtf;
    std::vector<double> m_roughStepArray;
    std::vector<double> m_roughVidArray;
    std::vector<double> m_fineStepArray;
    std::vector<double> m_fineVidArray;
    std::vector<double> m_fineFocusmtf;
};
}  // namespace MLColorimeter
}  // namespace ML

#endif  // SRC_MLCOLORIMETER_MLMONOBUSINESSMANAGE_H_
