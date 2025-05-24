//
// Created by leixing on 2025/5/24.
//

#pragma once

#include <memory>

#include "camera/NdkCameraDevice.h"

namespace ndkcamera {

    class CameraDeviceStateCallbacks {
    private:
        ACameraDevice_StateCallbacks *mStateCallbacks;

    public:

        CameraDeviceStateCallbacks();

        ~CameraDeviceStateCallbacks();

        CameraDeviceStateCallbacks(const CameraDeviceStateCallbacks &other) = delete;

        CameraDeviceStateCallbacks &operator=(const CameraDeviceStateCallbacks &other) = delete;

        CameraDeviceStateCallbacks(CameraDeviceStateCallbacks &&other) noexcept;

        CameraDeviceStateCallbacks &operator=(CameraDeviceStateCallbacks &&other) noexcept;

        ACameraDevice_StateCallbacks& getStateCallbacks();
    };

} // ndkcamera
