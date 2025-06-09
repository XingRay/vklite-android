//
// Created by leixing on 2025/6/9.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class HardwareBufferImageMeta {
    public:
        vk::Format format;
        vk::Extent3D extent;
        uint32_t mipLevels;
    };

} // vklite
