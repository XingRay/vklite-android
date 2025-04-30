//
// Created by leixing on 2025/4/11.
//

#include "VulkanImageBuilder.h"
#include "VulkanImage.h"

namespace vklite {
    VulkanImageBuilder::VulkanImageBuilder() {

    }

    VulkanImageBuilder::~VulkanImageBuilder() {

    }

    std::unique_ptr<VulkanImageInterface> VulkanImageBuilder::build(const Device &device, uint32_t width, uint32_t height, vk::Format format) {
        return std::make_unique<VulkanImage>(device, width, height, format);
    }
} // vklite