//
// Created by leixing on 2025/6/7.
//

#include "UniformBufferDescriptorMapping.h"

namespace vklite {

    UniformBufferDescriptorMapping::UniformBufferDescriptorMapping()
            : mBinding(0), mDescriptorIndex(0) {}

    UniformBufferDescriptorMapping::~UniformBufferDescriptorMapping() = default;

    UniformBufferDescriptorMapping::UniformBufferDescriptorMapping(UniformBufferDescriptorMapping &&other) noexcept
            : mBinding(other.mBinding),
              mDescriptorIndex(other.mDescriptorIndex),
              mDescriptorBufferInfos(std::move(other.mDescriptorBufferInfos)) {}

    UniformBufferDescriptorMapping &UniformBufferDescriptorMapping::operator=(UniformBufferDescriptorMapping &&other) noexcept {
        if (this != &other) {
            mBinding = other.mBinding;
            mDescriptorIndex = other.mDescriptorIndex;
            mDescriptorBufferInfos = std::move(other.mDescriptorBufferInfos);
        }
        return *this;
    }

    uint32_t UniformBufferDescriptorMapping::getBinding() const {
        return mBinding;
    }

    uint32_t UniformBufferDescriptorMapping::getDescriptorIndex() const {
        return mDescriptorIndex;
    }

    std::vector<vk::DescriptorBufferInfo> &UniformBufferDescriptorMapping::getDescriptorBufferInfos() {
        return mDescriptorBufferInfos;
    }

    UniformBufferDescriptorMapping &UniformBufferDescriptorMapping::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    UniformBufferDescriptorMapping &UniformBufferDescriptorMapping::descriptorIndex(uint32_t descriptorIndex) {
        mDescriptorIndex = descriptorIndex;
        return *this;
    }

    // add buffer info
    UniformBufferDescriptorMapping &UniformBufferDescriptorMapping::addBufferInfo(vk::DescriptorBufferInfo bufferInfo) {
        mDescriptorBufferInfos.push_back(bufferInfo);
        return *this;
    }

    UniformBufferDescriptorMapping &UniformBufferDescriptorMapping::addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range) {
        addBufferInfo({buffer, offset, range});
        return *this;
    }

    UniformBufferDescriptorMapping &UniformBufferDescriptorMapping::addBufferInfo(const Buffer &buffer) {
        addBufferInfo({buffer.getVkBuffer(), 0, buffer.getSize()});
        return *this;
    }

} // vklite