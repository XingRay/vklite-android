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
    Surface AndroidSurfaceBuilder::build() const {
        vk::SurfaceKHR surface = mInstance.createAndroidSurfaceKHR(mSurfaceCreateInfo);
        if (surface == nullptr) {
            throw std::runtime_error("AndroidSurfaceBuilder::build(): createAndroidSurfaceKHR failed");
        }
        return Surface(mInstance, surface);
    };

} // vklite