//
// Created by leixing on 2025/5/31.
//

#include "ShaderModuleBuilder.h"

namespace vklite {

    ShaderModuleBuilder::ShaderModuleBuilder() = default;

    ShaderModuleBuilder::~ShaderModuleBuilder() = default;

    ShaderModuleBuilder &ShaderModuleBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    ShaderModuleBuilder &ShaderModuleBuilder::code(std::vector<uint32_t> &&code) {
        mCode = std::move(code);
        return *this;
    }

    ShaderModule ShaderModuleBuilder::build() {
        vk::ShaderModuleCreateInfo createInfo;
        createInfo
                .setCode(mCode);

        vk::ShaderModule shaderModule = mDevice.createShaderModule(createInfo);
        return ShaderModule(mDevice, shaderModule);
    }

    std::unique_ptr<ShaderModule> ShaderModuleBuilder::buildUnique() {
        return std::make_unique<ShaderModule>(build());
    }

} // vklite