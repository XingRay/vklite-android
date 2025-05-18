//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <string>

#include "vklite/device/Device.h"

namespace vklite {

    class ShaderModule {
        const Device &mDevice;

        vk::ShaderModule mShaderModule;

    public:
        ShaderModule(const Device &device, const std::vector<uint32_t> &code);

        ~ShaderModule();

        const vk::ShaderModule &getShaderModule() const;
    };

} // vklite
