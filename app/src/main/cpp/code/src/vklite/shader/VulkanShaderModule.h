//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <string>

#include "vklite/device/Device.h"

namespace vklite {

    class VulkanShaderModule {
        const Device &mDevice;

        vk::ShaderModule mShaderModule;

    public:
        VulkanShaderModule(const Device &device, const std::vector<char> &code);

        ~VulkanShaderModule();

        const vk::ShaderModule &getShaderModule() const;
    };

} // vklite
