// Copyright 2024 MLOptic

#ifndef SRC_MLCOLORIMETER_MLALGORITHMS_H_
#define SRC_MLCOLORIMETER_MLALGORITHMS_H_

#include "MLColorimeterCommon.h"
#include "Result.h"
#include "mlcolorimeter_global.h"

/**
 * @file MLColorimeterAlgorithms.h
 * @brief Defines the calibration process and mtf pipeline class and associated functionality.
 */

class PipeLine;

namespace ML {
namespace MLColorimeter {
class MLColorimeterConfig;

/**
 * @class MLColorimeterAlgorithms
 * @brief Manages the calibration process and mtf pipeline.
 */
class MLCOLORIMETER_EXPORT MLColorimeterAlgorithms {
 public:
    MLColorimeterAlgorithms();
    ~MLColorimeterAlgorithms();

    /**
     * @brief Set config path of calibration process.
     * @param path  calibration profile path.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetConfigPath(const char* path);

    /**
     * @brief Load dark images of exposuretime list from external path.
     * @param path  external dark images path.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_LoadDarkByExposureTimeList(const char* path);

    /**
     * @brief Load calibration profile data.
     * @param config  config setting to load calibration data.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_LoadCalibrationData(ML::MLColorimeter::CalibrationConfig config);

    /**
     * @brief  Set CaptureData map to perform calibration process.
     * @param dataMap  CaptureData map.
     * @param isSubDarkFromList  if set to true, subtract dark image after capturing.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SetCaptureDataMap(
        std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData> dataMap,
        bool isSubDarkFromList = false);

    /**
     * @brief  Get CaptureData map after calling ML_SetCaptureDataMap().
     * @return a map of CaptureData.
     */
    virtual std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData>
    ML_GetCaptureDataMap();

    /**
     * @brief  Execute calibration process according to the CalibrationConfig.
     * @param config  config setting to do calibration process.
     * @return the result contains the message, code and status.
     * @note If the parameter isSubDarkFromList is set to true when calling
     * ML_SetCaptureDataMap(). So, when calling ML_Process(), the Dark_flag of the
     * parameter config must be set to false.
     */
    virtual Result ML_Process(ML::MLColorimeter::CalibrationConfig config);

    /**
     * @brief  Get calibration data after calling ML_Process().
     * @return the data after calibration process.
     */
    virtual std::map<
        ML::MLColorimeter::CalibrationEnum,
        std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaliProcessData>>
    ML_GetCalibrationData();

    /**
     * @brief  Save calibration data after calling ML_Process().
     * @param caliData  the calibration data to save, get from ML_GetCalibrationData().
     * @param saveconfig  save config setting.
     * @return the result contains the message, code and status.
     */
    virtual Result ML_SaveCalibrationData(
        std::map<
            ML::MLColorimeter::CalibrationEnum,
            std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaliProcessData>>
            caliData,
        ML::MLColorimeter::SaveDataConfig saveconfig);

    /**
     * @brief  Get ROI center based on optical axis center and offset.
     * @param image  input an image containing one cross-hair.
     * @param offset  offset between optical axis center and roi center.
     * @return roi center coordinate.
     */
    virtual std::vector<cv::Point> ML_GetCrossCenter(cv::Mat image, int offset);

    /**
     * @brief  Calculate mtf based on one image containing one cross-hair.
     * @param image  input an image containing one cross-hair.
     * @param focusLength  focal length of the module.
     * @param lpmmUnit  if set to true, use lp/mm unit to calculate mtf, else use lp/degree.
     * @param chessMode  if set to true, calculate mtf for chessboard, else for cross-hair.
     * @return a map of curve data, including mtf, lsf, esf.
     */
    virtual std::map<std::string, double*> ML_CalculateMTF(cv::Mat image,
                                                           double focusLength,
                                                           bool lpmmUnit, bool chessMode);

    /**
     * @brief  Clear pipeline cache before calling ML_GetMTFByFreq().
     * @return a map of curve data, including mtf, lsf, esf.
     */
    virtual Result
    ML_ClearMTFPipeline();

    /**
     * @brief  Get mtf curve by freq after calling ML_CalculateMTF().
     * @param freq  input freq to get mtf curve
     * @return the mtf curve.
     */
    virtual std::vector<double> ML_GetMTFByFreq(double freq);

 public:
    virtual double CalculateMTF(cv::Mat image, double freq, double focusLength,
                                bool lpmmUnit = true, bool chessMode = false,
                                int binNum = 1);

    virtual double CalculateStd(cv::Mat img);

    virtual Result Smooth(double* value, int half_window, int len);

    virtual double FindGaussianFitMaxValue(std::vector<cv::Point2f> pts,
                                           std::vector<cv::Point2f>& fitPts);

    // signals:
    //    void sendProcessFailedSignal(QString);

 private:
    virtual bool GaussiantCurveFit(const std::vector<cv::Point2f>& cvRawPointVec, int n,
                                   cv::Mat& A);

 private:
    virtual Result DarkCorrect(ML::MLFilterWheel::MLFilterEnum colorfilter);

    virtual Result FFCCorrect(ML::MLFilterWheel::MLFilterEnum colorfilter,
                              int length = 100, int value = 0);

    virtual Result ColorShiftCorrect(ML::MLFilterWheel::MLFilterEnum colorfilter);

    virtual Result DistortionCorrect(ML::MLFilterWheel::MLFilterEnum colorfilter);

    virtual Result ExposureCorrect(ML::MLFilterWheel::MLFilterEnum colorfilter);

    virtual Result FourColorCalculation();

    virtual Result LuminanceCalculation(ML::MLFilterWheel::MLFilterEnum colorfilter);

    virtual Result FOVCropCorrect(ML::MLFilterWheel::MLFilterEnum colorfilter);

 private:
    MLColorimeterConfig* m_BMConfig = nullptr;
    std::vector<PipeLine*> pipelinevec;
    std::string m_configPath;
    ML::MLColorimeter::PreCalibrationData m_preCaliData;
    std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData>
        m_CaptureData;
    std::map<
        ML::MLColorimeter::CalibrationEnum,
        std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaliProcessData>>
        m_afterCaliDta;
    std::vector<std::string> m_darkList;
    std::vector<double> m_darkETList;
};
}  // namespace MLColorimeter
}  // namespace ML

#endif  // SRC_MLCOLORIMETER_MLALGORITHMS_H_