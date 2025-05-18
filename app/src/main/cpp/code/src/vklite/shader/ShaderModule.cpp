//
// Created by leixing on 2025/3/10.
//

#include "ShaderModule.h"

namespace vklite {

    ShaderModule::ShaderModule(const Device &device, const std::vector<uint32_t> &code)
            : mDevice(device) {
        vk::ShaderModuleCreateInfo createInfo;
        createInfo
                .setCode(code);

        mShaderModule = device.getDevice().createShaderModule(createInfo);
    }

    ShaderModule::~ShaderModule() {
        mDevice.getDevice().destroy(mShaderModule);
    }

    const vk::ShaderModule &ShaderModule::getShaderModule() const {
        return mShaderModule;
    }

} // vklite