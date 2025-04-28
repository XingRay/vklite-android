//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "vklite/device/Device.h"
#include "VulkanImageInterface.h"

namespace vklite {

    class VulkanImageBuilderInterface {
    public:
        VulkanImageBuilderInterface();

        virtual ~VulkanImageBuilderInterface() = 0;

        virtual std::unique_ptr<VulkanImageInterface> build(const Device &vulkanDevice, uint32_t width, uint32_t height, vk::Format format) = 0;
    };

} // vklite
