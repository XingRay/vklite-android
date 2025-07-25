//
// Created by leixing on 2025/1/20.
//

#include "CameraManager.h"
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

    CameraMetadata CameraManager::queryCameraMetadata(const char *cameraId) {
        ACameraMetadata *metadata;
        camera_status_t status = ACameraManager_getCameraCharacteristics(mCameraManager, cameraId, &metadata);
        if (status != ACAMERA_OK || metadata == nullptr) {
            LOG_E("ACameraManager_getCameraCharacteristics() Failed, status:%d, metadata:%p", status, metadata);
            throw std::runtime_error("ACameraManager_getCameraCharacteristics failed");
        }
        return CameraMetadata(cameraId, metadata);
    }

    std::unique_ptr<CameraMetadata> CameraManager::queryUniqueCameraMetadata(const char *cameraId) {
        return std::make_unique<CameraMetadata>(queryCameraMetadata(cameraId));
    }

    std::vector<CameraMetadata> CameraManager::queryCameraMetadataList() {
        std::vector<CameraMetadata> cameraMetadataList;

        std::vector<const char *> ids = getCameraIdList();
        for (const char *id: ids) {
            CameraMetadata metaData = queryCameraMetadata(id);
            cameraMetadataList.push_back(std::move(metaData));
        }
        return cameraMetadataList;
    }

    std::unique_ptr<CameraDevice> CameraManager::openCamera(const char *cameraId) {
        ACameraDevice *device;
        std::unique_ptr<CameraDeviceStateCallbacks> stateCallbacks = CameraDevice::createUniqueStateCallbacks();

        camera_status_t status = ACameraManager_openCamera(mCameraManager, cameraId, &(stateCallbacks->getStateCallbacks()), &device);
        if (status != ACAMERA_OK || device == nullptr) {
            LOG_E("ACameraManager_openCamera() Failed, status:%d, device:%p", status, device);
            return nullptr; // 返回空指针表示失败
        }
        LOG_D("Device opened successfully: %p", device);
        return std::make_unique<CameraDevice>(device, std::move(stateCallbacks));
    }

} // ndkcamera