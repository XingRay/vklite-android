//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CameraManager.h"
#include "ndk_camera/Log.h"

#include <utility>

namespace ndkcamera {

    CameraManager::CameraManager()
            : mCameraManager(ACameraManager_create()) {}

    CameraManager::~CameraManager() {
        ACameraManager_delete(mCameraManager);
    }

    CameraManager::CameraManager(CameraManager &&other) noexcept
            : mCameraManager(std::exchange(other.mCameraManager, nullptr)) {}

    CameraManager &CameraManager::operator=(CameraManager &&other) noexcept {
        if (this != &other) {
            mCameraManager = std::exchange(other.mCameraManager, nullptr);
        }
        return *this;
    }

    std::vector<const char *> CameraManager::getCameraIdList() {
        std::vector<const char *> cameraIds;

        ACameraIdList *cameraIdList = nullptr;
        camera_status_t status = ACameraManager_getCameraIdList(mCameraManager, &cameraIdList);
        if (status != ACAMERA_OK || cameraIdList == nullptr) {
            LOG_E("ACameraManager_getCameraIdList() Failed, status:%d, cameraIdList:%p", status, cameraIdList);
            return cameraIds;
        }

        if (cameraIdList->numCameras == 0) {
            LOG_E("cameraIdList->numCameras == 0, no camera found");
            return cameraIds;
        }

        for (int i = 0; i < cameraIdList->numCameras; i++) {
            cameraIds.push_back(cameraIdList->cameraIds[i]);
        }
        return cameraIds;
    }

    std::optional<CameraMetadata> CameraManager::queryCameraMetadata(const char *cameraId) {
        ACameraMetadata *metadata;
        camera_status_t status = ACameraManager_getCameraCharacteristics(mCameraManager, cameraId, &metadata);
        if (status != ACAMERA_OK || metadata == nullptr) {
            LOG_E("ACameraManager_getCameraCharacteristics() Failed, status:%d, metadata:%p", status, metadata);
            return std::nullopt;
        }
        return CameraMetadata(cameraId, metadata);
    }

    std::unique_ptr<CameraMetadata> CameraManager::queryUniqueCameraMetadata(const char *cameraId) {
        std::optional<CameraMetadata> cameraMetadata = queryCameraMetadata(cameraId);
        if (!cameraMetadata.has_value()) {
            return nullptr;
        }
        return std::make_unique<CameraMetadata>(std::move(cameraMetadata.value()));
    }

    std::vector<CameraMetadata> CameraManager::queryCameraMetadataList() {
        std::vector<CameraMetadata> cameraMetadataList;

        std::vector<const char *> ids = getCameraIdList();
        for (const char *id: ids) {
            std::optional<CameraMetadata> metaData = queryCameraMetadata(id);
            if (!metaData.has_value()) {
                LOG_E("Failed to get camera characteristics");
                continue;
            }
            cameraMetadataList.push_back(std::move(metaData.value()));
        }
        return cameraMetadataList;
    }

    std::unique_ptr<CameraDevice> CameraManager::openCamera(const char *cameraId) {
        ACameraDevice *device;
        ACameraDevice_StateCallbacks *callbacks = new ACameraDevice_StateCallbacks();
        callbacks->onDisconnected = CameraDevice::onDisconnected;
        callbacks->onError = CameraDevice::onError;

        camera_status_t status = ACameraManager_openCamera(mCameraManager, cameraId, callbacks, &device);
        if (status != ACAMERA_OK || device == nullptr) {
            LOG_E("Failed to open camera: %d", status);
            return nullptr; // 返回空指针表示失败
        }
        LOG_D("Device opened successfully: %p", device);
        return std::make_unique<CameraDevice>(device, callbacks);
    }

} // ndkcamera