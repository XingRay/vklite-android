//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <string>

#include "vklite/device/Device.h"

namespace vklite {

    class VulkanShaderModule {
        const Device &mVulkanDevice;

        vk::ShaderModule mShaderModule;

    public:
        VulkanShaderModule(const Device &vulkanDevice, const std::vector<char> &code);

        ~VulkanShaderModule();

        const vk::ShaderModule &getShaderModule() const;
    };

} // vklite
