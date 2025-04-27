//
// Created by leixing on 2024/12/22.
//
#ifdef ANDROID

#pragma once

#include "android/native_window.h"
#include <vulkan/vulkan.hpp>

#include "vklite/surface/Surface.h"
#include "vklite/instance/Instance.h"

namespace vklite {

    class AndroidSurface : public Surface {

    private:
        const Instance &mInstance;
        vk::SurfaceKHR mSurface;

    public:
        AndroidSurface(const Instance &instance, ANativeWindow *window);

        ~AndroidSurface() override;

        [[nodiscard]]
        vk::SurfaceKHR getSurface() const override { return mSurface; }
    };

} // vklite

#endif