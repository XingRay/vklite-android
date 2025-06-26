//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <string>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class ShaderModule {
    private:
        vk::Device mDevice;
        vk::ShaderModule mShaderModule;

    public:
        explicit ShaderModule(vk::Device device, vk::ShaderModule shaderModule);

        ~ShaderModule();

        ShaderModule(const ShaderModule &other) = delete;

        ShaderModule &operator=(const ShaderModule &other) = delete;

        ShaderModule(ShaderModule &&other) noexcept;

        ShaderModule &operator=(ShaderModule &&other) noexcept;

        [[nodiscard]]
        const vk::ShaderModule &getVkShaderModule() const;
    };

} // vklite
