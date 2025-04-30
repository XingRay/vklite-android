//
// Created by leixing on 2025/3/10.
//

#include "VulkanShaderModule.h"

namespace vklite {
    VulkanShaderModule::VulkanShaderModule(const Device &device, const std::vector<char> &code)
            : mDevice(device) {
        vk::ShaderModuleCreateInfo createInfo;
        createInfo
                .setCodeSize(code.size())
                .setPCode(reinterpret_cast<const uint32_t *>(code.data()));

        mShaderModule = device.getDevice().createShaderModule(createInfo);
    }

    VulkanShaderModule::~VulkanShaderModule() {
        mDevice.getDevice().destroy(mShaderModule);
    }

    const vk::ShaderModule &VulkanShaderModule::getShaderModule() const {
        return mShaderModule;
    }

} // vklite