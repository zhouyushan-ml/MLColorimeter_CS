#pragma once

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace System::Drawing;

namespace MLColorimeterCS {
    namespace MLCommon {

        public enum class MLFilterStatus {
            OK,
            Connecting,
            Close,
            Moving,
            Stationary,
            Alarm,
            Error,
            SerialException,
            Timeout,
            Unknown,
            NotEnable
        };

        /// <summary>
        /// This enumeration is used to specify the camera status.
        /// </summary>
        public enum class MLCameraState {
            kIsOpen,

            kIsGrabbing,

            kDisconnected,

            kConnected,

            kError

        };

        /// <summary>
        /// This enumeration is used to specify the camera pixel format.
        /// </summary>
        public enum class MLPixelFormat {
            MLMono8 = 0,  // 8 bits per pixel

            MLMono10,

            MLMono12,  // 12 bits per pixel

            MLMono16,  // 16 bits per pixel

            MLRGB24,  // RGB data format

            MLBayer = 10,

            MLBayerGB8,

            MLBayerGB10,

            MLBayerGB12,
        };

        /// <summary>
        /// This enumeration is used to specify the motion state.
        /// </summary>
        public enum class MLMotionState {
            kMLStatusStationary = 4,

            kMLStatusIsMoving = 3,

            kMLStatusConnected = 2,

            /** @brief operate successfully */
            kMLStatusOk = 1,

            /** @brief operation failure */
            kMLStatusFailed = 0,

            kMLStatusDisConnected = -1,

            /** @brief unknown error */
            kMLStatusUnknow = -2,
        };

        /// <summary>
        /// Filter Wheel Callback Interface
        /// </summary>
        public interface class IMLFilterWheelCallback {
            void NotifyFilterStatusChanged(String^ object, MLFilterStatus status);
        };

        /// <summary>
        /// Camera Callback Interface
        /// </summary>
        public interface class IMLCameraCallback
        {
            void NotifyCameraStateChanged(MLCameraState old_state, MLCameraState new_state);
            void NotifyCameraFrameReceived(IntPtr frame, MLPixelFormat format);
            void NotifyCameraGrayLevel(int gray_level);
        };

        /// <summary>
        /// Motion Callback Interface
        /// </summary>
        public interface class IMLMotionCallback
        {
            void NotifyMotionStateChanged(MLMotionState old_state, MLMotionState new_state);
            void NotifyMotionPosition(int position);
        };

        /// <summary>
        /// 操作结果
        /// </summary>
        [StructLayout(LayoutKind::Sequential)]
        public value struct MLResult {
            /// <summary>
            /// 是否成功
            /// </summary>
            property bool IsSuccess;

            /// <summary>
            /// 错误信息
            /// </summary>
            property String^ ErrorMsg;

            /// <summary>
            /// 错误代码：0 (运行错误); 1 (手动停止)
            /// </summary>
            property int ErrorCode;

            /// <summary>
            /// 默认构造函数
            /// </summary>
            MLResult(bool success, String^ errorMsg, int errorCode) {
                IsSuccess = success;
                ErrorMsg = errorMsg;
                ErrorCode = errorCode;
            }

            /// <summary>
            /// 创建成功结果
            /// </summary>
            static MLResult CreateSuccess() {
                return MLResult(true, "", 0);
            }

            /// <summary>
            /// 创建错误结果
            /// </summary>
            static MLResult CreateError(String^ message, int code) {
                return MLResult(false, message, code);
            }

            /// <summary>
            /// 转换为字符串
            /// </summary>
            virtual String^ ToString() override {
                return String::Format("IsSuccess: {0}, Error: {1} (Code: {2})",
                    IsSuccess, ErrorMsg, ErrorCode);
            }
		};

        /// <summary>
        /// 相机Binning模式
        /// </summary>
        public enum class Binning {
            /// <summary>
            /// 1x1 binning
            /// </summary>
            ONE_BY_ONE = 0,

            /// <summary>
            /// 2x2 binning
            /// </summary>
            TWO_BY_TWO = 1,

            /// <summary>
            /// 4x4 binning
            /// </summary>
            FOUR_BY_FOUR = 2
        };

        /// <summary>
        /// This enumeration is used to specify the camera binning mode.
        /// </summary>
        public enum class BinningMode {
            SUM = 0,
            AVERAGE = 1,
        };

        // MLColorimeterCommon.h enums
        public enum class FFCMethod {
            Normal = 1,
            Synthetic = 2
        };

        public enum class FocusMethod {
            Inverse = 1
        };

        public enum class EyeMode {
            EYE1 = 1,
            EYE2 = 2,
            BOTH = 3
        };

        public enum class MappingMethod {
            Nearby = 1,
            LinearInter = 2
        };

        public enum class CalibrationEnum {
            Raw = 1,
            FFC,
            ColorShift,
            Distortion,
            FourColor,
            Luminance,
            Result,
            CIEResult,
            FOVCrop
        };

        public enum class OperationMode {
            Parallel = 1,
            Serial = 2
        };

        public enum class ExposureMode {
            Fixed = 1,
            Auto = 2
        };

        // MLFilterWheelClass.h enums
        public enum class MLFilterEnum {
            X = 1,
            Y = 2,
            Z,
            ND0,
            ND1,
            ND2,
            ND3,
            ND4,
            ND5,
            Clear,
            Block,
            Customer1,
            Customer2,
            Customer3,
            Customer4,
            Customer5,
            Customer6,
            Customer7,
            Customer8,
            Customer9,
            Customer10,
            Unknown
        };

        // Basic structures
        [StructLayout(LayoutKind::Sequential)]
        public value struct ExposureSetting {
            property ExposureMode Mode;
            property double ExposureTime;

            ExposureSetting(ExposureMode mode, double time) {
                Mode = mode;
                ExposureTime = time;
            }
        };

        //[StructLayout(LayoutKind::Sequential)]
        //public value struct RXCombination {
        //    property double Sphere;
        //    property double Cylinder;
        //    property int Axis;

        //    RXCombination(double sphere, double cylinder, int axis) {
        //        Sphere = sphere;
        //        Cylinder = cylinder;
        //        Axis = axis;
        //    }

        //    /// <summary>
        //    ///  Structure initialization 
        //    /// </summary>
        //    static RXCombination Init() {
        //        return RXCombination(double::MaxValue, double::MaxValue, int::MaxValue);
        //    }
        //};

        public ref class RXCombination {
        public:
            property double Sphere;
            property double Cylinder;
            property int Axis;

            RXCombination() {
                Sphere = double::MaxValue;
                Cylinder = double::MaxValue;
                Axis = int::MaxValue;
            }
        };

        // FilterWheel structures
        public ref class MLIOCommand {
        public:
            property int Start;
            property int ZHome;
            property int Stop;
            property int AlarmRst;

            MLIOCommand() {
                Start = 3;
                ZHome = 4;
                Stop = 5;
                AlarmRst = 7;
            }
        };

        public ref class MLAxisInfo {
        public:
            property bool Enable;
            property String^ Name;
            property String^ MotorType;
            property int Station;
            property bool MLIOCommand_Enable;
            property MLIOCommand^ Command;
            property int PulsePerCycle;
            property int Axis_Index;
            property int Min;
            property int Max;

            MLAxisInfo() {
                Enable = true;
                Name = "Axis";
                MotorType = "Oriental/AZ";
                Station = 4;
                MLIOCommand_Enable = false;
                Command = gcnew MLIOCommand();
                PulsePerCycle = 28333;
                Axis_Index = 0;
                Min = 0;
                Max = 360;
            }
        };

        public ref class MLSerialInfo {
        public:
            property int Baudrate;
            property int ByteSize;
            property String^ Parity;
            property String^ StopBits;
            property String^ FlowControl;

            MLSerialInfo() {
                Baudrate = 115200;
                ByteSize = 8;
                Parity = "even";
                StopBits = "onestop";
                FlowControl = "none";
            }
        };

        // Configuration structures
        public ref class MotionConfig {
        public:
            property bool Enable;
            property String^ Key;
            property String^ Type;
            property String^ Name;
            property String^ ConnectAddress;
            property String^ ConnectType;
            property String^ Port;
            property int DeviceID;
            property int Axis;
            property String^ AxisName;
            property String^ HomingMethod;
            property double Speed;
            property bool IsReverse;
            property double SoftwareLimitMax;
            property double SoftwareLimitMin;
            property double ReferencePosition;
            property double FocalLength;
            property double FocalPlanesObjectSpace;
        };

        public ref class CameraConfig {
        public:
            property bool Enable;
            property bool ColourCamera;
            property String^ Key;
            property String^ ConnectAddress;
            property String^ Type;
            property String^ Name;

            CameraConfig() {
                Enable = false;
                ColourCamera;
                Key = "";
                ConnectAddress = "";
                Type = "";
                Name = "";
            }
        };

        /// <summary>
/// 相机采集数据结构
/// </summary>
        [StructLayout(LayoutKind::Sequential)]
        public ref class CaptureData {
        public:
            property String^ SerialNumber;
            property String^ ModuleName;
            property String^ Key;
            property String^ Aperture;
            property String^ LightSource;
            property MLFilterEnum NDFilter;
            property MLFilterEnum ColorFilter;
            property RXCombination^ MovementRX;
            property double VID;
            property double ExposureTime;
            property Bitmap^ Img;
            property Binning Bining;
            property MLPixelFormat PixelFormat;

            CaptureData() {
                SerialNumber = "";
                ModuleName = "";
                Key = "";
                Aperture = "3mm";
                LightSource = "";
                NDFilter = MLFilterEnum::ND0;
                ColorFilter = MLFilterEnum::X;
                MovementRX = gcnew RXCombination();
                VID = 0.0;
                ExposureTime = 100.0;
                Img = nullptr;
                Bining = Binning::ONE_BY_ONE;
                PixelFormat = MLPixelFormat::MLMono12;
            }
        };

        using CaliProcessData = CaptureData;

        /// <summary>
/// ND滤光片配置
/// </summary>
        public ref class MLNDFilterConfiguation {
        public:
            bool Enable;
            String^ Type;
            String^ Name;
            String^ Protocol;
            String^ MotorType;
            String^ Port;
            int Station;
            MLSerialInfo^ SerialInfo;
            Dictionary<String^, int>^ PositionName_List;
            Dictionary<MLFilterEnum, int>^ PositionEnum_List;
            bool MLIOCommand_Enable;
            MLIOCommand^ Command;

            MLNDFilterConfiguation() {
                Enable = false;
                Type = "FilterWheel";
                Name = "ND";
                Protocol = "modbus/serial";
                MotorType = "Oriental/AZ";
                Station = 1;
                SerialInfo = gcnew MLSerialInfo();
                PositionName_List = gcnew Dictionary<String^, int>();
                PositionEnum_List = gcnew Dictionary<MLFilterEnum, int>();
                MLIOCommand_Enable = false;
                Command = gcnew MLIOCommand();
            }
        };

        /// <summary>
        /// RX滤光片配置
        /// </summary>
        public ref class MLRXFilterConfiguation {
        public:
            property bool Enable;
            property String^ Type;
            property String^ Name;
            property String^ Protocol;
            property String^ MotorType;
            property String^ Port;
            property int Station;
            property MLSerialInfo^ SerialInfo;
            property Dictionary<String^, int>^ PositionName_List;
            property bool MLIOCommand_Enable;
            property MLIOCommand^ Command;
            property MLAxisInfo^ AxisInfo;

            MLRXFilterConfiguation() {
                Enable = false;
                Type = "RXFilterWheel";
                Name = "RX Filter";
                Protocol = "modbus/serial";
                MotorType = "Oriental/AZ";
                Station = 3;
                SerialInfo = gcnew MLSerialInfo();
                PositionName_List = gcnew Dictionary<String^, int>();
                MLIOCommand_Enable = false;
                Command = gcnew MLIOCommand();
                AxisInfo = gcnew MLAxisInfo();
            }
        };

        public ref class ModuleConfig {
        public:
            property bool Enable;
            property String^ SerialNumber;
            property String^ Name;
            property String^ Key;
            property String^ Aperture;
            property EyeMode EyeMode;
            property int ID;
            property Dictionary<String^, MotionConfig^>^ MotionConfig_Map;
            property CameraConfig^ CameraConfig;
            property Dictionary<String^, MLNDFilterConfiguation^>^ NDFilterConfig_Map;
            property MLRXFilterConfiguation^ RXFilterConfig;
            property MotionConfig^ IPDConfig;
        };

        /// <summary>
/// 滤光片结果
/// </summary>
        public ref class MLFilterResult {
        public:
            property bool Code;
            property MLFilterStatus Status;
            property String^ Message;

            MLFilterResult() {
                Code = false;
                Status = MLFilterStatus::Unknown;
                Message = "";
            }
        };

        public ref class MLPoint2F
        {
        private:
            System::Drawing::PointF point;

        public:
            MLPoint2F(System::Drawing::PointF point) : point(point) {}
        };

        public ref class FOVCrop {
        public:
            property MLPoint2F^ Center;
            property double WidthDegree;
            property double HeightDegree;
            property double FocalLength;
            property double PixelSize;
        };

        // Calibration structures
        public ref class PreCalibrationData {
        public:
            property Dictionary<MLFilterEnum, Bitmap^>^ LightFFCMap;
            property Dictionary<MLFilterEnum, Bitmap^>^ DarkMap;
            property Dictionary<MLFilterEnum, MLPoint2F^>^ ColorShiftMap;
            property Bitmap^ CameraMatrix;
            property Bitmap^ Coefficient;
            property Bitmap^ RMatrix;
            property Dictionary<MLFilterEnum, Bitmap^>^ LuminanceKMap;
            property MLFilterEnum NDFilter;
            property RXCombination^ RX;
            property String^ LightSource;
            property String^ Aperture;
            property FOVCrop^ Cropping;
        };

        public ref class CalibrationConfig {
        public:
            property String^ InputPath;
            property String^ Aperture;
            property MLFilterEnum NDFilter;
            property List<MLFilterEnum>^ ColorFilterList;
            property RXCombination^ RX;
            property String^ LightSource;
            property bool Dark_Flag;
            property bool FFC_Flag;
            property bool ColorShift_Flag;
            property bool Distortion_Flag;
            property bool Exposure_Flag;
            property bool FourColor_Flag;
            property bool Luminance_Flag;
            property bool FOVCrop_Flag;

            CalibrationConfig() {
                InputPath = "";
                Aperture = "3mm";
                NDFilter = MLFilterEnum::ND0;
                ColorFilterList = gcnew List<MLFilterEnum>();
                ColorFilterList->Add(MLFilterEnum::X);
                ColorFilterList->Add(MLFilterEnum::Y);
                ColorFilterList->Add(MLFilterEnum::Z);
                RX = gcnew RXCombination();
                LightSource = "";
                Dark_Flag = true;
                FFC_Flag = true;
                ColorShift_Flag = true;
                Distortion_Flag = true;
                Exposure_Flag = true;
                FourColor_Flag = true;
                Luminance_Flag = false;
                FOVCrop_Flag = false;
            }
        };

        [StructLayout(LayoutKind::Sequential)]
        public value struct RXMappingMethod {
        public:
            property MappingMethod SphMapping;
            property MappingMethod CylMapping;
            property MappingMethod AxisMapping;

            RXMappingMethod(MappingMethod sph, MappingMethod cyl, MappingMethod axis) {
                SphMapping = sph;
                CylMapping = cyl;
                AxisMapping = axis;
            }
        };

        // 然后修正 RXMappingRule 的定义
        [StructLayout(LayoutKind::Sequential)]
        public ref class RXMappingRule {
        public:
            property List<double>^ SphMappingList;
            property List<double>^ CylMappingList;
            property List<int>^ AxisMappingList;
            property RXMappingMethod RXMethod;

            RXMappingRule() {
                SphMappingList = gcnew List<double>();
                CylMappingList = gcnew List<double>();
                AxisMappingList = gcnew List<int>();
                RXMethod = RXMappingMethod(
                    MappingMethod::LinearInter,
                    MappingMethod::Nearby,
                    MappingMethod::Nearby
                );
            }
        };

        /// <summary>
        /// 文件夹规则
        /// </summary>
        [StructLayout(LayoutKind::Sequential)]
        public ref class FolderRule {
        public:
            property String^ Rule;
            property String^ Suffix;
            property RXMappingRule^ RXRule;
            property FFCMethod FFCMethod_;
            property List<double>^ Sphere_Lum_Coefficient;
            property List<double>^ Cylinder_Lum_Coefficient;

            FolderRule() {
                Rule = "";
                Suffix = "";
                RXRule = gcnew RXMappingRule();
                FFCMethod_ = FFCMethod::Normal;
                Sphere_Lum_Coefficient = gcnew List<double>();
                Cylinder_Lum_Coefficient = gcnew List<double>();
            }
        };

        /// <summary>
        /// 处理路径配置
        /// </summary>
        [StructLayout(LayoutKind::Sequential)]
        public ref class ProcessPathConfig {
        public:
            property String^ InputPath;
            property MLFilterEnum NDFilter;
            property MLFilterEnum ColorFilter;
            property String^ LightSource;
            property String^ Aperture;
            property RXCombination^ MovementRX;

            ProcessPathConfig() {
                InputPath = "";
                NDFilter = MLFilterEnum::ND0;
                ColorFilter = MLFilterEnum::X;
                LightSource = "";
                Aperture = "3mm";
                MovementRX = gcnew RXCombination();
            }
        };

        /// <summary>
        /// 表示一个矩形区域
        /// </summary>
        [StructLayout(LayoutKind::Sequential)]
        public value struct Rect {
        public:
            /// <summary>
            /// X坐标
            /// </summary>
            property int X;

            /// <summary>
            /// Y坐标
            /// </summary>
            property int Y;

            /// <summary>
            /// 宽度
            /// </summary>
            property int Width;

            /// <summary>
            /// 高度
            /// </summary>
            property int Height;

            /// <summary>
            /// 默认构造函数
            /// </summary>
            Rect(int x, int y, int width, int height) {
                X = x;
                Y = y;
                Width = width;
                Height = height;
            }
        };

        /// <summary>
        /// 保存数据配置
        /// </summary>
        [StructLayout(LayoutKind::Sequential)]
        public ref class SaveDataConfig {
        public:
            property String^ SavePath;
            property String^ Prefix;
            property Rect SaveROI;
            property bool SaveRaw;
            property bool SaveResult;
            property bool SaveCalibration;
            SaveDataConfig() {
                SavePath = "D:\\";
                Prefix = "";
                SaveROI = Rect();
                SaveRaw = true;
                SaveResult = true;
                SaveCalibration = false;
            }
        };

        /// <summary>
        /// 通过焦点配置
        /// </summary>
        [StructLayout(LayoutKind::Sequential)]
        public ref class ThroughFocusConfig {
        public:
            property double FocusMax;
            property double FocusMin;
            property double ReferencePosition;
            property double FocalLength;
            property double FocalPlanesObjectSpace;
            property double RoughStep;
            property double FineRange;
            property double FineStep;
            property double Freq;
            property double Smooth;
            property List<Rect>^ ROIs;
            property bool ChessMode;
            property bool LpmmUnit;

            ThroughFocusConfig() {
                FocusMax = Double::MaxValue;
                FocusMin = Double::MaxValue;
                ReferencePosition = Double::MaxValue;
                FocalLength = Double::MaxValue;
                FocalPlanesObjectSpace = Double::MaxValue;
                RoughStep = Double::MaxValue;
                FineRange = Double::MaxValue;
                FineStep = Double::MaxValue;
                Freq = Double::MaxValue;
                Smooth = Double::MaxValue;
                ROIs = gcnew List<Rect>();
                ChessMode = true;
                LpmmUnit = true;
            }
        };
    }
}