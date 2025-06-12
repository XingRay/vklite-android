//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <vector>
#include <string>
#include <functional>
#include <optional>
#include <memory>

#include <camera/NdkCameraManager.h>

#include "CameraMetadata.h"
#include "ndk_camera/device/CameraDevice.h"

namespace ndkcamera {

    class CameraManager {
    private:
        ACameraManager *mCameraManager;

    public:
        CameraManager();

        ~CameraManager();

        CameraManager(const CameraManager &other) = delete;

        CameraManager &operator=(const CameraManager &other) = delete;

        CameraManager(CameraManager &&other) noexcept;

        CameraManager &operator=(CameraManager &&other) noexcept;


        std::vector<const char *> getCameraIdList();

        CameraMetadata queryCameraMetadata(const char *cameraId);

        std::unique_ptr<CameraMetadata> queryUniqueCameraMetadata(const char *cameraId);

        std::vector<CameraMetadata> queryCameraMetadataList();

        std::unique_ptr<CameraDevice> openCamera(const char *cameraId);
    };

} // ndkcamera
