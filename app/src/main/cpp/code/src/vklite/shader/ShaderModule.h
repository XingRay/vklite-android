//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <string>

#include "vklite/device/Device.h"

namespace vklite {

    class ShaderModule {
        vk::Device mDevice;
        vk::ShaderModule mShaderModule;

    public:
        ShaderModule(vk::Device device, const std::vector<uint32_t> &code);

        ShaderModule(const Device &device, const std::vector<uint32_t> &code);

        ~ShaderModule();

        ShaderModule(const ShaderModule &other) = delete;

        ShaderModule &operator=(const ShaderModule &other) = delete;

        ShaderModule(ShaderModule &&other) noexcept;

        ShaderModule &operator=(ShaderModule &&other) noexcept;

        const vk::ShaderModule &getShaderModule() const;
    };

} // vklite
