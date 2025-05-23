//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CameraMetadata.h"
#include "ndk_camera/Log.h"

#include <utility>

namespace ndkcamera {

    CameraMetadata::CameraMetadata(const char *id, ACameraMetadata *metadata)
            : mId(id), mMetadata(metadata) {}

    CameraMetadata::~CameraMetadata() {
        ACameraMetadata_free(mMetadata);
    }

    CameraMetadata::CameraMetadata(CameraMetadata &&other) noexcept
            : mId(other.mId),
              mMetadata(std::exchange(other.mMetadata, nullptr)) {}

    CameraMetadata &CameraMetadata::operator=(CameraMetadata &&other) noexcept {
        if (this != &other) {
            mId = other.mId;
            mMetadata = std::exchange(other.mMetadata, nullptr);
        }
        return *this;
    }


    const char *CameraMetadata::getId() const {
        return mId;
    }

    std::optional<SupportedHardwareLevel> CameraMetadata::querySupportedHardwareLevel() const {
        // 检查相机硬件支持级别
        ACameraMetadata_const_entry hardwareLevelEntry = {};
        camera_status_t status = ACameraMetadata_getConstEntry(mMetadata, ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL, &hardwareLevelEntry);
        if (status != ACAMERA_OK) {
            LOG_E("ACameraMetadata_getConstEntry() Failed, status:%d", status);
            return std::nullopt;
        }

        LOG_D("Camera hardware level: %d", hardwareLevelEntry.data.u8[0]);
        uint8_t supportedHardwareLevel = hardwareLevelEntry.data.u8[0];
        // 将 uint8_t 转换为 SupportedHardwareLevel 枚举
        switch (supportedHardwareLevel) {
            case 0:
                return SupportedHardwareLevel::LIMITED;
            case 1:
                return SupportedHardwareLevel::FULL;
            case 2:
                return SupportedHardwareLevel::LEGACY;
            case 3:
                return SupportedHardwareLevel::LEVEL_3;
            case 4:
                return SupportedHardwareLevel::EXTERNAL;
            default:
                LOG_E("Unknown hardware level: %d", supportedHardwareLevel);
                return std::nullopt;
        }
    }

    std::optional<CameraLensFacing> CameraMetadata::queryCameraLensFacing() const {
        ACameraMetadata_const_entry lensFacingEntry = {};
        camera_status_t status = ACameraMetadata_getConstEntry(mMetadata, ACAMERA_LENS_FACING, &lensFacingEntry);
        if (status != ACAMERA_OK) {
            LOG_E("Failed to get lens facing");
            return std::nullopt;
        }

        switch (lensFacingEntry.data.u8[0]) {
            case ACAMERA_LENS_FACING_FRONT:
                return CameraLensFacing::FRONT;

            case ACAMERA_LENS_FACING_BACK:
                return CameraLensFacing::BACK;

            case ACAMERA_LENS_FACING_EXTERNAL:
                return CameraLensFacing::EXTERNAL;

            default:
                LOG_E("Unknown lensFacing: %d", lensFacingEntry.data.u8[0]);
                return std::nullopt;
        }
    }

    std::vector<CameraFpsRange> CameraMetadata::queryCameraFpsRanges() const {
        // 查询支持的 FPS 范围
        std::vector<CameraFpsRange> ranges;
        camera_status_t status;

        ACameraMetadata_const_entry entry = {};
        status = ACameraMetadata_getConstEntry(
                mMetadata,
                ACAMERA_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES,
                &entry
        );

        if (status != ACAMERA_OK || entry.count == 0) {
            LOG_D("Device does not support FPS range query");
            return ranges;
        }

        // 遍历 FPS 范围列表
        for (uint32_t i = 0; i < entry.count; i += 2) {
            int32_t minFps = entry.data.i32[i];
            int32_t maxFps = entry.data.i32[i + 1];
            LOG_D("Supported FPS range: [%d, %d]", minFps, maxFps);

            ranges.emplace_back(CameraFpsRange(minFps, maxFps));
        }

        return ranges;
    }

} // ndkcamera