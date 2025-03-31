#pragma once

#include <msclr/marshal_cppstd.h>
#include "ModuleCommon.h"
#include "MLColorimeterCommon.h"
#include "MLFilterWheelClass.h"
#include "MLCamaraCommon.h"
#include "MotionCommon.h"
#include "Result.h"

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

namespace MLColorimeterCS {
	namespace MLCommon {

		public ref class MatWrapper
		{
		public:
			MatWrapper(cv::Mat* mat) : mat(mat) {}
			IntPtr GetMatPtr() { return IntPtr(mat); }
			int GetMatCols() { return mat->cols; }
			int GetMatRows() { return mat->rows; }
		private:
			cv::Mat* mat;
		};

		private ref class MLConverter {
		public:

			// 将托管数组转换为cv::Mat
			static cv::Mat ConvertManagedArrayToMat(array<unsigned char>^ frameArray, int width, int height, int channels) {
				// 检查输入是否有效
				if (frameArray == nullptr) {
					throw gcnew ArgumentException("Input array is null.");
				}

				// 计算总字节数
				int totalBytes = width * height * channels;

				// 确保托管数组的长度与计算的总字节数一致
				if (frameArray->Length != totalBytes) {
					throw gcnew ArgumentException("Array length does not match the expected size.");
				}

				// 获取托管数组的指针
				GCHandle gch = GCHandle::Alloc(frameArray, GCHandleType::Pinned);
				unsigned char* dataPtr = static_cast<unsigned char*>(gch.AddrOfPinnedObject().ToPointer());

				// 创建cv::Mat对象
				cv::Mat mat(height, width, CV_MAKETYPE(CV_8U, channels), dataPtr);

				// 释放GCHandle
				gch.Free();

				return mat;
			}
			// 将cv::Mat转换为托管数组
			static array<unsigned char>^ ConvertMatToManagedArray(const cv::Mat& mat) {
				// 检查输入是否有效
				if (mat.empty()) {
					throw gcnew ArgumentException("Input cv::Mat is empty.");
				}

				// 获取图像的宽度、高度和通道数
				int width = mat.cols;
				int height = mat.rows;
				int channels = mat.channels();

				// 计算总字节数
				int totalBytes = width * height * channels;

				// 创建托管数组
				array<unsigned char>^ managedArray = gcnew array<unsigned char>(totalBytes);

				// 拷贝数据
				Marshal::Copy(IntPtr(mat.data), managedArray, 0, totalBytes);

				return managedArray;
			}

			static int readInt(const unsigned char*& dataPtr) {
				int value = 0;
				value |= dataPtr[0];
				value |= dataPtr[1] << 8;
				value |= dataPtr[2] << 16;
				value |= dataPtr[3] << 24;
				dataPtr += 4;
				return value;
			}

			// 序列化 cv::Mat 到 byte[]
			static array<Byte>^ ToByteArray(const cv::Mat & mat) {
				if (mat.empty()) {
					throw gcnew System::ArgumentException("Input Mat is empty!");
				}

				// 确保数据连续
				cv::Mat continuousMat;
				if (!mat.isContinuous()) {
					continuousMat = mat.clone();
				}
				else {
					continuousMat = mat;
				}

				// 提取元数据
				int type_code = CV_MAT_DEPTH(mat.type());
				int channels = CV_MAT_CN(mat.type());
				int rows = mat.rows;
				int cols = mat.cols;

				// 计算总字节数
				size_t dataSize = continuousMat.total() * continuousMat.elemSize();

				// 创建托管 byte[]（总长度=16字节元数据 + 数据长度）
				array<Byte>^ byteArray = gcnew array<Byte>(16 + dataSize);
				pin_ptr<Byte> pinnedArray = &byteArray[0];

				// 写入元数据（小端序）
				unsigned char* ptr = pinnedArray;
				*reinterpret_cast<int*>(ptr) = type_code;  ptr += 4;
				*reinterpret_cast<int*>(ptr) = rows;       ptr += 4;
				*reinterpret_cast<int*>(ptr) = cols;       ptr += 4;
				*reinterpret_cast<int*>(ptr) = channels;   ptr += 4;

				// 写入像素数据
				memcpy(ptr, continuousMat.data, dataSize);

				return byteArray;
			}

			//（从byte[]重建Mat）
			static cv::Mat ToNative(array<Byte>^ byteData) {
				if (byteData->Length < 16) {
					throw gcnew System::ArgumentException("Invalid byte array: missing metadata!");
				}

				pin_ptr<Byte> pinnedData = &byteData[0];
				const unsigned char* dataPtr = pinnedData;

				//// 解析元数据（假设小端序）
				//int type_code = *reinterpret_cast<const int*>(dataPtr);
				//dataPtr += 4;
				//int rows = *reinterpret_cast<const int*>(dataPtr);
				//dataPtr += 4;
				//int cols = *reinterpret_cast<const int*>(dataPtr);
				//dataPtr += 4;
				//int channels = *reinterpret_cast<const int*>(dataPtr);
				//dataPtr += 4;
				int type_code = readInt(dataPtr);
				int rows = readInt(dataPtr);
				int cols = readInt(dataPtr);
				int channels = readInt(dataPtr);

				// 计算OpenCV类型（如CV_32FC3）
				int cv_type = CV_MAKETYPE(type_code, channels);

				// 创建cv::Mat并拷贝数据
				cv::Mat *nativeMat = new cv::Mat(rows, cols, cv_type);
				size_t dataSize = rows * cols * channels * CV_ELEM_SIZE(cv_type);
				if (byteData->Length - 16 != dataSize) {
					throw gcnew System::ArgumentException("Data size mismatch!");
				}
				memcpy(nativeMat->data, dataPtr, dataSize);
				return *nativeMat;
			}

			// 基础类型转换
			static String^ ToManaged(const std::string& native) {
				return gcnew String(native.c_str());
			}

			static std::string ToNative(String^ managed) {
				return msclr::interop::marshal_as<std::string>(managed);
			}

			/// <summary>
			/// 将托管 Result 转换为原生 Result
			/// </summary>
			static Result ToNative(MLResult managed) {
				return Result(
					managed.IsSuccess,
					msclr::interop::marshal_as<std::string>(managed.ErrorMsg),
					managed.ErrorCode
				);
			}

			/// <summary>
			/// 将原生 Result 转换为托管 Result
			/// </summary>
			static MLResult ToManaged(const Result& native) {
				return MLResult(
					native.success,
					gcnew String(native.errorMsg.c_str()),
					native.errorCode
				);
			}

			// 枚举转换
			
			// OperationMode
			static ML::MLColorimeter::OperationMode ToNative(OperationMode managed) {
				return static_cast<ML::MLColorimeter::OperationMode>(static_cast<int>(managed));
			}
			// FocusMethod
			static ML::MLColorimeter::FocusMethod ToNative(FocusMethod managed) {
				return static_cast<ML::MLColorimeter::FocusMethod>(static_cast<int>(managed));
			}
			// FFCMethod
			static ML::MLColorimeter::FFCMethod ToNative(FFCMethod managed) {
				return static_cast<ML::MLColorimeter::FFCMethod>(static_cast<int>(managed));
			}
			// EyeMode
			static ML::MLColorimeter::EyeMode ToNative(EyeMode managed) {
				return static_cast<ML::MLColorimeter::EyeMode>(static_cast<int>(managed));
			}

			static EyeMode ToManaged(ML::MLColorimeter::EyeMode native) {
				return static_cast<EyeMode>(static_cast<int>(native));
			}

			// CalibrationEnum
			static ML::MLColorimeter::CalibrationEnum ToNative(CalibrationEnum managed) {
				return static_cast<ML::MLColorimeter::CalibrationEnum>(static_cast<int>(managed));
			}
			static CalibrationEnum ToManaged(ML::MLColorimeter::CalibrationEnum native) {
				return static_cast<CalibrationEnum>(static_cast<int>(native));
			}

			// MLFilterEnum
			static ML::MLFilterWheel::MLFilterEnum ToNative(MLFilterEnum managed) {
				return static_cast<ML::MLFilterWheel::MLFilterEnum>(static_cast<int>(managed));
			}

			static MLFilterEnum ToManaged(ML::MLFilterWheel::MLFilterEnum native) {
				return static_cast<MLFilterEnum>(static_cast<int>(native));
			}

			// MLFilterStatus
			static ML::MLFilterWheel::MLFilterStatus ToNative(MLFilterStatus managed) {
				return static_cast<ML::MLFilterWheel::MLFilterStatus>(static_cast<int>(managed));
			}

			static MLFilterStatus ToManaged(ML::MLFilterWheel::MLFilterStatus native) {
				return static_cast<MLFilterStatus>(static_cast<int>(native));
			}

			// MLCameraState
			static ML::CameraV2::MLCameraState ToNative(MLCameraState managed) {
				return static_cast<ML::CameraV2::MLCameraState>(static_cast<int>(managed));
			}

			static MLCameraState ToManaged(ML::CameraV2::MLCameraState native) {
				return static_cast<MLCameraState>(static_cast<int>(native));
			}

			// Binning
			static ML::CameraV2::Binning ToNative(Binning managed) {
				return static_cast<ML::CameraV2::Binning>(static_cast<int>(managed));
			}
			static Binning ToManaged(ML::CameraV2::Binning native) {
				return static_cast<Binning>(static_cast<int>(native));
			}

			// BinningMode
			static ML::CameraV2::BinningMode ToNative(BinningMode managed) {
				return static_cast<ML::CameraV2::BinningMode>(static_cast<int>(managed));
			}
			static BinningMode ToManaged(ML::CameraV2::BinningMode native) {
				return static_cast<BinningMode>(static_cast<int>(native));
			}

			// MLPixelFormat
			static ML::CameraV2::MLPixelFormat ToNative(MLPixelFormat managed) {
				return static_cast<ML::CameraV2::MLPixelFormat>(static_cast<int>(managed));
			}

			static MLPixelFormat ToManaged(ML::CameraV2::MLPixelFormat native) {
				return static_cast<MLPixelFormat>(static_cast<int>(native));
			}

			// MLMotionState
			static ML::Motion::MLMotionState ToNative(MLMotionState managed) {
				return static_cast<ML::Motion::MLMotionState>(static_cast<int>(managed));
			}

			static MLMotionState ToManaged(ML::Motion::MLMotionState native) {
				return static_cast<MLMotionState>(static_cast<int>(native));
			}

			// ExposureSetting 转换
			static ML::MLColorimeter::ExposureSetting ToNative(ExposureSetting managed) {
				ML::MLColorimeter::ExposureSetting native;
				native.Mode = static_cast<ML::MLColorimeter::ExposureMode>(managed.Mode);
				native.ExposureTime = managed.ExposureTime;
				return native;
			}

			static ExposureSetting ToManaged(const ML::MLColorimeter::ExposureSetting& native) {
				return ExposureSetting(
					static_cast<ExposureMode>(native.Mode),
					native.ExposureTime
				);
			}

			// RXCombination 转换
			static ML::MLColorimeter::RXCombination ToNative(RXCombination^ managed) {
				ML::MLColorimeter::RXCombination native;
				native.Sphere = managed->Sphere;
				native.Cylinder = managed->Cylinder;
				native.Axis = managed->Axis;
				return native;
			}

			static RXCombination^ ToManaged(const ML::MLColorimeter::RXCombination native) {
				RXCombination^ managed = gcnew RXCombination();
				managed->Sphere = native.Sphere;
				managed->Cylinder = native.Cylinder;
				managed->Axis = native.Axis;
				return managed;
			}

			// Bitmap 转换
			static cv::Mat ToNative(Bitmap^ managed) {
				if (managed == nullptr) return cv::Mat();

				BitmapData^ bmpData = managed->LockBits(
					System::Drawing::Rectangle(0, 0, managed->Width, managed->Height),
					ImageLockMode::ReadOnly,
					managed->PixelFormat);

				cv::Mat native;

				switch (managed->PixelFormat)
				{
				case System::Drawing::Imaging::PixelFormat::Format8bppIndexed:
					native = cv::Mat(managed->Height, managed->Width, CV_8UC1, bmpData->Scan0.ToPointer());
					break;
				case System::Drawing::Imaging::PixelFormat::Format24bppRgb:
					native = cv::Mat(managed->Height, managed->Width, CV_8UC3, bmpData->Scan0.ToPointer());
					break;
				case System::Drawing::Imaging::PixelFormat::Format32bppArgb:
					native = cv::Mat(managed->Height, managed->Width, CV_32FC1, bmpData->Scan0.ToPointer());
					break;
					// 添加其他格式支持...
				}

				managed->UnlockBits(bmpData);
				return native.clone();
			}

			static Bitmap^ ToManaged(const cv::Mat& native) {
				if (native.empty()) return nullptr;

				PixelFormat format;
				switch (native.type())
				{
				case CV_8UC1:
					format = PixelFormat::Format8bppIndexed;
					break;
				case CV_8UC3:
					format = PixelFormat::Format24bppRgb;
					break;
					// 添加其他格式支持...
				default:
					throw gcnew ArgumentException("Unsupported mat type");
				}

				Bitmap^ managed = gcnew Bitmap(native.cols, native.rows, format);

				BitmapData^ bmpData = managed->LockBits(
					System::Drawing::Rectangle(0, 0, managed->Width, managed->Height),
					ImageLockMode::WriteOnly,
					format);

				memcpy(bmpData->Scan0.ToPointer(), native.data, native.total() * native.elemSize());

				managed->UnlockBits(bmpData);
				return managed;
			}

			// MLSerialInfo 转换
			static ML::MLFilterWheel::MLSerialInfo ToNative(MLSerialInfo^ managed) {
				if (managed == nullptr)
					return ML::MLFilterWheel::MLSerialInfo();

				ML::MLFilterWheel::MLSerialInfo native;
				native.baudrate = managed->Baudrate;
				native.bytesize = managed->ByteSize;
				native.parity = ToNative(managed->Parity);
				native.stopbits = ToNative(managed->StopBits);
				native.flowcontrol = ToNative(managed->FlowControl);
				return native;
			}
			static MLSerialInfo^ ToManaged(const ML::MLFilterWheel::MLSerialInfo& native) {
				MLSerialInfo^ managed = gcnew MLSerialInfo();
				managed->Baudrate = native.baudrate;
				managed->ByteSize = native.bytesize;
				managed->Parity = gcnew String(native.parity.c_str());
				managed->StopBits = gcnew String(native.stopbits.c_str());
				managed->FlowControl = gcnew String(native.flowcontrol.c_str());
				return managed;
			}

			// MLIOCommand 转换
			static ML::MLFilterWheel::MLIOCommand ToNative(MLIOCommand^ managed) {
				ML::MLFilterWheel::MLIOCommand native;
				native.start = managed->Start;
				native.zHome = managed->ZHome;
				native.stop = managed->Stop;
				native.alarmRst = managed->AlarmRst;
				return native;
			}
			static MLIOCommand^ ToManaged(ML::MLFilterWheel::MLIOCommand native) {
				MLIOCommand^ managed = gcnew MLIOCommand();
				managed->Start = native.start;
				managed->ZHome = native.zHome;
				managed->Stop = native.stop;
				managed->AlarmRst = native.alarmRst;
				return managed;
			}

			// MLAxisInfo 转换
			static ML::MLFilterWheel::MLAxisInfo ToNative(MLAxisInfo^ managed) {
				ML::MLFilterWheel::MLAxisInfo native;
				native.enable = managed->Enable;
				native.name = ToNative(managed->Name);
				native.motorType = ToNative(managed->MotorType);
				native.station = managed->Station;
				native.MLIOCommand_enable = managed->MLIOCommand_Enable;
				native.mlIOCommand = ToNative(managed->Command);
				native.PulsePerCycle = managed->PulsePerCycle;
				native.Axis_Index = managed->Axis_Index;
				native.Min = managed->Min;
				native.Max = managed->Max;
				return native;
			}
			static MLAxisInfo^ ToManaged(ML::MLFilterWheel::MLAxisInfo native) {
				MLAxisInfo^ managed = gcnew MLAxisInfo();
				managed->Enable = native.enable;
				managed->Name = ToManaged(native.name);
				managed->MotorType = ToManaged(native.motorType);
				managed->Station = native.station;
				managed->MLIOCommand_Enable = native.MLIOCommand_enable;
				managed->Command = ToManaged(native.mlIOCommand);
				managed->PulsePerCycle = native.PulsePerCycle;
				managed->Axis_Index = native.Axis_Index;
				managed->Min = native.Min;
				managed->Max = native.Max;
				return managed;
			}

			// MLNDFilterConfiguation 转换
			static ML::MLFilterWheel::MLNDFilterConfiguation ToNative(MLNDFilterConfiguation^ managed) {
				ML::MLFilterWheel::MLNDFilterConfiguation native;
				native.enable = managed->Enable;
				native.type = ToNative(managed->Type);
				native.name = ToNative(managed->Name);
				native.protocol = ToNative(managed->Protocol);
				native.motorType = ToNative(managed->MotorType);
				native.port = ToNative(managed->Port);
				native.station = managed->Station;
				native.serial_info = ToNative(managed->SerialInfo);

				// 转换字典
				for each (auto pair in managed->PositionName_List) {
					native.positionName_List[ToNative(pair.Key)] = pair.Value;
				}

				for each (auto pair in managed->PositionEnum_List) {
					native.positionEnum_List[ToNative(pair.Key)] = pair.Value;
				}

				native.MLIOCommand_enable = managed->MLIOCommand_Enable;
				native.MLIOCommand = ToNative(managed->Command);
				return native;
			}
			static MLNDFilterConfiguation^ ToManaged(const ML::MLFilterWheel::MLNDFilterConfiguation& native) {
				MLNDFilterConfiguation^ managed = gcnew MLNDFilterConfiguation();

				managed->Enable = native.enable;
				managed->Type = gcnew String(native.type.c_str());
				managed->Name = gcnew String(native.name.c_str());
				managed->Protocol = gcnew String(native.protocol.c_str());
				managed->MotorType = gcnew String(native.motorType.c_str());
				managed->Port = gcnew String(native.port.c_str());
				managed->Station = native.station;

				// 转换 SerialInfo
				managed->SerialInfo = ToManaged(native.serial_info);

				// 转换 PositionName_List
				for (const auto& pair : native.positionName_List) {
					managed->PositionName_List->Add(
						gcnew String(pair.first.c_str()),
						pair.second
					);
				}

				// 转换 PositionEnum_List
				for (const auto& pair : native.positionEnum_List) {
					managed->PositionEnum_List->Add(
						static_cast<MLFilterEnum>(pair.first),
						pair.second
					);
				}

				managed->MLIOCommand_Enable = native.MLIOCommand_enable;
				managed->Command = ToManaged(native.MLIOCommand);

				return managed;
			}

			// MLRXFilterConfiguation 转换
			static ML::MLFilterWheel::MLRXFilterConfiguation ToNative(MLRXFilterConfiguation^ managed) {
				if (managed == nullptr)
					return ML::MLFilterWheel::MLRXFilterConfiguation();

				ML::MLFilterWheel::MLRXFilterConfiguation native;
				native.enable = managed->Enable;
				native.type = msclr::interop::marshal_as<std::string>(managed->Type);
				native.name = msclr::interop::marshal_as<std::string>(managed->Name);
				native.protocol = msclr::interop::marshal_as<std::string>(managed->Protocol);
				native.motorType = msclr::interop::marshal_as<std::string>(managed->MotorType);
				native.port = msclr::interop::marshal_as<std::string>(managed->Port);
				native.station = managed->Station;

				// 转换 SerialInfo
				native.serial_info = ToNative(managed->SerialInfo);

				// 转换字典
				for each (System::Collections::Generic::KeyValuePair<String^, int> pair
					in managed->PositionName_List) {
					native.positionName_List[msclr::interop::marshal_as<std::string>(pair.Key)] = pair.Value;
				}

				native.MLIOCommand_enable = managed->MLIOCommand_Enable;

				// 转换 Command
				if (managed->Command != nullptr) {
					native.MLIOCommand = ToNative(managed->Command);
				}

				// 转换 AxisInfo
				if (managed->AxisInfo != nullptr) {
					native.axis_info = ToNative(managed->AxisInfo);
				}

				return native;
			}

			static MLRXFilterConfiguation^ ToManaged(const ML::MLFilterWheel::MLRXFilterConfiguation& native) {
				MLRXFilterConfiguation^ managed = gcnew MLRXFilterConfiguation();

				managed->Enable = native.enable;
				managed->Type = gcnew String(native.type.c_str());
				managed->Name = gcnew String(native.name.c_str());
				managed->Protocol = gcnew String(native.protocol.c_str());
				managed->MotorType = gcnew String(native.motorType.c_str());
				managed->Port = gcnew String(native.port.c_str());
				managed->Station = native.station;

				// 转换 SerialInfo
				managed->SerialInfo = ToManaged(native.serial_info);

				// 转换字典
				managed->PositionName_List = gcnew Dictionary<String^, int>();
				for (const auto& pair : native.positionName_List) {
					managed->PositionName_List->Add(
						gcnew String(pair.first.c_str()),
						pair.second
					);
				}

				managed->MLIOCommand_Enable = native.MLIOCommand_enable;

				// 转换 Command
				managed->Command = ToManaged(native.MLIOCommand);

				// 转换 AxisInfo
				managed->AxisInfo = ToManaged(native.axis_info);

				return managed;
			}

			// CaptureData 转换
			static ML::MLColorimeter::CaptureData ToNative(CaptureData^ managed) {
				ML::MLColorimeter::CaptureData native;
				native.SerialNumber = ToNative(managed->SerialNumber);
				native.ModuleName = ToNative(managed->ModuleName);
				native.Key = ToNative(managed->Key);
				native.Aperture = ToNative(managed->Aperture);
				native.LightSource = ToNative(managed->LightSource);
				native.NDFilter = ToNative(managed->NDFilter);
				native.ColorFilter = ToNative(managed->ColorFilter);
				native.MovementRX = ToNative(managed->MovementRX);
				native.VID = managed->VID;
				native.ExposureTime = managed->ExposureTime;
				native.Img = ToNative(managed->Img);
				native.Binning = static_cast<ML::CameraV2::Binning>(managed->Bining);
				native.PixelFormat = static_cast<ML::CameraV2::MLPixelFormat>(managed->PixelFormat);
				return native;
			}
			static CaptureData^ ToManaged(const ML::MLColorimeter::CaptureData& native) {
				CaptureData^ managed = gcnew CaptureData();

				managed->SerialNumber = ToManaged(native.SerialNumber);
				managed->ModuleName = ToManaged(native.ModuleName);
				managed->Key = ToManaged(native.Key);
				managed->Aperture = ToManaged(native.Aperture);
				managed->LightSource = ToManaged(native.LightSource);
				managed->NDFilter = ToManaged(native.NDFilter);
				managed->ColorFilter = ToManaged(native.ColorFilter);
				managed->MovementRX = ToManaged(native.MovementRX);
				managed->VID = native.VID;
				managed->ExposureTime = native.ExposureTime;
				managed->Img = ToManaged(native.Img);
				managed->Bining = ToManaged(native.Binning);
				managed->PixelFormat = ToManaged(native.PixelFormat);
				return managed;
			}

			static ML::MLColorimeter::RXMappingMethod ToNative(RXMappingMethod managed) {
				ML::MLColorimeter::RXMappingMethod native;
				native.SphMapping = static_cast<ML::MLColorimeter::MappingMethod>(static_cast<int>(managed.SphMapping));
				native.CylMapping = static_cast<ML::MLColorimeter::MappingMethod>(static_cast<int>(managed.CylMapping));
				native.AxisMapping = static_cast<ML::MLColorimeter::MappingMethod>(static_cast<int>(managed.AxisMapping));
				return native;
			}

			static RXMappingMethod ToManaged(const ML::MLColorimeter::RXMappingMethod& native) {
				return RXMappingMethod(
					static_cast<MappingMethod>(static_cast<int>(native.SphMapping)),
					static_cast<MappingMethod>(static_cast<int>(native.CylMapping)),
					static_cast<MappingMethod>(static_cast<int>(native.AxisMapping))
				);
			}

			// RXMappingRule 转换
			static ML::MLColorimeter::RXMappingRule ToNative(RXMappingRule^ managed) {
				ML::MLColorimeter::RXMappingRule native;
				for each (double value in managed->SphMappingList)
					native.SphMappingList.push_back(value);

				for each (double value in managed->CylMappingList)
					native.CylMappingList.push_back(value);

				for each (int value in managed->AxisMappingList)
					native.AxisMappingList.push_back(value);

				native.RXMethod = ToNative(managed->RXMethod);

				return native;
			}

			static RXMappingRule^ ToManaged(const ML::MLColorimeter::RXMappingRule& native) {
				RXMappingRule^ managed = gcnew RXMappingRule();

				for (const auto& value : native.SphMappingList)
					managed->SphMappingList->Add(value);

				for (const auto& value : native.CylMappingList)
					managed->CylMappingList->Add(value);

				for (const auto& value : native.AxisMappingList)
					managed->AxisMappingList->Add(value);

				managed->RXMethod = ToManaged(native.RXMethod);

				return managed;
			}

			// CalibrationConfig 转换
			static ML::MLColorimeter::CalibrationConfig ToNative(CalibrationConfig^ managed) {
				ML::MLColorimeter::CalibrationConfig native;
				native.InputPath = ToNative(managed->InputPath);
				native.Aperture = ToNative(managed->Aperture);
				native.NDFilter = ToNative(managed->NDFilter);
				native.RX = ToNative(managed->RX);
				native.LightSource = ToNative(managed->LightSource);
				native.Dark_Flag = managed->Dark_Flag;
				native.FFC_Flag = managed->FFC_Flag;
				native.ColorShift_Flag = managed->ColorShift_Flag;
				native.Distortion_Flag = managed->Distortion_Flag;
				native.Exposure_Flag = managed->Exposure_Flag;
				native.FourColor_Flag = managed->FourColor_Flag;
				native.Luminance_Flag = managed->Luminance_Flag;
				native.FOVCrop_Flag = managed->FOVCrop_Flag;

				native.ColorFilterList.clear();
				for each (MLFilterEnum value in managed->ColorFilterList)
					native.ColorFilterList.push_back(ToNative(value));

				return native;
			}

			// SaveDataConfig 转换
			static ML::MLColorimeter::SaveDataConfig ToNative(SaveDataConfig^ managed) {
				ML::MLColorimeter::SaveDataConfig native;
				native.SavePath = ToNative(managed->SavePath);
				native.Prefix = ToNative(managed->Prefix);
				native.SaveROI = cv::Rect(managed->SaveROI.X, managed->SaveROI.Y,
					managed->SaveROI.Width, managed->SaveROI.Height);
				native.SaveRaw = managed->SaveRaw;
				native.SaveResult = managed->SaveResult;
				native.SaveCalibration = managed->SaveCalibration;
				return native;
			}

			// ThroughFocusConfig 转换
			static ML::MLColorimeter::ThroughFocusConfig ToNative(ThroughFocusConfig^ managed) {
				ML::MLColorimeter::ThroughFocusConfig native;
				native.FocusMax = managed->FocusMax;
				native.FocusMin = managed->FocusMin;
				native.ReferencePosition = managed->ReferencePosition;
				native.FocalLength = managed->FocalLength;
				native.FocalPlanesObjectSpace = managed->FocalPlanesObjectSpace;
				native.RoughStep = managed->RoughStep;
				native.FineRange = managed->FineRange;
				native.FineStep = managed->FineStep;
				native.Freq = managed->Freq;
				native.Smooth = managed->Smooth;

				for each (Rect roi in managed->ROIs) {
					native.ROIs.push_back(cv::Rect(roi.X, roi.Y, roi.Width, roi.Height));
				}

				native.ChessMode = managed->ChessMode;
				native.LpmmUnit = managed->LpmmUnit;
				return native;
			}
			// 其他辅助方法...
			
			// MotionConfig
			static MotionConfig^ ToManaged(const ML::MLColorimeter::MotionConfig& native) {
				MotionConfig^ managed = gcnew MotionConfig();

				managed->Enable = native.Enable;
				managed->Key = ToManaged(native.Key);
				managed->Type = ToManaged(native.Type);
				managed->Name = ToManaged(native.Name);
				managed->ConnectAddress = ToManaged(native.ConnectAddress);
				managed->ConnectType = ToManaged(native.ConnectType);
				managed->Port = ToManaged(native.Port);
				managed->DeviceID = native.DeviceID;
				managed->Axis = native.Axis;
				managed->AxisName = ToManaged(native.AxisName);
				managed->HomingMethod = ToManaged(native.HomingMethod);
				managed->Speed = native.Speed;
				managed->IsReverse = native.IsReverse;
				managed->SoftwareLimitMax = native.SoftwareLimitMax;
				managed->SoftwareLimitMin = native.SoftwareLimitMin;
				managed->ReferencePosition = native.ReferencePosition;
				managed->FocalLength = native.FocalLength;
				managed->FocalPlanesObjectSpace = native.FocalPlanesObjectSpace;

				return managed;
			}
			// CameraConfig
			static CameraConfig^ ToManaged(const ML::MLColorimeter::CameraConfig& native) {
				CameraConfig^ managed = gcnew CameraConfig();

				managed->Enable = native.Enable;
				managed->ColourCamera = native.ColourCamera;
				managed->Key = ToManaged(native.Key);
				managed->ConnectAddress = ToManaged(native.ConnectAddress);
				managed->Type = ToManaged(native.Type);
				managed->Name = ToManaged(native.Name);
				return managed;
			}
			//ModuleConfig
			static ModuleConfig^ ToManaged(const ML::MLColorimeter::ModuleConfig& native) {
				ModuleConfig^ managed = gcnew ModuleConfig();

				managed->Enable = native.Enable;
				managed->SerialNumber = ToManaged(native.SerialNumber);
				managed->Name = ToManaged(native.Name);
				managed->Key = ToManaged(native.Key);
				managed->Aperture = ToManaged(native.Aperture);
				managed->EyeMode = ToManaged(native.EyeMode);
				managed->ID = native.ID;
				managed->CameraConfig = ToManaged(native.CameraConfig);
				managed->IPDConfig = ToManaged(native.IPDConfig);


				// 转换 MotionConfig_Map
				for (const auto& pair : native.MotionConfig_Map) {
					managed->MotionConfig_Map->Add(
						gcnew String(pair.first.c_str()),
						ToManaged(pair.second)
					);
				}

				// 转换 NDFilterConfig_Map
				for (const auto& pair : native.NDFilterConfig_Map) {
					managed->NDFilterConfig_Map->Add(
						gcnew String(pair.first.c_str()),
						ToManaged(pair.second)
					);
				}

				return managed;
			}

			static Dictionary<MLCommon::MLFilterEnum, MLCommon::CaptureData^>^ ToManaged(
				const std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData>& nativeMap
			) {
				Dictionary<MLCommon::MLFilterEnum, MLCommon::CaptureData^>^ dict = gcnew Dictionary<MLCommon::MLFilterEnum, MLCommon::CaptureData^>();
				for (const auto& pair : nativeMap) {
					dict->Add(MLCommon::MLConverter::ToManaged(pair.first),
						MLCommon::MLConverter::ToManaged(pair.second));
				}
				return dict;
			}

			static std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData> ToNative(
				Dictionary<MLCommon::MLFilterEnum, MLCommon::CaptureData^>^ managedMap
			) {
				std::map<ML::MLFilterWheel::MLFilterEnum, ML::MLColorimeter::CaptureData> nativeMap;
				for each (auto % pair in managedMap) {
					nativeMap[MLCommon::MLConverter::ToNative(pair.Key)] = MLCommon::MLConverter::ToNative(pair.Value);
				}
				return nativeMap;
			}
		};
	}
}