#include "pch.h"

#include "MLColorimeter_CS.h"
#include <mutex>

#include <msclr\marshal_cppstd.h>

namespace MLColorimeterCS {
	namespace Interface
	{
		MLColorimeterWrapper::MLColorimeterWrapper()
		{
		}

		MLColorimeterWrapper::~MLColorimeterWrapper()
		{
		}

		MLBinoBusinessModuleWrapper^ MLColorimeterModuleWrapper::GetBusinessManageModule() {
			ML::MLColorimeter::MLBinoBusinessManage* nativeModule = ml_colorimeter->ML_GetBusinessManageModule();
			return gcnew MLBinoBusinessModuleWrapper(nativeModule);
		}

		MLColorimeterModuleWrapper^ MLColorimeterWrapper::GetMLColorimeterInstance()
		{
			ML::MLColorimeter::MLColorimeter* nativeModule = new ML::MLColorimeter::MLColorimeter();
			return gcnew MLColorimeterModuleWrapper(nativeModule);
			//MLColorimeterPlugin* plugin =
			//	PluginCore::getinstance()->GetPlugin<MLColorimeterPlugin>("MLColorimeterPlugin");
			//ML::MLColorimeter::MLColorimeter* nativeModule = dynamic_cast<ML::MLColorimeter::MLColorimeter*>(plugin);
			//return gcnew MLColorimeterModuleWrapper(nativeModule);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_AddModule(String^ path)
		{
			return MLCommon::MLConverter::ToManaged(ml_bino->ML_AddModule(MLCommon::MLConverter::ToNative(path).c_str()));
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_RemoveModule(int moduleID)
		{
			return MLCommon::MLConverter::ToManaged(ml_bino->ML_RemoveModule(moduleID));
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_AddIPDMotion(String^ path)
		{
			return MLCommon::MLConverter::ToManaged(ml_bino->ML_AddIPDMotion(MLCommon::MLConverter::ToNative(path).c_str()));
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_RemoveIPDMotion()
		{
			return MLCommon::MLConverter::ToManaged(ml_bino->ML_RemoveIPDMotion());
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_ConnectModules()
		{
			Result ret = ml_bino->ML_ConnectModules();
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_DisconnectModules()
		{
			return MLCommon::MLConverter::ToManaged(ml_bino->ML_DisconnectModules());
		}

		bool MLBinoBusinessModuleWrapper::ML_IsModulesConnect()
		{
			return ml_bino->ML_IsModulesConnect();
		}

		bool MLBinoBusinessModuleWrapper::ML_IsModulesMoving()
		{
			return ml_bino->ML_IsModulesMoving();
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_WaitForMovingStop(int timeout, MLCommon::OperationMode mode)
		{
			return MLCommon::MLConverter::ToManaged(ml_bino->ML_WaitForMovingStop(timeout, MLCommon::MLConverter::ToNative(mode)));
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_StopModulesMovement(MLCommon::OperationMode mode)
		{
			return MLCommon::MLConverter::ToManaged(ml_bino->ML_StopModulesMovement(MLCommon::MLConverter::ToNative(mode)));
		}

		Dictionary<int, String^>^ MLBinoBusinessModuleWrapper::ML_GetModulesSerialNumber()
		{
			std::map<int, std::string> native = ml_bino->ML_GetModulesSerialNumber();
			Dictionary<int, String^>^ managed = gcnew Dictionary<int, String^>();
			for (const auto& pair : native) {
				managed->Add(pair.first, gcnew String(pair.second.c_str()));
			}
			return managed;
		}

		Dictionary<int, String^>^ MLBinoBusinessModuleWrapper::ML_GetModulesName()
		{
			std::map<int, std::string> native = ml_bino->ML_GetModulesName();
			Dictionary<int, String^>^ managed = gcnew Dictionary<int, String^>();
			for (const auto& pair : native) {
				managed->Add(pair.first, gcnew String(pair.second.c_str()));
			}
			return managed;
		}

		List<int>^ MLBinoBusinessModuleWrapper::ML_GetModulesIDList()
		{
			std::vector<int> native = ml_bino->ML_GetModulesIDList();
			List<int>^ managed = gcnew List<int>();
			for each (int value in native)
				managed->Add(value);
			return managed;
		}

		int MLBinoBusinessModuleWrapper::ML_GetModulesNumber()
		{
			return ml_bino->ML_GetModulesNumber();
		}

		Dictionary<int, MLCommon::ModuleConfig^>^ MLBinoBusinessModuleWrapper::ML_GetModulesConfig()
		{
			std::map<int, ML::MLColorimeter::ModuleConfig>native = ml_bino->ML_GetModulesConfig();
			Dictionary<int, MLCommon::ModuleConfig^>^ managed = gcnew Dictionary<int, MLCommon::ModuleConfig^>();
			for (const auto& pair : native) {
				managed->Add(pair.first, MLCommon::MLConverter::ToManaged(pair.second));
			}
			return managed;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_MoveModulesND_XYZFilterAsync(String^ keyName, MLCommon::MLFilterEnum channle, MLCommon::OperationMode mode)
		{
			std::string keyName_str = MLCommon::MLConverter::ToNative(keyName);
			ML::MLFilterWheel::MLFilterEnum ml_filter = MLCommon::MLConverter::ToNative(channle);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_MoveModulesND_XYZFilterAsync(keyName_str, ml_filter, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_MoveModulesND_XYZFilterSync(String^ keyName, MLCommon::MLFilterEnum channle, MLCommon::OperationMode mode)
		{
			std::string keyName_str = MLCommon::MLConverter::ToNative(keyName);
			ML::MLFilterWheel::MLFilterEnum ml_filter = MLCommon::MLConverter::ToNative(channle);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_MoveModulesND_XYZFilterSync(keyName_str, ml_filter, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, MLCommon::MLFilterEnum>^ MLBinoBusinessModuleWrapper::ML_GetND_XYZFilterChannel(String^ keyName)
		{
			Dictionary<int, MLCommon::MLFilterEnum>^ managed = gcnew Dictionary<int, MLCommon::MLFilterEnum>();
			std::string keyName_str = MLCommon::MLConverter::ToNative(keyName);
			std::map<int, ML::MLFilterWheel::MLFilterEnum> channelMap = ml_bino->ML_GetND_XYZFilterChannel(keyName_str);
			for (const auto& pair : channelMap) {
				managed->Add(pair.first, MLCommon::MLConverter::ToManaged(pair.second));
			}
			return managed;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_MoveModulesRXFilterAsync(String^ channelName, int degree, MLCommon::OperationMode mode)
		{
			std::string channelName_str = MLCommon::MLConverter::ToNative(channelName);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_MoveModulesRXFilterAsync(channelName_str, degree, ml_mode, nullptr);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_MoveModulesRXFilterSync(String^ channelName, int degree, MLCommon::OperationMode mode)
		{
			std::string channelName_str = MLCommon::MLConverter::ToNative(channelName);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_MoveModulesRXFilterSync(channelName_str, degree, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, String^>^ MLBinoBusinessModuleWrapper::ML_GetRXFilterChannel()
		{
			Dictionary<int, String^>^ channel_dict = gcnew Dictionary<int, String^>();
			std::map<int, std::string> channel_map = ml_bino->ML_GetRXFilterChannel();
			for (const auto& pair : channel_map) {
				channel_dict->Add(pair.first, MLCommon::MLConverter::ToManaged(pair.second));
			}
			return channel_dict;
		}

		Dictionary<int, int>^ MLBinoBusinessModuleWrapper::ML_GetRXFilterAxis()
		{
			Dictionary<int, int>^ channel_dict = gcnew Dictionary<int, int>();
			std::map<int, int> channel_map = ml_bino->ML_GetRXFilterAxis();
			for (const auto& pair : channel_map) {
				channel_dict->Add(pair.first, pair.second);
			}
			return channel_dict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetFocusAsync(double vid, MLCommon::OperationMode mode, MLCommon::FocusMethod method)
		{
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			ML::MLColorimeter::FocusMethod ml_method = MLCommon::MLConverter::ToNative(method);
			Result ret = ml_bino->ML_SetFocusAsync(vid, ml_mode, ml_method);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetFocusSync(double vid, MLCommon::OperationMode mode, MLCommon::FocusMethod method)
		{
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			ML::MLColorimeter::FocusMethod ml_method = MLCommon::MLConverter::ToNative(method);
			Result ret = ml_bino->ML_SetFocusSync(vid, ml_mode, ml_method);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, double>^ MLBinoBusinessModuleWrapper::ML_GetVID(MLCommon::FocusMethod method)
		{
			Dictionary<int, double>^ vid_dict = gcnew Dictionary<int, double>();
			ML::MLColorimeter::FocusMethod ml_method = MLCommon::MLConverter::ToNative(method);
			std::map<int, double> vid_map = ml_bino->ML_GetVID(ml_method);
			for (const auto& pair : vid_map) {
				vid_dict->Add(pair.first, pair.second);
			}
			return vid_dict;

			//throw gcnew System::NotImplementedException();
			//// TODO: 在此处插入 return 语句
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_ThroughFocus(ThroughFocusParams^ params)
		{
			std::string key = MLCommon::MLConverter::ToNative(params->KeyName);
			std::map<int, double> vid;
			for each (auto % pair in params->VID) { vid[pair.Key] = pair.Value;}
			std::map<int, double> pos;
			for each (auto % pair in params->Position) { pos[pair.Key] = pair.Value; }
			ML::MLColorimeter::ThroughFocusConfig focusconfig = MLCommon::MLConverter::ToNative(params->FocusConfig);
			ML::MLColorimeter::OperationMode mode = MLCommon::MLConverter::ToNative(params->Mode);
			Result ret = ml_bino->ML_ThroughFocus(key, vid, pos, focusconfig, mode);
			for each (auto % pair in params->VID) { vid[pair.Key] = pair.Value; }
			Dictionary<int, double>^ vid_dict = gcnew Dictionary<int, double>();
			for (const auto& pair : vid) {vid_dict->Add(pair.first, pair.second);}
			params->VID = vid_dict;
			Dictionary<int, double>^ pos_dict = gcnew Dictionary<int, double>();
			for (const auto& pair : pos) { pos_dict->Add(pair.first, pair.second); }
			params->Position = pos_dict;
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, List<double>^>^ MLBinoBusinessModuleWrapper::ML_GetVIDCurve()
		{
			std::map<int, std::vector<double>> vid_curveMap = ml_bino->ML_GetVIDCurve();
			Dictionary<int, List<double>^>^ dict = gcnew Dictionary<int, List<double>^>();
			for (const auto& pair : vid_curveMap) {
				List<double>^ list = gcnew List<double>();
				for (const auto& value : pair.second) {
					list->Add(value);
				}
				dict->Add(pair.first, list);
			}
			return dict;

			//throw gcnew System::NotImplementedException();
			//// TODO: 在此处插入 return 语句
		}

		Dictionary<int, List<double>^>^ MLBinoBusinessModuleWrapper::ML_GetMTFCurve()
		{
			std::map<int, std::vector<double>> mtf_curveMap = ml_bino->ML_GetMTFCurve();
			Dictionary<int, List<double>^>^ dict = gcnew Dictionary<int, List<double>^>();
			for (const auto& pair : mtf_curveMap) {
				List<double>^ list = gcnew List<double>();
				for (const auto& value : pair.second) {
					list->Add(value);
				}
				dict->Add(pair.first, list);
			}
			return dict;
		}

		Dictionary<int, List<double>^>^ MLBinoBusinessModuleWrapper::ML_GetMotionCurve()
		{
			std::map<int, std::vector<double>> motion_curveMap = ml_bino->ML_GetMotionCurve();
			Dictionary<int, List<double>^>^ dict = gcnew Dictionary<int, List<double>^>();
			for (const auto& pair : motion_curveMap) {
				List<double>^ list = gcnew List<double>();
				for (const auto& value : pair.second) {
					list->Add(value);
				}
				dict->Add(pair.first, list);
			}
			return dict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetPosistionAbsAsync(String^ keyName, double pos, MLCommon::OperationMode mode)
		{
			std::string keyName_str = MLCommon::MLConverter::ToNative(keyName);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_SetPosistionAbsAsync(keyName_str, pos, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetPosistionAbsSync(String^ keyName, double pos, MLCommon::OperationMode mode)
		{
			std::string keyName_str = MLCommon::MLConverter::ToNative(keyName);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_SetPosistionAbsSync(keyName_str, pos, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetPositionRelAsync(String^ keyName, double pos, MLCommon::OperationMode mode)
		{
			std::string keyName_str = MLCommon::MLConverter::ToNative(keyName);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_SetPositionRelAsync(keyName_str, pos, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetPositionRelSync(String^ keyName, double pos, MLCommon::OperationMode mode)
		{
			std::string keyName_str = MLCommon::MLConverter::ToNative(keyName);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_SetPositionRelSync(keyName_str, pos, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, double>^ MLBinoBusinessModuleWrapper::ML_GetMotionPosition(String^ keyName)
		{
			Dictionary<int, double>^ posDict = gcnew Dictionary<int, double>();
			std::string keyName_str = MLCommon::MLConverter::ToNative(keyName);
			std::map<int, double> posMap = ml_bino->ML_GetMotionPosition(keyName_str);
			for (const auto& pair : posMap) {
				posDict->Add(pair.first, pair.second);
			}
			return posDict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_StopMotionMovement(String^ keyName, MLCommon::OperationMode mode)
		{
			std::string keyName_str = MLCommon::MLConverter::ToNative(keyName);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_StopMotionMovement(keyName_str, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetSphericalAsync(double sphere, MLCommon::OperationMode mode)
		{
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_SetSphericalAsync(sphere, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetSphericalSync(double sphere, MLCommon::OperationMode mode)
		{
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_SetSphericalSync(sphere, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, double>^ MLBinoBusinessModuleWrapper::ML_GetSpherical()
		{
			Dictionary<int, double>^ sphereDict = gcnew Dictionary<int, double>();
			std::map<int, double> sphereMap = ml_bino->ML_GetSpherical();
			for (const auto& pair : sphereMap) {
				sphereDict->Add(pair.first, pair.second);
			}
			return sphereDict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetRXAsync(MLCommon::RXCombination^ rx, MLCommon::OperationMode mode)
		{
			ML::MLColorimeter::RXCombination ml_rx = MLCommon::MLConverter::ToNative(rx);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_SetRXAsync(ml_rx, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetRXSync(MLCommon::RXCombination^ rx, MLCommon::OperationMode mode)
		{
			ML::MLColorimeter::RXCombination ml_rx = MLCommon::MLConverter::ToNative(rx);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_SetRXSync(ml_rx, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, MLCommon::RXCombination^>^ MLBinoBusinessModuleWrapper::ML_GetRX()
		{
			Dictionary<int, MLCommon::RXCombination^>^ rxDict = gcnew Dictionary<int, MLCommon::RXCombination^>();
			std::map<int, ML::MLColorimeter::RXCombination> rxMap = ml_bino->ML_GetRX();
			for (const auto& pair : rxMap) {
				rxDict->Add(pair.first, MLCommon::MLConverter::ToManaged(pair.second));
			}
			return rxDict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetIPDAsync(double ipd)
		{
			Result ret = ml_bino->ML_SetIPDAsync(ipd);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetIPDSync(double ipd)
		{
			Result ret = ml_bino->ML_SetIPDSync(ipd);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		double MLBinoBusinessModuleWrapper::ML_GetIPD()
		{
			return ml_bino->ML_GetIPD();
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetAperture(String^ aperture)
		{
			std::string aperture_str = MLCommon::MLConverter::ToNative(aperture);
			Result ret = ml_bino->ML_SetAperture(aperture_str);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, String^>^ MLBinoBusinessModuleWrapper::ML_GetAperture()
		{
			Dictionary<int, String^>^ aperDict = gcnew Dictionary<int, String^>();
			std::map<int, std::string> aperMap = ml_bino->ML_GetAperture();
			for (const auto& pair : aperMap) {
				aperDict->Add(pair.first, MLCommon::MLConverter::ToManaged(pair.second));
			}
			return aperDict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetLightSource(String^ lightSource)
		{
			std::string lightSource_str = MLCommon::MLConverter::ToNative(lightSource);
			Result ret = ml_bino->ML_SetLightSource(lightSource_str);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, String^>^ MLBinoBusinessModuleWrapper::ML_GetLightSource()
		{
			Dictionary<int, String^>^ lightDict = gcnew Dictionary<int, String^>();
			std::map<int, std::string> lightMap = ml_bino->ML_GetLightSource();
			for (const auto& pair : lightMap) {
				lightDict->Add(pair.first, MLCommon::MLConverter::ToManaged(pair.second));
			}
			return lightDict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetExposure(MLCommon::ExposureSetting exposure, MLCommon::OperationMode mode)
		{
			ML::MLColorimeter::ExposureSetting ml_exposure = MLCommon::MLConverter::ToNative(exposure);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_SetExposure(ml_exposure, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, double>^ MLBinoBusinessModuleWrapper::ML_GetExposureTime()
		{
			Dictionary<int, double>^ expoDict = gcnew Dictionary<int, double>();
			std::map<int, double> expoMap = ml_bino->ML_GetExposureTime();
			for (const auto& pair : expoMap) {
				expoDict->Add(pair.first, pair.second);
			}
			return expoDict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetBinning(MLCommon::Binning binning)
		{
			ML::CameraV2::Binning ml_binning = MLCommon::MLConverter::ToNative(binning);
			Result ret = ml_bino->ML_SetBinning(ml_binning);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, MLCommon::Binning>^ MLBinoBusinessModuleWrapper::ML_GetBinning()
		{
			Dictionary<int, MLCommon::Binning>^ binDict = gcnew Dictionary<int, MLCommon::Binning>();
			std::map<int, ML::CameraV2::Binning> binMap = ml_bino->ML_GetBinning();
			for (const auto& pair : binMap) {
				binDict->Add(pair.first, MLCommon::MLConverter::ToManaged(pair.second));
			}
			return binDict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetBinningMode(MLCommon::BinningMode binningMode)
		{
			ML::CameraV2::BinningMode ml_binningMode = MLCommon::MLConverter::ToNative(binningMode);
			Result ret = ml_bino->ML_SetBinningMode(ml_binningMode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, MLCommon::BinningMode>^ MLBinoBusinessModuleWrapper::ML_GetBinningMode()
		{
			Dictionary<int, MLCommon::BinningMode>^ binmodeDict = gcnew Dictionary<int, MLCommon::BinningMode>();
			std::map<int, ML::CameraV2::BinningMode> binmodeMap = ml_bino->ML_GetBinningMode();
			for (const auto& pair : binmodeMap) {
				binmodeDict->Add(pair.first, MLCommon::MLConverter::ToManaged(pair.second));
			}
			return binmodeDict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetPixelFormat(MLCommon::MLPixelFormat pixelFormat)
		{
			ML::CameraV2::MLPixelFormat ml_pixelFormat = MLCommon::MLConverter::ToNative(pixelFormat);
			Result ret = ml_bino->ML_SetPixelFormat(ml_pixelFormat);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, MLCommon::MLPixelFormat>^ MLBinoBusinessModuleWrapper::ML_GetPixelFormat()
		{
			Dictionary<int, MLCommon::MLPixelFormat>^ formatDict = gcnew Dictionary<int, MLCommon::MLPixelFormat>();
			std::map<int, ML::CameraV2::MLPixelFormat> formatMap = ml_bino->ML_GetPixelFormat();
			for (const auto& pair : formatMap) {
				formatDict->Add(pair.first, MLCommon::MLConverter::ToManaged(pair.second));
			}
			return formatDict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_CaptureImageAsync(MLCommon::OperationMode mode)
		{
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_CaptureImageAsync(ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_CaptureImageSync(MLCommon::OperationMode mode)
		{
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_CaptureImageSync(ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<int, IntPtr>^ MLBinoBusinessModuleWrapper::ML_GetImage()
		{
			Dictionary<int, IntPtr>^ imageDict = gcnew Dictionary<int, IntPtr>();
			std::map<int, cv::Mat> imageMap = ml_bino->ML_GetImage();
			for (auto pair : imageMap) {
				MLColorimeterCS::MLCommon::MatWrapper^ wrapper = gcnew MLColorimeterCS::MLCommon::MatWrapper(&pair.second);
				IntPtr matPtr = wrapper->GetMatPtr();
				imageDict->Add(pair.first, matPtr);
			}
			return imageDict;
		}

		Dictionary<int, MLCommon::CaptureData^>^ MLBinoBusinessModuleWrapper::ML_GetCaptureData()
		{
			Dictionary<int, MLCommon::CaptureData^>^ dataDict = gcnew Dictionary<int, MLCommon::CaptureData^>();
			std::map<int, ML::MLColorimeter::CaptureData> dataMap = ml_bino->ML_GetCaptureData();
			for (auto pair : dataMap) {
				dataDict->Add(pair.first, MLCommon::MLConverter::ToManaged(pair.second));
			}
			return dataDict;
		}

		Dictionary<int, Dictionary<MLCommon::MLFilterEnum, MLCommon::CaptureData^>^>^ MLBinoBusinessModuleWrapper::ML_GetColorCameraCaptureData()
		{
			Dictionary<int, Dictionary<MLCommon::MLFilterEnum, MLCommon::CaptureData^>^>^ dict =
				gcnew Dictionary<int, Dictionary<MLCommon::MLFilterEnum, MLCommon::CaptureData^>^>();
			std::map<int, std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData>> dataMap = ml_bino->ML_GetColorCameraCaptureData();
			for (const auto& outerPair : dataMap) {
				dict->Add(outerPair.first, MLCommon::MLConverter::ToManaged(outerPair.second));
			}
			return dict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_LoadDarkByExposureTimeList(String^ path)
		{
			std::string ml_path = MLCommon::MLConverter::ToNative(path);
			Result ret = ml_bino->ML_LoadDarkByExposureTimeList(ml_path.c_str());
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_LoadCalibrationData(MLCommon::CalibrationConfig^ config, MLCommon::OperationMode mode)
		{
			ML::MLColorimeter::CalibrationConfig ml_config = MLCommon::MLConverter::ToNative(config);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_LoadCalibrationData(ml_config, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SetCaptureDataMap(int moduleID, Dictionary<MLCommon::MLFilterEnum, MLCommon::CaptureData^>^ dataMap, bool isSubDarkFromList)
		{
			std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData> ml_dataMap;
			for each (auto % pair in dataMap) {
				ml_dataMap[MLCommon::MLConverter::ToNative(pair.Key)] = MLCommon::MLConverter::ToNative(pair.Value);
			}
			Result ret = ml_bino->ML_SetCaptureDataMap(moduleID, ml_dataMap, isSubDarkFromList);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_Process(MLCommon::CalibrationConfig^ config, MLCommon::OperationMode mode)
		{
			ML::MLColorimeter::CalibrationConfig mlconfig = MLCommon::MLConverter::ToNative(config);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			Result ret = ml_bino->ML_Process(mlconfig, ml_mode);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		Dictionary<MLCommon::CalibrationEnum, Dictionary<MLCommon::MLFilterEnum, MLCommon::CaliProcessData^>^>^MLBinoBusinessModuleWrapper::ML_GetCalibrationData(int moduleID)
		{
			Dictionary<MLCommon::CalibrationEnum, Dictionary<MLCommon::MLFilterEnum, MLCommon::CaliProcessData^>^>^ dict =
				gcnew Dictionary<MLCommon::CalibrationEnum, Dictionary<MLCommon::MLFilterEnum, MLCommon::CaliProcessData^>^>();
			std::map<
				ML::MLColorimeter::CalibrationEnum,
				std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaliProcessData>> calibrationDataMap = ml_bino->ML_GetCalibrationData(moduleID);
			for (const auto& outerPair : calibrationDataMap) {
				dict->Add(MLCommon::MLConverter::ToManaged(outerPair.first), MLCommon::MLConverter::ToManaged(outerPair.second));
			}
			return dict;
		}

		MLCommon::MLResult MLBinoBusinessModuleWrapper::ML_SaveCalibrationData(
			Dictionary<
			MLCommon::CalibrationEnum,
			Dictionary<MLCommon::MLFilterEnum, MLCommon::CaliProcessData^>^>^
			caliData,
			int moduleID, MLCommon::SaveDataConfig^ saveconfig)
		{
			std::map<
				ML::MLColorimeter::CalibrationEnum,
				std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaliProcessData>> calibrationDataMap;

			for each (auto % pair in caliData) {
				calibrationDataMap[MLCommon::MLConverter::ToNative(pair.Key)] = MLCommon::MLConverter::ToNative(pair.Value);
			}
			ML::MLColorimeter::SaveDataConfig ml_saveconfig = MLCommon::MLConverter::ToNative(saveconfig);
			Result ret = ml_bino->ML_SaveCalibrationData(calibrationDataMap, moduleID, ml_saveconfig);
			return MLCommon::MLConverter::ToManaged(ret);
		}

		array<Byte>^ MLBinoBusinessModuleWrapper::GetImageByte()
		{
			cv::Mat image = cv::imread("D:/Image/img2.tif", -1);
			array<Byte>^ c_image = MLCommon::MLConverter::ToByteArray(image);
			return c_image;
		}

		MLCommon::MLResult MLColorimeterModuleWrapper::ML_Measurement(String^ ndKey, String^ xyzKey, MLCommon::CalibrationConfig^ config, MLCommon::ExposureSetting exposure, bool isColorCamera, MLCommon::OperationMode mode)
		{
			std::string ndKey_str = MLCommon::MLConverter::ToNative(ndKey);
			std::string xyzKey_str = MLCommon::MLConverter::ToNative(xyzKey);
			ML::MLColorimeter::CalibrationConfig mlconfig = MLCommon::MLConverter::ToNative(config);
			ML::MLColorimeter::ExposureSetting ml_exposure = MLCommon::MLConverter::ToNative(exposure);
			ML::MLColorimeter::OperationMode ml_mode = MLCommon::MLConverter::ToNative(mode);
			ml_colorimeter->ML_Measurement(ndKey_str, xyzKey_str, mlconfig, ml_exposure, isColorCamera, ml_mode);
			return MLCommon::MLResult();
		}
	}
}
