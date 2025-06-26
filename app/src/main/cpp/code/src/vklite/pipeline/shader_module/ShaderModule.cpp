//
// Created by leixing on 2025/3/10.
//

#include "ShaderModule.h"

#include <utility>

namespace vklite {

    ShaderModule::ShaderModule(vk::Device device, vk::ShaderModule shaderModule)
            : mDevice(device), mShaderModule(shaderModule) {}

    ShaderModule::~ShaderModule() {
        if (mDevice != nullptr && mShaderModule != nullptr) {
            mDevice.destroy(mShaderModule);
            mDevice = nullptr;
            mShaderModule = nullptr;
        }
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

    const vk::ShaderModule &ShaderModule::getVkShaderModule() const {
        return mShaderModule;
    }

} // vklite