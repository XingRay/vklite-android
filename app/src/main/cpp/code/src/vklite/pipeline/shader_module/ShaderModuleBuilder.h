//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/shader_module/ShaderModule.h"

namespace vklite {

    class ShaderModuleBuilder {
    private:
        vk::Device mDevice;
        std::vector<uint32_t> mCode;

    public:
        ShaderModuleBuilder();

        ~ShaderModuleBuilder();

        ShaderModuleBuilder &device(vk::Device device);

        ShaderModuleBuilder &code(std::vector<uint32_t>&& code);

        ShaderModule build();

        std::unique_ptr<ShaderModule> buildUnique();
    };

} // vklite
