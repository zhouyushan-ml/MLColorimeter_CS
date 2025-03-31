#include "pch.h"

#include "MLColorimeterCallback.h"

namespace MLColorimeterCS {
	namespace NotifyCameraCallback
	{
		ManagedCameraCallback::ManagedCameraCallback(NotifyCameraStateChangedDelegate^ stateChanged,
			NotifyCameraFrameReceivedDelegate^ frameReceived,
			NotifyCameraGrayLevelDelegate^ grayLevel) {
			this->stateChangedDelegate = stateChanged;
			this->frameReceivedDelegate = frameReceived;
			this->grayLevelDelegate = grayLevel;

			// 创建非托管回调对象
			unmanagedCallback = new MLCameraCallbackWrapper(this);
		}

		// MLCameraCallbackWrapper 构造函数的实现
		MLCameraCallbackWrapper::MLCameraCallbackWrapper(ManagedCameraCallback^ callback) {
			this->managedCallback = callback;
		}
	};

	namespace NotifyMotionCallback
	{
		ManagedMotionCallback::ManagedMotionCallback(NotifyMotionStateChangedDelegate^ stateChanged,
			NotifyMotionPositionDelegate^ position) {
			this->stateChangedDelegate = stateChanged;
			this->positionDelegate = position;

			// 创建非托管回调对象
			unmanagedCallback = new MLMotionCallbackWrapper(this);
		}

		// MLMotionCallbackWrapper 构造函数的实现
		MLMotionCallbackWrapper::MLMotionCallbackWrapper(ManagedMotionCallback^ callback) {
			this->managedCallback = callback;
		}
	}

	namespace NotifyFilterWheelCallback
	{
		ManagedFilterWheelCallback::ManagedFilterWheelCallback(NotifyFilterStatusChangedDelegate^ stateChanged) {
			this->stateChangedDelegate = stateChanged;

			// 创建非托管回调对象
			unmanagedCallback = new MLFilterWheelCallbackWrapper(this);
		}

		// MLFilterWheelCallbackWrapper 构造函数的实现
		MLFilterWheelCallbackWrapper::MLFilterWheelCallbackWrapper(ManagedFilterWheelCallback^ callback) {
			this->managedCallback = callback;
		}
	}
}
