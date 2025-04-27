//
// Created by leixing on 2024/12/22.
//
#ifdef ANDROID

#include "AndroidSurface.h"
#include "vklite/Log.h"

namespace vklite {
    AndroidSurface::AndroidSurface(const Instance &instance, ANativeWindow *window)
            : mInstance(instance) {
        LOG_D("AndroidSurface::AndroidSurface");
        vk::AndroidSurfaceCreateInfoKHR createInfo{};
        createInfo
                .setFlags(vk::AndroidSurfaceCreateFlagsKHR{})
                .setWindow(window);

        try {
            mSurface = instance.getInstance().createAndroidSurfaceKHR(createInfo);
        } catch (vk::SystemError &err) {
            LOG_E("Failed to create Android surface: %s", std::string(err.what()).data());
            throw std::runtime_error("Failed to create Android surface: " + std::string(err.what()));
        }
    }

    AndroidSurface::~AndroidSurface() {
        LOG_D("AndroidSurface::~AndroidSurface()");
        if (mSurface != nullptr) {
            mInstance.getInstance().destroy(mSurface);
        } else {
            LOG_W("surface is null");
        }
    }

} // vklite

#endif