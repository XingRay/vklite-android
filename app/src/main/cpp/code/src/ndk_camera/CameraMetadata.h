//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <optional>
#include <vector>

#include "camera/NdkCameraMetadata.h"

namespace ndkcamera {

    // acamera_metadata_enum_acamera_info_supported_hardware_level
    enum class SupportedHardwareLevel {
        //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_LIMITED                    = 0,
        LIMITED,

        //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_FULL                       = 1,
        FULL,

        //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY                     = 2,
        LEGACY,

        //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_3                          = 3,
        LEVEL_3,

        //ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL_EXTERNAL                   = 4,
        EXTERNAL,
    };

    enum class CameraLensFacing {
        // acamera_metadata_enum_acamera_lens_facing#ACAMERA_LENS_FACING_FRONT= 0,
        FRONT,

        // acamera_metadata_enum_acamera_lens_facing#ACAMERA_LENS_FACING_BACK= 1,
        BACK,

        // acamera_metadata_enum_acamera_lens_facing#ACAMERA_LENS_FACING_EXTERNAL= 2,
        EXTERNAL,
    };

    class CameraFpsRange {
    public:
        uint32_t min;
        uint32_t max;
    };

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
        std::optional<SupportedHardwareLevel> querySupportedHardwareLevel() const;

        [[nodiscard]]
        std::optional<CameraLensFacing> queryCameraLensFacing() const;

        [[nodiscard]]
        std::vector<CameraFpsRange> queryCameraFpsRanges() const;
    };

} // ndkcamera
