//
// Created by leixing on 2025/4/27.
//

#include "AndroidSurfaceBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    AndroidSurfaceBuilder::AndroidSurfaceBuilder()
            : mFlags(vk::AndroidSurfaceCreateFlagBitsKHR{}),
              mNativeWindow(nullptr) {}

    AndroidSurfaceBuilder::~AndroidSurfaceBuilder() = default;

    AndroidSurfaceBuilder &AndroidSurfaceBuilder::instance(vk::Instance instance) {
        mInstance = instance;
        return *this;
    }

    AndroidSurfaceBuilder &AndroidSurfaceBuilder::flags(vk::AndroidSurfaceCreateFlagsKHR flags) {
        mFlags = flags;
        return *this;
    }

    AndroidSurfaceBuilder &AndroidSurfaceBuilder::nativeWindow(ANativeWindow *nativeWindow) {
        mNativeWindow = nativeWindow;
        return *this;
    }

    [[nodiscard]]
    std::optional<Surface> AndroidSurfaceBuilder::build() const {

        vk::AndroidSurfaceCreateInfoKHR createInfo{};
        createInfo
                .setFlags(mFlags)
                .setWindow(mNativeWindow);

        try {
            vk::SurfaceKHR surface = mInstance.createAndroidSurfaceKHR(createInfo);
//            return std::optional<Surface>(Surface(mInstance, surface));
            return Surface(mInstance, surface);
        } catch (vk::SystemError &err) {
            LOG_EF("Failed to create Android surface: {}", err.what());
            return std::nullopt;
        }
    };

} // vklite