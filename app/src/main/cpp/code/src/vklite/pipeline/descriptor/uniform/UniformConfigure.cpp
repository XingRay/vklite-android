//
// Created by leixing on 2025/3/24.
//

#include "UniformConfigure.h"
#include "vklite/pipeline/descriptor/old/DescriptorSlotConfigure.h"

namespace vklite {

    UniformConfigure::UniformConfigure()
            : mBinding(0), mDescriptorCount(1), mShaderStageFlags(vk::ShaderStageFlagBits::eVertex) {}

    UniformConfigure::~UniformConfigure() = default;

    UniformConfigure &UniformConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    UniformConfigure &UniformConfigure::shaderStageFlags(vk::ShaderStageFlags shaderStageFlags) {
        mShaderStageFlags = shaderStageFlags;
        return *this;
    }

    UniformConfigure &UniformConfigure::descriptorCount(uint32_t descriptorCount) {
        mDescriptorCount = descriptorCount;
        return *this;
    }

//    UniformConfigure &UniformConfigure::setUniformBuffer(uint32_t capacity, const void *data, uint32_t size) {
//        mDescriptorBufferInfoConfigure = std::make_unique<DescriptorBufferInfoConfigure>(std::make_unique<UniformBufferBuilder>(), capacity, data, size);
//        return *this;
//    }
//
//    UniformConfigure &UniformConfigure::setUniformBuffer(std::unique_ptr<BufferInfo> &&bufferInfo) {
//        mDescriptorBufferInfoConfigure = std::make_unique<DescriptorBufferInfoConfigure>(std::move(bufferInfo));
//        return *this;
//    }

//    vk::DescriptorSetLayoutBinding VulkanUniformConfigure::createDescriptorSetLayoutBinding() const {
//        vk::DescriptorSetLayoutBinding descriptorSetLayoutBinding{};
//
//        descriptorSetLayoutBinding
//                .setBinding(mBinding)
//                .setDescriptorType(vk::DescriptorType::eUniformBuffer)
//                .setDescriptorCount(mDescriptorCount)
//                .setStageFlags(mShaderStageFlags);
//
//        return descriptorSetLayoutBinding;
//    }

//    std::shared_ptr<VulkanDeviceLocalUniformBuffer> VulkanUniformConfigure::createUniformBuffer(const VulkanDevice &device, const CommandPool &commandPool) const {
//        if (mBuffer != nullptr) {
//            return mBuffer;
//        }
//
//        if (mCreateBufferCapacity == 0) {
//            return nullptr;
//        }
//
//        std::shared_ptr<VulkanDeviceLocalUniformBuffer> buffer = std::make_shared<VulkanDeviceLocalUniformBuffer>(device, mCreateBufferCapacity);
//        if (!mCreateBufferData.empty()) {
//            buffer->update(commandPool, mCreateBufferData);
//        }
//
//        return buffer;
//    }

//    std::unique_ptr<VulkanDescriptorBindingConfigure> VulkanUniformConfigure::createVulkanDescriptorSetConfigure() {
//        return std::make_unique<VulkanDescriptorBindingConfigure>(mBinding, vk::DescriptorType::eUniformBuffer,
//                                                                  mDescriptorCount, mShaderStageFlags, );
//    }

    DescriptorBindingConfigure UniformConfigure::createDescriptorBindingConfigure() const {
//        return DescriptorBindingConfigure(mBinding, vk::DescriptorType::eUniformBuffer, mShaderStageFlags, mDescriptorCount);
        return {mBinding, vk::DescriptorType::eUniformBuffer, mShaderStageFlags, mDescriptorCount, {}};
    }

} // vklite