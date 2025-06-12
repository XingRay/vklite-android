//
// Created by leixing on 2025/1/20.
//

#include "CameraMetadata.h"


#include <utility>
#include <format>
#include <media/NdkImage.h>

#include "ndk_camera/Log.h"
#include "ndk_camera/image/ImageFormat.h"

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

    HardwareLevel::Value CameraMetadata::querySupportedHardwareLevel() const {
        // 检查相机硬件支持级别
        ACameraMetadata_const_entry hardwareLevelEntry = {};
        camera_status_t status = ACameraMetadata_getConstEntry(mMetadata, ACAMERA_INFO_SUPPORTED_HARDWARE_LEVEL, &hardwareLevelEntry);
        if (status != ACAMERA_OK) {
            LOG_E("ACameraMetadata_getConstEntry() Failed, status:%d", status);
            throw std::runtime_error("querySupportedHardwareLevel(): ACameraMetadata_getConstEntry failed");
        }

//        LOG_D("Camera hardware level: %d", hardwareLevelEntry.data.u8[0]);
        uint8_t supportedHardwareLevel = hardwareLevelEntry.data.u8[0];
        return HardwareLevel::of(supportedHardwareLevel).value();
    }

    CameraLensFacing::Value CameraMetadata::queryCameraLensFacing() const {
        ACameraMetadata_const_entry lensFacingEntry = {};
        camera_status_t status = ACameraMetadata_getConstEntry(mMetadata, ACAMERA_LENS_FACING, &lensFacingEntry);
        if (status != ACAMERA_OK) {
            LOG_E("Failed to get lens facing");
            throw std::runtime_error("CameraMetadata::queryCameraLensFacing(): ACameraMetadata_getConstEntry failed");
        }
        uint8_t cameraLensFacing = lensFacingEntry.data.u8[0];
        return CameraLensFacing::of(cameraLensFacing).value();
    }

    std::vector<Range> CameraMetadata::queryCameraFpsRanges() const {
        // 查询支持的 FPS 范围
        std::vector<Range> ranges;
        camera_status_t status;

        ACameraMetadata_const_entry entry = {};
        status = ACameraMetadata_getConstEntry(mMetadata, ACAMERA_CONTROL_AE_AVAILABLE_TARGET_FPS_RANGES, &entry);

        if (status != ACAMERA_OK || entry.count == 0) {
            LOG_D("Device does not support FPS range query");
            return ranges;
        }

        // 遍历 FPS 范围列表
        for (uint32_t i = 0; i < entry.count; i += 2) {
            int32_t minFps = entry.data.i32[i];
            int32_t maxFps = entry.data.i32[i + 1];

            ranges.emplace_back(minFps, maxFps);
        }

        return ranges;
    }

    std::vector<StreamConfiguration> CameraMetadata::queryScalerAvailableStreamConfigurations() const {
        // 查询支持的流配置
        ACameraMetadata_const_entry entry;
        camera_status_t status = ACameraMetadata_getConstEntry(mMetadata, ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS, &entry);
        if (status != ACAMERA_OK) {
            throw std::runtime_error("ACameraMetadata_getConstEntry failed");
        }

        std::string info;
        info = std::format("\tACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS, count:{}\n", entry.count);

        std::vector<StreamConfiguration> configures;
        configures.reserve(entry.count);

        for (int i = 0; i < entry.count; i += 4) {
            // 图像格式（上表对应值）
            int32_t format = entry.data.i32[i];
            // width：分辨率宽度（像素）
            int32_t width = entry.data.i32[i + 1];
            // height：分辨率高度（像素）
            int32_t height = entry.data.i32[i + 2];
            //0 = 输出流，1 = 输入流）
            int32_t io = entry.data.i32[i + 3];

            configures.emplace_back(ImageFormat::of(format).value(), width, height, StreamType::of(io).value());
        }

        return configures;
    }

} // ndkcamera