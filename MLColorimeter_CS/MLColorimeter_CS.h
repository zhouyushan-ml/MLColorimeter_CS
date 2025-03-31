#pragma once

/************************************************************************/
/* 将C++非托管类进行托管                                                */
/************************************************************************/
#define LX_DLL_CLASS_EXPORTS

#include <memory>
#include <msclr\marshal_cppstd.h>
using namespace msclr::interop;

#include "MLBinoBusinessManage.h"
#include "MLColorimeter.h"
#include "PluginCore.h"
#include "opencv2/opencv.hpp"
#include "ModuleCommon.h"
#include "MLConverters.h"
#include "MLColorimeterCallback.h"

//参数传入默认值的函数：   
//返回值MLMonoBusinessManage*和MLColorimeterAlgorithms*没有实现
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::ComponentModel;
using namespace MLColorimeterCS;
using namespace MLColorimeterCS::NotifyFilterWheelCallback;

namespace MLColorimeterCS {
    namespace Interface
    {
        /// <summary>
        /// Through focus params config.
        /// </summary>
        /// <param name="KeyName">The key name of Motion to perform through focus, from the config</param>
        /// <param name="VID">The VID on the best mtf</param>
        /// <param name="Position">The position on the best mtf</param>
        /// <param name="FocusConfig">Through focus config, default from ThroughFocus.json</param>
        /// <param name="Mode">Operation mode between multiple modules.</param>
        public ref class ThroughFocusParams {
        public:
            property String^ KeyName;
            property Dictionary<int, double>^ VID;
            property Dictionary<int, double>^ Position;
            property MLCommon::ThroughFocusConfig^ FocusConfig;
            property MLCommon::OperationMode Mode;

            ThroughFocusParams(String^ keyName, Dictionary<int, double>^ vid, Dictionary<int, double>^ position) {
                KeyName = keyName;
                VID = vid;
                Position = position;
                FocusConfig = gcnew MLCommon::ThroughFocusConfig();
                Mode = MLCommon::OperationMode::Parallel;
            }
        };

        public ref class MLBinoBusinessModuleWrapper {
        public:
            MLBinoBusinessModuleWrapper(ML::MLColorimeter::MLBinoBusinessManage* nativeModule) {
                ml_bino = nativeModule;
            }

            ~MLBinoBusinessModuleWrapper() {
                delete ml_bino;
            }

            !MLBinoBusinessModuleWrapper() {
                delete ml_bino;
            }

            /// <summary>
            /// Add a monocular businessmanage instance.
            /// </summary>
            /// <param name="path">module's config path.</param>
            /// <returns>The result contains the message, code, and status.</returns>
            MLCommon::MLResult ML_AddModule(String^ path);

            /// <summary>
            /// Remove a monocular businessmanage instance added by ML_AddModule().
            /// </summary>
            /// <param name="moduleID">module's id, from ModuleConfig.json</param>
            /// <returns>The result contains the message, code, and status.</returns>
            MLCommon::MLResult ML_RemoveModule(int moduleID);

            /// <summary>
            /// Connect the module created by ML_CreateModule().
            /// </summary>
            /// <returns>The result contains the message, code, and status.</returns>
            MLCommon::MLResult ML_ConnectModules();

            /// <summary>
            /// Add a ipd module instance.
            /// </summary>
            /// <param name="path">module's config path.</param>
            /// <returns>The result contains the message, code, and status.</returns>
            MLCommon::MLResult ML_AddIPDMotion(String^ path);

            /// <summary>
            /// Remove the ipd module added by ML_AddIPDMotion().
            /// </summary>
            /// <returns>The result contains the message, code, and status.</returns>
            MLCommon::MLResult ML_RemoveIPDMotion();

            /// <summary>
            /// Disconnect and destory modules added by ML_AddModule() and ML_AddIPDMotion().
            /// </summary>
            /// <returns>The result contains the message, code, and status.</returns>
            MLCommon::MLResult ML_DisconnectModules();

            /// <summary>
            /// Check if all module created by ML_CreateModules() are connected.
            /// </summary>
            /// <returns>True if all are connected, otherwise false.</returns>
            bool ML_IsModulesConnect();

            /// <summary>
            /// Check if all modules created by ML_CreateModules() are moving.
            /// </summary>
            /// <returns>True if one is moving, otherwise false.</returns>
            bool ML_IsModulesMoving();

            /// <summary>
            /// Wait for all modules' movement finish.
            /// </summary>
            /// <param name="timeout">Time out limit (unit: millisecond).</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_WaitForMovingStop([Optional, DefaultParameterValue(10000)]int timeout, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Stop all movements of modules.
            /// </summary>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_StopModulesMovement([Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Get the serial number of the modules.
            /// </summary>
            /// <returns>A map of modules' serial number.</returns>
            Dictionary<int, String^>^ ML_GetModulesSerialNumber();

            /// <summary>
            /// Get the name of the modules.
            /// </summary>
            /// <returns>A map of modules' name.</returns>
            Dictionary<int, String^>^ ML_GetModulesName();

            /// <summary>
            /// Get the modules id list.
            /// </summary>
            /// <returns>A list of modules' id.</returns>
            List<int>^ ML_GetModulesIDList();

            /// <summary>
            /// Get the modules number.
            /// </summary>
            /// <returns>The number of module in the MLBinoBusinessManage system.</returns>
            int ML_GetModulesNumber();

            /// <summary>
            /// Get modules' config.
            /// </summary>
            /// <returns>A map of module config of the MLBinoBusinessMange system.</returns>
            Dictionary<int, MLCommon::ModuleConfig^>^ ML_GetModulesConfig();

            /// <summary>
            /// Switch ND/XYZ filter wheel's channel by enum for all modules asynchronously.
            /// </summary>
            /// <param name="keyName">The key name of the module.</param>
            /// <param name="channle">The filter to apply.</param>
            /// <param name="mode">The operation mode.</param>
            /// <returns>The result contains the message, code, and status.</returns>
            MLCommon::MLResult ML_MoveModulesND_XYZFilterAsync(String^ keyName, MLCommon::MLFilterEnum channle, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Move the modules with the specified filter and operation mode.
            /// </summary>
            /// <param name="keyName">The key name of the module.</param>
            /// <param name="channle">The filter to apply.</param>
            /// <param name="mode">The operation mode.</param>
            /// <returns>The result contains the message, code, and status.</returns>
            MLCommon::MLResult ML_MoveModulesND_XYZFilterSync(String^ keyName, MLCommon::MLFilterEnum channle, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Get the channel of ND/XYZ FilterWheel.
            /// </summary>
            /// <param name="keyName">The key name of FilterWheel, from the config.</param>
            /// <returns>A map of Filter channel (format: {module id, enum of channel}).</returns>
            Dictionary<int, MLCommon::MLFilterEnum>^ ML_GetND_XYZFilterChannel(String^ keyName);

            /// <summary>
            /// Switch RX filter wheel's channel by name and change RX axis degree asynchronously.
            /// </summary>
            /// <param name="channelName">The name of channel to switch</param>
            /// <param name="degree">The degree of axis to change</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <param name="cb">Callback function</param>
            /// <returns>The result contains the message, code and status.</returns>
			MLCommon::MLResult ML_MoveModulesRXFilterAsync(String^ channelName, int degree, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Switch RX filter wheel's channel by name and change RX axis degree
            /// </summary>
            /// <param name="channelName">The name of channel to switch</param>
            /// <param name="degree">The degree of axis to change</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_MoveModulesRXFilterSync(String^ channelName, int degree, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Get the channel name of RX FilterWheel.
            /// </summary>
            /// <returns>A map of Filter channel (format: {module id, name of channel}).</returns>
            Dictionary<int, String^>^ ML_GetRXFilterChannel();

            //overloaded function
            /// <summary>
            /// Get the degree of RXFilterWheel's axis.
            /// </summary>
            /// <returns>A map of Filter channel (format: {module id, axis degree}).</returns>
            Dictionary<int, int>^ ML_GetRXFilterAxis();

            /// <summary>
            /// Set Focus by vid asynchronously.
            /// </summary>
            /// <param name="vid">The vid to set (unit: millimeter).</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <param name="method">Focus method (default: Inverse)</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetFocusAsync(double vid, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode, [Optional, DefaultParameterValue(MLCommon::FocusMethod::Inverse)]MLCommon::FocusMethod method);

            /// <summary>
            /// Set Focus by vid synchronously.
            /// </summary>
            /// <param name="vid">The vid to set (unit: millimeter).</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <param name="method">Focus method (default: Inverse)</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetFocusSync(double vid, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode, [Optional, DefaultParameterValue(MLCommon::FocusMethod::Inverse)]MLCommon::FocusMethod method);

            /// <summary>
            /// Get modules' vid (unit: millimeter).
            /// </summary>
            /// <param name="method">Focus method (default: Inverse)</param>
            /// <returns>A map of vid (format: {module id, vid value}).</returns>
			Dictionary<int, double>^ ML_GetVID([Optional, DefaultParameterValue(MLCommon::FocusMethod::Inverse)]MLCommon::FocusMethod method);

            /// <summary>
            /// Perform through focus and return the vid and position on best mtf.
            /// </summary>
            /// <param name="params">All parameter structures of through focus</param>
            MLCommon::MLResult ML_ThroughFocus(ThroughFocusParams^ params);

            /// <summary>
            /// Get fine vid curve after calling ML_ThroughFocus().
            /// </summary>
            /// <returns>A map of fine vid curve (format: {module id, vid curve}).</returns>
            Dictionary<int, List<double>^>^ ML_GetVIDCurve();

            /// <summary>
            /// Get fine mtf curve after calling ML_ThroughFocus().
            /// </summary>
            /// <returns>A map of fine mtf curve (format: {module id, mtf curve}).</returns>
            Dictionary<int, List<double>^>^ ML_GetMTFCurve();

            /// <summary>
            /// Get fine motion curve after calling ML_ThroughFocus().
            /// </summary>
            /// <returns>A map of fine motion curve (format: {module id, motion curve}).</returns>
            Dictionary<int, List<double>^>^ ML_GetMotionCurve();

            /// <summary>
            /// Set absolute motion position asynchronously.
            /// </summary>
            /// <param name="keyName">The key name of Motion, from the config.</param>
            /// <param name="pos">Absolute motion position to set (unit: millimeter).</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status</returns>
            MLCommon::MLResult ML_SetPosistionAbsAsync(String^ keyName, double pos, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Set absolute motion position synchronously.
            /// </summary>
            /// <param name="keyName">The key name of Motion, from the config.</param>
            /// <param name="pos">Absolute motion position to set (unit: millimeter).</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status</returns>
            MLCommon::MLResult ML_SetPosistionAbsSync(String^ keyName, double pos, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Set relative motion position asynchronously.
            /// </summary>
            /// <param name="keyName">The key name of Motion, from the config.</param>
            /// <param name="pos">Absolute motion position to set (unit: millimeter).</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status</returns>
            MLCommon::MLResult ML_SetPositionRelAsync(String^ keyName, double pos, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Set relative motion position synchronously.
            /// </summary>
            /// <param name="keyName">The key name of Motion, from the config.</param>
            /// <param name="pos">Absolute motion position to set (unit: millimeter).</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status</returns>
            MLCommon::MLResult ML_SetPositionRelSync(String^ keyName, double pos, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);
            
            /// <summary>
            /// Get motion position (unit: millimeter).
            /// </summary>
            /// <param name="keyName">The key name of Motion, from the config.</param>
            /// <returns>A map of position for keyName motion (format: {module id, position}).</returns>
            Dictionary<int, double>^ ML_GetMotionPosition(String^ keyName);

            /// <summary>
            /// Stop motion movement.
            /// </summary>
            /// <param name="keyName">The key name of Motion, from the config.</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_StopMotionMovement(String^ keyName, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);
            
            /// <summary>
            /// Set Spherical power asynchronously.
            /// </summary>
            /// <param name="sphere">Sphere to set.</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetSphericalAsync(double sphere, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);
            
            /// <summary>
            /// Set Spherical power synchronously.
            /// </summary>
            /// <param name="sphere">Sphere to set.</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetSphericalSync(double sphere, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Get Spherical power.
            /// </summary>
            /// <returns>A map of spherical power (format: {module id, spherical value}).</returns>
            Dictionary<int, double>^ ML_GetSpherical();

            /// <summary>
            /// Set RX asynchronously.
            /// </summary>
            /// <param name="rx">RXCombination setting containing spherical power, cylindrical power and cylindrical axis.</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetRXAsync(MLCommon::RXCombination^ rx, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);
            
            /// <summary>
            /// Set RX synchronously.
            /// </summary>
            /// <param name="rx">RXCombination setting containing spherical power, cylindrical power and cylindrical axis.</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetRXSync(MLCommon::RXCombination^ rx, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);

            /// <summary>
            /// Get RX.
            /// </summary>
            /// <returns>A map of RXCombination (format: {module id, RXCombination}).</returns>
            Dictionary<int, MLCommon::RXCombination^>^ ML_GetRX();

            /// <summary>
            /// Set interpupillary distance for binocular module asynchronously.
            /// </summary>
            /// <param name="ipd">The interpupillary distance to set (unit: millimeter).</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetIPDAsync(double ipd);
            
            /// <summary>
            /// Set interpupillary distance for binocular system synchronously.
            /// </summary>
            /// <param name="ipd">The interpupillary distance to set (unit: millimeter).</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetIPDSync(double ipd);

            /// <summary>
            /// Get the interpupillary distance.
            /// </summary>
            /// <returns>The interpupillary distance of binocular system.</returns>
            double ML_GetIPD();

            /// <summary>
            /// Set aperture for MLBinoBusinessManage system.
            /// </summary>
            /// <param name="aperture">Aperture to set, example: 3mm.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetAperture(String^ aperture);

            /// <summary>
            /// Get aperture from MLBinoBusinessManage system.
            /// </summary>
            /// <returns>A map of aperture (format: {module id, aperture}).</returns>
            Dictionary<int, String^>^ ML_GetAperture();

            /// <summary>
            /// Set lightSource value for MLBinoBusinessManage system.
            /// </summary>
            /// <param name="lightSource">lightSource to set, example: R/G/B/W.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetLightSource(String^ lightSource);

            /// <summary>
            /// Get lightSource from MLBinoBusinessManage system.
            /// </summary>
            /// <returns>A map of lightSource (format: {module id, lightSource}).</returns>
            Dictionary<int, String^>^ ML_GetLightSource();

            /// <summary>
            /// Set exposure for camera, contain auto and fixed.
            /// </summary>
            /// <param name="exposure">Exposure setting to set, include exposure mode and exposure time(unit: millisecond)</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetExposure(MLCommon::ExposureSetting exposure, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);
            
            /// <summary>
            /// Get the exposure time from camera.
            /// </summary>
            /// <returns>A map of the exposure time (format: {module id, exposure time}).</returns>
            Dictionary<int, double>^ ML_GetExposureTime();

            /// <summary>
            /// Set binning for camera.
            /// </summary>
            /// <param name="binning">Binning to set.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetBinning(MLCommon::Binning binning);

            /// <summary>
            /// Get the binning from camera.
            /// </summary>
            /// <returns>A map of the binning (format: {module id, Binning}).</returns>
            Dictionary<int, MLCommon::Binning>^ ML_GetBinning();

            /// <summary>
            /// Set binning mode for camera.
            /// </summary>
            /// <param name="binningMode">Binning mode to set.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetBinningMode(MLCommon::BinningMode binningMode);

            /// <summary>
            /// Get the binning mode from camera.
            /// </summary>
            /// <returns>A map of the binning mode (format: {module id, BinningMode}).</returns>
            Dictionary<int, MLCommon::BinningMode>^ ML_GetBinningMode();

            /// <summary>
            /// Set the pixel format for camera.
            /// </summary>
            /// <param name="pixelFormat">Pixel format to set.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetPixelFormat(MLCommon::MLPixelFormat pixelFormat);

            /// <summary>
            /// Get the pixel format from camera.
            /// </summary>
            /// <returns>A map of the pixel format (format: {module id, MLPixelFormat}).</returns>
            Dictionary<int, MLCommon::MLPixelFormat>^ ML_GetPixelFormat();

            /// <summary>
            /// Capture single image asynchronously.
            /// </summary>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_CaptureImageAsync([Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);
            
            /// <summary>
            /// Capture single image synchronously.
            /// </summary>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_CaptureImageSync([Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);
            
            /// <summary>
            /// Get an image after calling ML_CaptureImageAsync() or ML_CaptureImageSync().
            /// </summary>
            /// <returns>A map of signle channel image for mono camera, or a map a three-channel image for color camera (format: {module id, cv::Mat}).</returns>
            Dictionary<int, IntPtr>^ ML_GetImage();

            /// <summary>
            /// Get a CaptureData for mono camera.
            /// </summary>
            /// <returns>A map of CaptureData (format: {module id, CaptureData}).</returns>
            Dictionary<int, MLCommon::CaptureData^>^ ML_GetCaptureData();

            /// <summary>
            /// Get modules' CaptureData for color camera.
            /// </summary>
            /// <returns>A map of CaptureData for color camera (map format: {module id, {color filter channel enum, capture data}}).</returns>
			Dictionary<int, Dictionary<MLCommon::MLFilterEnum, MLCommon::CaptureData^>^>^ML_GetColorCameraCaptureData();

            /// <summary>
            /// Load dark images from exposuretime list of external path.
            /// </summary>
            /// <param name="path">Containing dark images named by exposure time.</param>
            /// <returns>The result contains the message, code and status.</returns>
            /// <note>Dark images must named by exposure time.</note>
            MLCommon::MLResult ML_LoadDarkByExposureTimeList(String^ path);

            /// <summary>
            /// Load the corresponding calibration data according to the CalibrationConfig.
            /// </summary>
            /// <param name="config">Config setting to load calibration data.</param>
            /// <param name="mode">Operation mode between multiple modules</param>
            /// <returns>The result contains the message, code and status.</returns>
            /// <note>Dark images must named by exposure time.</note>
            MLCommon::MLResult ML_LoadCalibrationData(MLCommon::CalibrationConfig^ config, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);
            
            /// <summary>
            /// Set CaptureData map to perform calibration process.
            /// </summary>
            /// <param name="moduleID">Select a module to set CaptureData map.</param>
            /// <param name="dataMap">CaptureData map.</param>
            /// <param name="isSubDarkFromList">If set to true, subtract dark image after capturing.</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SetCaptureDataMap(
                int moduleID,
                Dictionary<MLCommon::MLFilterEnum, MLCommon::CaptureData^>^ dataMap,
                [Optional, DefaultParameterValue(false)]bool isSubDarkFromList);

            /// <summary>
            /// Execute calibration process according to the CalibrationConfig.
            /// </summary>
            /// <param name="config">Config setting to do calibration process.</param>
            /// <param name="mode">Operation mode between multiple modules.</param>
            /// <returns>The result contains the message, code and status.</returns>
            /// <note>If the parameter isSubDarkFromList is set to true when calling ML_SetCaptureDataMap(). So, when calling ML_Process(), the Dark_flag of the parameter config must be set to false.</note>
            MLCommon::MLResult ML_Process(MLCommon::CalibrationConfig^ config, [Optional, DefaultParameterValue(MLCommon::OperationMode::Parallel)]MLCommon::OperationMode mode);
            
            /// <summary>
            /// Get calibration data after calling ML_Process().
            /// </summary>
            /// <param name="moduleID">Select a module to get calibration data.</param>
            /// <returns>The data after calibration process.</returns>
            Dictionary<MLCommon::CalibrationEnum,Dictionary<MLCommon::MLFilterEnum, MLCommon::CaliProcessData^>^>^ ML_GetCalibrationData(int moduleID);
            
            /// <summary>
            /// Save calibration data after calling ML_Process().
            /// </summary>
            /// <param name="caliData">The calibration data to save, get from ML_GetCalibrationData().</param>
            /// <param name="moduleID">Select a module to save calibration data.</param>
            /// <param name="saveconfig">Save config setting</param>
            /// <returns>The result contains the message, code and status.</returns>
            MLCommon::MLResult ML_SaveCalibrationData(
                Dictionary<MLCommon::CalibrationEnum,
                Dictionary<MLCommon::MLFilterEnum, MLCommon::CaliProcessData^>^>^
                caliData,
                int moduleID, MLCommon::SaveDataConfig^ saveconfig);

            array<Byte>^ GetImageByte();

        private:
            ML::MLColorimeter::MLBinoBusinessManage* ml_bino = nullptr;
        };

        public ref class MLColorimeterModuleWrapper {
        public:
            MLColorimeterModuleWrapper(ML::MLColorimeter::MLColorimeter* nativeModule) {
                ml_colorimeter = nativeModule;
            }

            ~MLColorimeterModuleWrapper() {
                delete ml_colorimeter;
            }

            //!MLColorimeterModuleWrapper() {
            //    delete ml_colorimeter;
            //}

            /// <summary>
            /// Get the business manage module wrapper.
            /// </summary>
            /// <returns>The business manage module wrapper.</returns>
            MLBinoBusinessModuleWrapper^ GetBusinessManageModule();

            MLCommon::MLResult ML_Measurement(String^ ndKey, String^ xyzKey,
                MLCommon::CalibrationConfig^ config,
                MLCommon::ExposureSetting exposure,
                bool isColorCamera,
                MLCommon::OperationMode mode);

        private:
            ML::MLColorimeter::MLColorimeter* ml_colorimeter = nullptr;
        };

        public ref class MLColorimeterWrapper
        {
        public:
            MLColorimeterWrapper();
            ~MLColorimeterWrapper();

        public:
            /// <summary>
            /// Get the colorimeter module wrapper.
            /// </summary>
            /// <returns>The colorimeter module wrapper.</returns>
            MLColorimeterModuleWrapper^ GetMLColorimeterInstance();
        };
    }
}
//#endif // MLCOLORIMETERWRAPPER_H
