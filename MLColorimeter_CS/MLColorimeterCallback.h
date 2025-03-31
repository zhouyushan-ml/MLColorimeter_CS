// C++/CLI桥接代码
#include <Windows.h>
#include "MLCamaraCommon.h"
#include "MotionCommon.h"
#include "ModuleCommon.h"
#include "MLFilterWheelClass.h"
#include "MLConverters.h"
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;
using namespace ML::CameraV2;
using namespace ML::Motion;
using namespace ML::MLFilterWheel;

namespace MLColorimeterCS {
	namespace NotifyCameraCallback
	{
		// 定义托管回调委托
		public delegate void NotifyCameraStateChangedDelegate(MLCommon::MLCameraState old_state, MLCommon::MLCameraState new_state);
		public delegate void NotifyCameraFrameReceivedDelegate(IntPtr frame, int width, int height, int channels, MLCommon::MLPixelFormat format);
		public delegate void NotifyCameraGrayLevelDelegate(int gray_level);

		// 托管回调类
		public ref class ManagedCameraCallback
		{
		private:
			MLCameraCallback* unmanagedCallback;
			NotifyCameraStateChangedDelegate^ stateChangedDelegate;
			NotifyCameraFrameReceivedDelegate^ frameReceivedDelegate;
			NotifyCameraGrayLevelDelegate^ grayLevelDelegate;

		public:
			ManagedCameraCallback(NotifyCameraStateChangedDelegate^ stateChanged,
				NotifyCameraFrameReceivedDelegate^ frameReceived,
				NotifyCameraGrayLevelDelegate^ grayLevel);

			~ManagedCameraCallback()
			{
				delete unmanagedCallback;
			}

			// 获取非托管回调对象
			MLCameraCallback* GetUnmanagedCallback()
			{
				return unmanagedCallback;
			}

			// 托管回调方法
			void NotifyCameraStateChanged(MLCommon::MLCameraState old_state, MLCommon::MLCameraState new_state)
			{
				stateChangedDelegate(old_state, new_state);
			}

			void NotifyCameraFrameReceived(IntPtr frame, int width, int height, int channels, MLCommon::MLPixelFormat format)
			{
				frameReceivedDelegate(frame, width, height, channels, format);
			}

			void NotifyCameraGrayLevel(int gray_level)
			{
				grayLevelDelegate(gray_level);
			}
			//test
			void changeCameraGrayLevel()
			{
				unmanagedCallback->NotifyCameraGrayLevel(20);
			}

			void changeCameraImage()
			{
				cv::Mat frame = cv::imread("D:/Image/img_mtf_display_w.tif",-1);
				MLPixelFormat format = MLPixelFormat::MLMono16;
				unmanagedCallback->NotifyCameraFrameReceived(frame, format);
			}
		};

		// 非托管回调包装类
		class MLCameraCallbackWrapper : public MLCameraCallback
		{
		private:
			gcroot<ManagedCameraCallback^> managedCallback;  // 使用 gcroot<> 代替 ^

		public:
			MLCameraCallbackWrapper(ManagedCameraCallback^ callback);

			virtual void NotifyCameraStateChanged(MLCameraState old_state, MLCameraState new_state)
			{
				managedCallback->NotifyCameraStateChanged(MLCommon::MLConverter::ToManaged(old_state), MLCommon::MLConverter::ToManaged(new_state));
			}

			virtual void NotifyCameraFrameReceived(cv::Mat frame, MLPixelFormat format)
			{
				// 将cv::Mat转换为托管数组
				int width = frame.cols;
				int height = frame.rows;
				int channels = frame.channels();
				MLCommon::MLPixelFormat formatInt = MLCommon::MLConverter::ToManaged(format);

				//array<unsigned char>^ frameArray = gcnew array<unsigned char>(frame.total() * channels);
				//Marshal::Copy(frame.data, frameArray, 0, frameArray->Length);

				//array<unsigned char>^ frameArray = MLCommon::MLConverter::ConvertMatToManagedArray(frame);

				MLColorimeterCS::MLCommon::MatWrapper^ wrapper = gcnew MLColorimeterCS::MLCommon::MatWrapper(&frame);
				IntPtr matPtr = wrapper->GetMatPtr();
				int cols = wrapper->GetMatCols();
				int rows = wrapper->GetMatRows();

				managedCallback->NotifyCameraFrameReceived(matPtr, width, height, channels, formatInt);
			}

			virtual void NotifyCameraGrayLevel(int gray_level)
			{
				managedCallback->NotifyCameraGrayLevel(gray_level);
			}
		};
	}

	namespace NotifyMotionCallback
	{
		// 定义托管回调委托
		public delegate void NotifyMotionStateChangedDelegate(MLCommon::MLMotionState old_state, MLCommon::MLMotionState new_state);
		public delegate void NotifyMotionPositionDelegate(int position);

		// 托管回调类
		public ref class ManagedMotionCallback
		{
		private:
			MLMotionCallback* unmanagedCallback;
			NotifyMotionStateChangedDelegate^ stateChangedDelegate;
			NotifyMotionPositionDelegate^ positionDelegate;

		public:
			ManagedMotionCallback(NotifyMotionStateChangedDelegate^ stateChanged,
				NotifyMotionPositionDelegate^ position);

			~ManagedMotionCallback()
			{
				delete unmanagedCallback;
			}

			// 获取非托管回调对象
			MLMotionCallback* GetUnmanagedCallback()
			{
				return unmanagedCallback;
			}

			// 托管回调方法
			void NotifyMotionStateChanged(MLCommon::MLMotionState old_state, MLCommon::MLMotionState new_state)
			{
				stateChangedDelegate(old_state, new_state);
			}

			void NotifyMotionPosition(int position)
			{
				positionDelegate(position);
			}
			//test
			void changeMotionPosition()
			{
				unmanagedCallback->NotifyMotionPosition(20);
			}
		};

		// 非托管回调包装类
		class MLMotionCallbackWrapper : public MLMotionCallback
		{
		private:
			gcroot<ManagedMotionCallback^> managedCallback;  // 使用 gcroot<> 代替 ^

		public:
			MLMotionCallbackWrapper(ManagedMotionCallback^ callback);

			virtual void NotifyMotionStateChanged(MLMotionState old_state, MLMotionState new_state)
			{
				managedCallback->NotifyMotionStateChanged(MLCommon::MLConverter::ToManaged(old_state), MLCommon::MLConverter::ToManaged(new_state));
			}

			virtual void NotifyMotionPosition(int position)
			{
				managedCallback->NotifyMotionPosition(position);
			}
		};
	}

	namespace NotifyFilterWheelCallback
	{
		// 定义托管回调委托
		public delegate void NotifyFilterStatusChangedDelegate(String^ object, MLCommon::MLFilterStatus status);

		// 托管回调类
		public ref class ManagedFilterWheelCallback
		{
		private:
			MLFilterWheelCallback* unmanagedCallback;
			NotifyFilterStatusChangedDelegate^ stateChangedDelegate;

		public:
			ManagedFilterWheelCallback(NotifyFilterStatusChangedDelegate^ stateChanged);

			~ManagedFilterWheelCallback()
			{
				delete unmanagedCallback;
			}

			// 获取非托管回调对象
			MLFilterWheelCallback* GetUnmanagedCallback()
			{
				return unmanagedCallback;
			}

			// 托管回调方法
			void NotifyFilterStatusChanged(String^ object, MLCommon::MLFilterStatus status)
			{
				stateChangedDelegate(object, status);
			}
		};

		// 非托管回调包装类
		class MLFilterWheelCallbackWrapper : public MLFilterWheelCallback
		{
		private:
			gcroot<ManagedFilterWheelCallback^> managedCallback;  // 使用 gcroot<> 代替 ^

		public:
			MLFilterWheelCallbackWrapper(ManagedFilterWheelCallback^ callback);

			virtual void NotifyFilterStatusChanged(const std::string object, ML::MLFilterWheel::MLFilterStatus status)
			{
				managedCallback->NotifyFilterStatusChanged(MLCommon::MLConverter::ToManaged(object), MLCommon::MLConverter::ToManaged(status));
			}
		};
	}
}



