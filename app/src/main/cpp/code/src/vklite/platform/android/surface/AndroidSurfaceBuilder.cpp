//
// Created by leixing on 2025/4/27.
//

#include "AndroidSurfaceBuilder.h"
#include "vklite/Log.h"
#include <utility>

namespace vklite {

    AndroidSurfaceBuilder::AndroidSurfaceBuilder() {
        mSurfaceCreateInfo = vk::AndroidSurfaceCreateInfoKHR{};
        mSurfaceCreateInfo
                .setFlags(vk::AndroidSurfaceCreateFlagBitsKHR{})
                .setWindow(nullptr);
    }

    AndroidSurfaceBuilder::~AndroidSurfaceBuilder() = default;

    AndroidSurfaceBuilder::AndroidSurfaceBuilder(AndroidSurfaceBuilder &&other) noexcept
            : mInstance(std::exchange(other.mInstance, nullptr)),
              mSurfaceCreateInfo(other.mSurfaceCreateInfo) {
        other.mSurfaceCreateInfo = vk::AndroidSurfaceCreateInfoKHR{};
    }

    AndroidSurfaceBuilder &AndroidSurfaceBuilder::operator=(AndroidSurfaceBuilder &&other) noexcept {
        if (this != &other) {
            mInstance = std::exchange(other.mInstance, nullptr);
            mSurfaceCreateInfo = other.mSurfaceCreateInfo;

            other.mSurfaceCreateInfo = vk::AndroidSurfaceCreateInfoKHR{};
        }
        return *this;
    }


    AndroidSurfaceBuilder &AndroidSurfaceBuilder::instance(vk::Instance instance) {
        mInstance = instance;
        return *this;
    }

    AndroidSurfaceBuilder &AndroidSurfaceBuilder::flags(vk::AndroidSurfaceCreateFlagsKHR flags) {
        mSurfaceCreateInfo.setFlags(flags);
        return *this;
    }

    AndroidSurfaceBuilder &AndroidSurfaceBuilder::nativeWindow(ANativeWindow *nativeWindow) {
        mSurfaceCreateInfo.setWindow(nativeWindow);
        return *this;
    }

    [[nodiscard]]
    std::optional<Surface> AndroidSurfaceBuilder::build() const {
        try {
            vk::SurfaceKHR surface = mInstance.createAndroidSurfaceKHR(mSurfaceCreateInfo);
//            return std::optional<Surface>(Surface(mInstance, surface));
            return Surface(mInstance, surface);
        } catch (vk::SystemError &err) {
            LOG_EF("Failed to create Android surface: {}", err.what());
            return std::nullopt;
        }
    };

} // vklite