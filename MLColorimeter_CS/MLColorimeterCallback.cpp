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

			// �������йܻص�����
			unmanagedCallback = new MLCameraCallbackWrapper(this);
		}

		// MLCameraCallbackWrapper ���캯����ʵ��
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

			// �������йܻص�����
			unmanagedCallback = new MLMotionCallbackWrapper(this);
		}

		// MLMotionCallbackWrapper ���캯����ʵ��
		MLMotionCallbackWrapper::MLMotionCallbackWrapper(ManagedMotionCallback^ callback) {
			this->managedCallback = callback;
		}
	}

	namespace NotifyFilterWheelCallback
	{
		ManagedFilterWheelCallback::ManagedFilterWheelCallback(NotifyFilterStatusChangedDelegate^ stateChanged) {
			this->stateChangedDelegate = stateChanged;

			// �������йܻص�����
			unmanagedCallback = new MLFilterWheelCallbackWrapper(this);
		}

		// MLFilterWheelCallbackWrapper ���캯����ʵ��
		MLFilterWheelCallbackWrapper::MLFilterWheelCallbackWrapper(ManagedFilterWheelCallback^ callback) {
			this->managedCallback = callback;
		}
	}
}
