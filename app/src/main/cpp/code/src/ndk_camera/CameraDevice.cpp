//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CameraDevice.h"
#include "ndk_camera/Log.h"

#include <utility>

namespace ndkcamera {

    CameraDevice::CameraDevice(ACameraDevice *cameraDevice, std::unique_ptr<CameraDeviceStateCallbacks> stateCallbacks)
            : mCameraDevice(cameraDevice), mStateCallbacks(std::move(stateCallbacks)) {
        if (mStateCallbacks != nullptr) {
            mStateCallbacks->getStateCallbacks().context = this;
        }
    }

    CameraDevice::~CameraDevice() {
        if (mCameraDevice != nullptr) {
            ACameraDevice_close(mCameraDevice);
        }
    }

    CameraDevice::CameraDevice(CameraDevice &&other) noexcept
            : mCameraDevice(std::exchange(other.mCameraDevice, nullptr)),
              mStateCallbacks(std::move(other.mStateCallbacks)) {
        mStateCallbacks->getStateCallbacks().context = this;
    }

    CameraDevice &CameraDevice::operator=(CameraDevice &&other) noexcept {
        if (this != &other) {
            mCameraDevice = std::exchange(other.mCameraDevice, nullptr);
            mStateCallbacks = std::move(other.mStateCallbacks);
            mStateCallbacks->getStateCallbacks().context = this;
        }
        return *this;
    }


    std::optional<CaptureRequest> CameraDevice::createCaptureRequest() {
        ACaptureRequest *captureRequest;
        camera_status_t status = ACameraDevice_createCaptureRequest(mCameraDevice, TEMPLATE_PREVIEW, &captureRequest);
        if (status != ACAMERA_OK || captureRequest == nullptr) {
            LOG_E("ACameraDevice_createCaptureRequest() Failed, status: %d, captureRequest:%p", status, captureRequest);
            return std::nullopt;
        }
        return CaptureRequest(captureRequest);
    }

    std::unique_ptr<CaptureRequest> CameraDevice::createUniqueCaptureRequest() {
        std::optional<CaptureRequest> captureRequest = createCaptureRequest();
        if (!captureRequest.has_value()) {
            return nullptr;
        }
        return std::make_unique<CaptureRequest>(std::move(captureRequest.value()));
    }

    std::optional<CameraCaptureSession> CameraDevice::createCaptureSession(const CaptureSessionOutputContainer &captureSessionOutputContainer) {
        ACaptureSessionOutputContainer *outputs = captureSessionOutputContainer.getCaptureSessionOutputContainer();
        std::unique_ptr<CameraCaptureSessionStateCallbacks> stateCallbacks = CameraCaptureSession::createUniqueStateCallbacks();

        ACameraCaptureSession *captureSession;
        camera_status_t status = ACameraDevice_createCaptureSession(mCameraDevice, outputs, &(stateCallbacks->getStateCallbacks()), &captureSession);
        if (status != ACAMERA_OK || captureSession == nullptr) {
            LOG_E("ACameraDevice_createCaptureSession() Failed, status: %d, captureSession:%p", status, captureSession);
            return std::nullopt;
        }

        return CameraCaptureSession(captureSession, std::move(stateCallbacks));
    }

    std::unique_ptr<CameraCaptureSession> CameraDevice::createUniqueCaptureSession(const CaptureSessionOutputContainer &captureSessionOutputContainer) {
        std::optional<CameraCaptureSession> cameraCaptureSession = createCaptureSession(captureSessionOutputContainer);
        if (cameraCaptureSession.has_value()) {
            return std::make_unique<CameraCaptureSession>(std::move(cameraCaptureSession.value()));
        } else {
            return nullptr;
        }
    }

    std::optional<CameraCaptureSession> CameraDevice::createCaptureSessionWithSessionParameters(const CaptureSessionOutputContainer &captureSessionOutputContainer,
                                                                                                const CaptureRequest &captureRequest) {
        ACameraCaptureSession *captureSession;
        std::unique_ptr<CameraCaptureSessionStateCallbacks> stateCallbacks = CameraCaptureSession::createUniqueStateCallbacks();

        camera_status_t status = ACameraDevice_createCaptureSessionWithSessionParameters(mCameraDevice,
                                                                                         captureSessionOutputContainer.getCaptureSessionOutputContainer(),
                                                                                         captureRequest.getCaptureRequest(),
                                                                                         &(stateCallbacks->getStateCallbacks()),
                                                                                         &captureSession);
        if (status != ACAMERA_OK || captureSession == nullptr) {
            LOG_E("ACameraDevice_createCaptureSessionWithSessionParameters() Failed,status: %d, captureSession:%p", status, captureSession);
            return std::nullopt;
        }
        return CameraCaptureSession(captureSession, std::move(stateCallbacks));
    }

    std::unique_ptr<CameraCaptureSession> CameraDevice::createUniqueCaptureSessionWithSessionParameters(const CaptureSessionOutputContainer &captureSessionOutputContainer,
                                                                                                        const CaptureRequest &captureRequest) {
        std::optional<CameraCaptureSession> captureSession = createCaptureSessionWithSessionParameters(captureSessionOutputContainer, captureRequest);
        if (!captureSession.has_value()) {
            return nullptr;
        }
        return std::make_unique<CameraCaptureSession>(std::move(captureSession.value()));
    }

    void CameraDevice::onDisconnected() {
        LOG_D("CameraDevice::onDisconnected(), this:%p", this);
    }

    void CameraDevice::onError(int error) {
        LOG_D("CameraDevice::onError(error:%d), this:%p", error, this);
    }


    // static methods

    std::unique_ptr<CameraDeviceStateCallbacks> CameraDevice::createUniqueStateCallbacks() {
        std::unique_ptr<CameraDeviceStateCallbacks> callbacks = std::make_unique<CameraDeviceStateCallbacks>();

        ACameraDevice_StateCallbacks &stateCallbacks = callbacks->getStateCallbacks();
        stateCallbacks.onDisconnected = onDisconnected;
        stateCallbacks.onError = onError;

        return callbacks;
    }

    void CameraDevice::onDisconnected(void *context, ACameraDevice *device) {
        CameraDevice *cameraDevice = static_cast<CameraDevice *>(context);
        cameraDevice->onDisconnected();
    }

    void CameraDevice::onError(void *context, ACameraDevice *device, int error) {
        CameraDevice *cameraDevice = static_cast<CameraDevice *>(context);
        cameraDevice->onError(error);
    }

} // ndkcamera