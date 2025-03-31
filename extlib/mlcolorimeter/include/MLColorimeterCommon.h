// Copyright 2024 MLOptic

#ifndef SRC_MLCOLORIMETER_MLCOLORIMETERCOMMON_H_
#define SRC_MLCOLORIMETER_MLCOLORIMETERCOMMON_H_

#include <map>
#include <opencv2/opencv.hpp>
#include <string>

#include "MLCamaraCommon.h"
#include "MLFilterWheelClass.h"

namespace ML {
namespace MLColorimeter {
enum class FFCMethod { Normal = 1, Synthetic = 2 };

enum class FocusMethod { Inverse = 1 };

enum class EyeMode { EYE1 = 1, EYE2 = 2, BOTH = 3 };

enum class MappingMethod { Nearby = 1, LinearInter = 2 };

enum class CalibrationEnum {
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

enum class OperationMode { Parallel = 1, Serial = 2 };

enum class ExposureMode { Fixed = 1, Auto = 2 };

struct ExposureSetting {
    ExposureMode Mode = ExposureMode::Auto;
    double ExposureTime = 0.0;
};

struct RXCombination {
    double Sphere = DBL_MAX;
    double Cylinder = DBL_MAX;
    int Axis = INT_MAX;
};

struct MotionConfig {
    bool Enable = false;
    std::string Key;
    std::string Type;
    std::string Name;
    std::string ConnectAddress;
    std::string ConnectType;
    std::string Port;
    int DeviceID;
    int Axis;
    std::string AxisName;
    std::string HomingMethod;
    double Speed;
    bool IsReverse;
    double SoftwareLimitMax;
    double SoftwareLimitMin;
    double ReferencePosition;
    double FocalLength;
    double FocalPlanesObjectSpace;
};

struct CameraConfig {
    bool Enable = false;
    bool ColourCamera;
    std::string Key;
    std::string ConnectAddress;
    std::string Type;
    std::string Name;
};

struct ModuleConfig {
    bool Enable = false;
    std::string SerialNumber;
    std::string Name;
    std::string Key;
    std::string Aperture;
    EyeMode EyeMode;
    int ID;
    std::map<std::string, MotionConfig> MotionConfig_Map;
    CameraConfig CameraConfig;
    std::map<std::string, ML::MLFilterWheel::MLNDFilterConfiguation> NDFilterConfig_Map;
    ML::MLFilterWheel::MLRXFilterConfiguation RXFilterConfig;
    MotionConfig IPDConfig;
};

typedef struct CaptureData {
    std::string SerialNumber;
    std::string ModuleName;
    std::string Key;
    std::string Aperture;
    std::string LightSource;
    ML::MLFilterWheel::MLFilterEnum NDFilter;
    ML::MLFilterWheel::MLFilterEnum ColorFilter;
    RXCombination MovementRX;
    double VID;
    double ExposureTime;
    cv::Mat Img;
    ML::CameraV2::Binning Binning;
    ML::CameraV2::MLPixelFormat PixelFormat;
    CaptureData() {
        Aperture = "3mm";
        LightSource = "";
        NDFilter = ML::MLFilterWheel::MLFilterEnum::ND0;
        ColorFilter = ML::MLFilterWheel::MLFilterEnum::X;
        VID = 0;
        ExposureTime = 100;
        Binning = ML::CameraV2::Binning::ONE_BY_ONE;
        PixelFormat = ML::CameraV2::MLPixelFormat::MLMono12;
    }
} CaliProcessData;

struct FOVCrop {
    cv::Point2f Center;
    double WidthDegree;
    double HeightDegree;
    // micrometer
    double FocalLength;
    // micrometer
    double PixelSize;
};

struct PreCalibrationData {
    // X, Y, Z color filter map
    std::map<ML::MLFilterWheel::MLFilterEnum, cv::Mat> LightFFCMap;
    // X, Y, Z color filter map
    std::map<ML::MLFilterWheel::MLFilterEnum, cv::Mat> DarkMap;
    // X, Y, Z color filter map
    std::map<ML::MLFilterWheel::MLFilterEnum, cv::Point2d> ColorShiftMap;
    cv::Mat CameraMatrix;
    cv::Mat Coefficient;
    cv::Mat RMatrix;
    // X, Y, Z color filter map
    std::map<ML::MLFilterWheel::MLFilterEnum, cv::Mat> LuminanceKMap;
    ML::MLFilterWheel::MLFilterEnum NDFilter;
    RXCombination RX;
    std::string LightSource;
    std::string Aperture;
    FOVCrop Cropping;
};

struct RXMappingMethod {
    MappingMethod SphMapping = MappingMethod::LinearInter;
    MappingMethod CylMapping = MappingMethod::Nearby;
    MappingMethod AxisMapping = MappingMethod::Nearby;
};

struct RXMappingRule {
    std::vector<double> SphMappingList;
    std::vector<double> CylMappingList;
    std::vector<int> AxisMappingList;
    RXMappingMethod RXMethod;
};

struct FolderRule {
    std::string Rule;
    std::string Suffix;
    RXMappingRule RXRule;
    FFCMethod ffcMethod;
    std::vector<double> Sphere_Lum_Cofficient;
    std::vector<double> Cylinder_Lum_Cofficient;
};

struct ProcessPathConfig {
    std::string InputPath;
    ML::MLFilterWheel::MLFilterEnum NDFilter;
    ML::MLFilterWheel::MLFilterEnum ColorFilter;
    std::string LightSource;
    std::string Aperture;
    RXCombination MovementRX;
};

struct CalibrationConfig {
    std::string InputPath = "";
    std::string Aperture = "3mm";
    ML::MLFilterWheel::MLFilterEnum NDFilter = ML::MLFilterWheel::MLFilterEnum::ND0;
    std::vector<ML::MLFilterWheel::MLFilterEnum> ColorFilterList = {
        ML::MLFilterWheel::MLFilterEnum::X, ML::MLFilterWheel::MLFilterEnum::Y,
        ML::MLFilterWheel::MLFilterEnum::Z};
    RXCombination RX = RXCombination();
    std::string LightSource = std::string();
    bool Dark_Flag = true;
    bool FFC_Flag = true;
    bool ColorShift_Flag = true;
    bool Distortion_Flag = true;
    bool Exposure_Flag = true;
    bool FourColor_Flag = true;
    bool Luminance_Flag = false;
    bool FOVCrop_Flag = false;
};

struct SaveDataConfig {
    std::string SavePath = "D:\\";
    std::string Prefix;
    cv::Rect SaveROI = cv::Rect();
    bool SaveRaw = true;
    bool SaveResult = true;
    bool SaveCalibration = false;
};

struct ThroughFocusConfig {
    double FocusMax = DBL_MAX;
    double FocusMin = DBL_MAX;
    double ReferencePosition = DBL_MAX;
    double FocalLength = DBL_MAX;
    double FocalPlanesObjectSpace = DBL_MAX;
    double RoughStep = DBL_MAX;
    double FineRange = DBL_MAX;
    double FineStep = DBL_MAX;
    double Freq = DBL_MAX;
    double Smooth = DBL_MAX;
    std::vector<cv::Rect> ROIs = std::vector<cv::Rect>();
    bool ChessMode = true;
    bool LpmmUnit = true;
};
}  // namespace MLColorimeter
}  // namespace ML

#endif  // SRC_MLCOLORIMETER_MLCOLORIMETERCOMMON_H_
