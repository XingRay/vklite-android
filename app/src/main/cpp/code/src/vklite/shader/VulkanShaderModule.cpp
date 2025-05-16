//
// Created by leixing on 2025/3/10.
//

#include "VulkanShaderModule.h"

namespace vklite {

    VulkanShaderModule::VulkanShaderModule(const Device &device, const std::vector<uint32_t> &code)
            : mDevice(device) {
        vk::ShaderModuleCreateInfo createInfo;
        createInfo
                .setCode(code);

        mShaderModule = device.getDevice().createShaderModule(createInfo);
    }

    VulkanShaderModule::~VulkanShaderModule() {
        mDevice.getDevice().destroy(mShaderModule);
    }

    const vk::ShaderModule &VulkanShaderModule::getShaderModule() const {
        return mShaderModule;
    }

} // vklite