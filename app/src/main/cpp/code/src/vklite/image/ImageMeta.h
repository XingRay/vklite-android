//
// Created by leixing on 2025/5/30.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class ImageMeta {
    public:
        vk::Format format;
        vk::Extent3D extent;
        uint32_t mipLevels;
    };

} // vklite
