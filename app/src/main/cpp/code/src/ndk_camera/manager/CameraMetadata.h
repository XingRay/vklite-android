//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <optional>
#include <vector>

#include "camera/NdkCameraMetadata.h"

#include "ndk_camera/manager/HardwareLevel.h"
#include "ndk_camera/manager/CameraLensFacing.h"
#include "ndk_camera/common/Range.h"
#include "ndk_camera/manager/StreamConfiguration.h"

namespace ndkcamera {

    class CameraMetadata {
    private:
        const char *mId;
        ACameraMetadata *mMetadata;

    public:
        explicit CameraMetadata(const char *id, ACameraMetadata *metadata);

        ~CameraMetadata();

        CameraMetadata(const CameraMetadata &other) = delete;

        CameraMetadata &operator=(const CameraMetadata &other) = delete;

        CameraMetadata(CameraMetadata &&other) noexcept;

        CameraMetadata &operator=(CameraMetadata &&other) noexcept;


        [[nodiscard]]
        const char *getId() const;

        [[nodiscard]]
        HardwareLevel::Value querySupportedHardwareLevel() const;

        [[nodiscard]]
        CameraLensFacing::Value queryCameraLensFacing() const;

        [[nodiscard]]
        std::vector<Range> queryCameraFpsRanges() const;

        [[nodiscard]]
        std::vector<StreamConfiguration> queryScalerAvailableStreamConfigurations() const;

    };

} // ndkcamera
