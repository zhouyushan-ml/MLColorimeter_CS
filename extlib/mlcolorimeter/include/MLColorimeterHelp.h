// Copyright 2024 MLOptic

#ifndef SRC_MLCOLORIMETER_MLCOLORIMETERHELP_H_
#define SRC_MLCOLORIMETER_MLCOLORIMETERHELP_H_
#include <fstream>
#include <map>
#include <opencv2/opencv.hpp>
#include <string>

#include "MLColorimeterCommon.h"
#include "json.hpp"
#include "mlcolorimeter_global.h"

namespace ML {
namespace MLColorimeter {
using json = nlohmann::ordered_json;

class MLCOLORIMETER_EXPORT MLColorimeterHelp {
 public:
    static MLColorimeterHelp* instance();

    ~MLColorimeterHelp();

    cv::Mat ReadJsonFileToMat(const char* filename, std::string objectName);

    ML::MLColorimeter::FOVCrop ReadJsonToFOVCrop(const char* filename,
                                                 std::string objectName);

    bool WriteMatToJsonFile(const char* filename, std::string objectName, cv::Mat mat);

    ML::MLColorimeter::RXCombination RXMapping(ML::MLColorimeter::RXCombination rx,
                                               ML::MLColorimeter::RXMappingRule rxRule);

    std::string TransRXToStr(ML::MLColorimeter::RXCombination rx);

    std::string TransSynRXToStr(ML::MLColorimeter::RXCombination rx);

    ML::MLColorimeter::RXCombination TransStrToRX(std::string str);

    ML::MLFilterWheel::MLFilterEnum TransStrToFilterEnum(std::string str);

    std::string TransFilterEnumToStr(ML::MLFilterWheel::MLFilterEnum fenum);

    std::string TransCaliEnumToString(ML::MLColorimeter::CalibrationEnum caliEnum);

    std::string TransBinningToString(ML::CameraV2::Binning binning);

    std::string TransMLPixelFormatToString(ML::CameraV2::MLPixelFormat format);

    bool WriteMetaDatatoJson(std::string filename,
                             ML::MLColorimeter::CaliProcessData data);

    cv::Mat CalculateAverageImage(std::vector<cv::Mat> image_list);

    bool GetNeedFile(std::string path, std::string fileType,
                     std::vector<std::string>* fileList);

 private:
    MLColorimeterHelp();

 private:
    static MLColorimeterHelp* m_instance;
};
}  // namespace MLColorimeter
}  // namespace ML

#endif  // SRC_MLCOLORIMETER_MLCOLORIMETERHELP_H_
