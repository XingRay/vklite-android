//
// Created by leixing on 2025/3/10.
//

#include "ShaderModule.h"

#include <utility>

namespace vklite {

    ShaderModule::ShaderModule(vk::Device device, const std::vector<uint32_t> &code)
            : mDevice(device) {
        vk::ShaderModuleCreateInfo createInfo;
        createInfo
                .setCode(code);

        mShaderModule = device.createShaderModule(createInfo);
    }

    ShaderModule::ShaderModule(const Device &device, const std::vector<uint32_t> &code)
            : ShaderModule(device.getDevice(), code) {}

    ShaderModule::~ShaderModule() {
        mDevice.destroy(mShaderModule);
    }

    ShaderModule::ShaderModule(ShaderModule &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mShaderModule(std::exchange(other.mShaderModule, nullptr)) {}

    ShaderModule &ShaderModule::operator=(ShaderModule &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mShaderModule = std::exchange(other.mShaderModule, nullptr);
        }
        return *this;
    }

    const vk::ShaderModule &ShaderModule::getShaderModule() const {
        return mShaderModule;
    }

} // vklite