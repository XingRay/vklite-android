//
// Created by leixing on 2025/5/29.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class SwapchainMeta {
    public:
        vk::SurfaceFormatKHR surfaceFormat;
        uint32_t imageCount;
        vk::Extent2D displaySize;
    };

} // vklite
