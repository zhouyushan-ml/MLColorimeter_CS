// Copyright 2024 MLOptic

#ifndef SRC_MLCOLORIMETER_MLCOLORIMETERCONFIG_H_
#define SRC_MLCOLORIMETER_MLCOLORIMETERCONFIG_H_

#include <qdir.h>
#include <qfile.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include <qvector.h>

#include <map>
#include <vector>

#include "MLColorimeterCommon.h"
#include "Result.h"
#include "json.hpp"
#include "mlcolorimeter_global.h"

namespace ML {
namespace MLColorimeter {
using json = nlohmann::ordered_json;

class MLCOLORIMETER_EXPORT MLColorimeterConfig : public QObject {
    Q_OBJECT
 public:
    MLColorimeterConfig(QObject* parent = nullptr);
    ~MLColorimeterConfig();

    bool LoadConfigPath(const char* path,
                        std::vector<std::string> nameList = {".json", ".csv"});

    bool ReadModuleInfo();

    ML::MLColorimeter::ModuleConfig GetModuleInfo();

    bool LoadThroughFocusConfig(const char* path);

    bool ReadThroughFocusConfig();

    ML::MLColorimeter::ThroughFocusConfig GetThroughFocusConfig();

    std::map<std::string, QVector<QStringList>> GetModuleCSVFile();

    bool LoadFolderRule(std::string path,
                        std::vector<std::string> sessionList = {
                            "Movement", "FFC", "ColorShift", "Distortion", "FourColor",
                            "FOVCrop", "FourColorCalibration", "Luminance", "Dark"});

    std::string ProcessPath(std::string session,
                            ML::MLColorimeter::ProcessPathConfig config,
                            std::string RXCombination = "");

    std::string ProcessPath(std::string session,
                            ML::MLColorimeter::ProcessPathConfig config, FolderRule rule,
                            std::string RXCombination = "");

    std::map<std::string, ML::MLColorimeter::FolderRule> GetFolderRule();

 private:
    std::map<std::string, json> m_JsonMap;
    std::map<std::string, QVector<QStringList>> m_CsvMap;
    ML::MLColorimeter::ModuleConfig m_ModuleConfig;
    ML::MLColorimeter::ThroughFocusConfig m_ThroughFocusConfig;
    std::map<std::string, ML::MLColorimeter::FolderRule> m_allFolderRule;
};
}  // namespace MLColorimeter
}  // namespace ML

#endif  // SRC_MLCOLORIMETER_MLCOLORIMETERCONFIG_H_
