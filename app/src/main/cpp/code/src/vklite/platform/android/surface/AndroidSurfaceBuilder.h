//
// Created by leixing on 2025/4/27.
//

#pragma once

#include "android/native_window.h"
#include <vulkan/vulkan.hpp>

#include "vklite/surface/Surface.h"
#include "vklite/surface/SurfaceBuilder.h"

namespace vklite {

    class AndroidSurfaceBuilder : public SurfaceBuilder {
    private:
        vk::Instance mInstance;
        vk::AndroidSurfaceCreateInfoKHR mSurfaceCreateInfo;

    public:
        explicit AndroidSurfaceBuilder();

        ~AndroidSurfaceBuilder() override;

        AndroidSurfaceBuilder(const AndroidSurfaceBuilder &other) = delete;

        AndroidSurfaceBuilder &operator=(const AndroidSurfaceBuilder &other) = delete;

        AndroidSurfaceBuilder(AndroidSurfaceBuilder &&other) noexcept;

        AndroidSurfaceBuilder &operator=(AndroidSurfaceBuilder &&other) noexcept;

        AndroidSurfaceBuilder &instance(vk::Instance instance);

        AndroidSurfaceBuilder &flags(vk::AndroidSurfaceCreateFlagsKHR flags);

        AndroidSurfaceBuilder &nativeWindow(ANativeWindow *nativeWindow);

        [[nodiscard]]
        std::optional<Surface> build() const override;
    };

} // vklite
