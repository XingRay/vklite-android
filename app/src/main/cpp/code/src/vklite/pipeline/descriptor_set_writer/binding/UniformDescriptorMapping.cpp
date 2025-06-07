//
// Created by leixing on 2025/6/7.
//

#include "UniformDescriptorMapping.h"

namespace vklite {

    UniformDescriptorMapping::UniformDescriptorMapping()
            : mBinding(0), mDescriptorIndex(0) {}

    UniformDescriptorMapping::~UniformDescriptorMapping() = default;

    UniformDescriptorMapping::UniformDescriptorMapping(UniformDescriptorMapping &&other) noexcept
            : mBinding(other.mBinding),
              mDescriptorIndex(other.mDescriptorIndex),
              mDescriptorBufferInfos(std::move(other.mDescriptorBufferInfos)) {}

    UniformDescriptorMapping &UniformDescriptorMapping::operator=(UniformDescriptorMapping &&other) noexcept {
        if (this != &other) {
            mBinding = other.mBinding;
            mDescriptorIndex = other.mDescriptorIndex;
            mDescriptorBufferInfos = std::move(other.mDescriptorBufferInfos);
        }
        return *this;
    }

    uint32_t UniformDescriptorMapping::getBinding() const {
        return mBinding;
    }

    uint32_t UniformDescriptorMapping::getDescriptorIndex() const {
        return mDescriptorIndex;
    }

    std::vector<vk::DescriptorBufferInfo> &UniformDescriptorMapping::getDescriptorBufferInfos() {
        return mDescriptorBufferInfos;
    }

    UniformDescriptorMapping &UniformDescriptorMapping::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    UniformDescriptorMapping &UniformDescriptorMapping::descriptorIndex(uint32_t descriptorIndex) {
        mDescriptorIndex = descriptorIndex;
        return *this;
    }

    // add buffer info
    UniformDescriptorMapping &UniformDescriptorMapping::addBufferInfo(vk::DescriptorBufferInfo bufferInfo) {
        mDescriptorBufferInfos.push_back(bufferInfo);
        return *this;
    }

    UniformDescriptorMapping &UniformDescriptorMapping::addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range) {
        addBufferInfo({buffer, offset, range});
        return *this;
    }

    UniformDescriptorMapping &UniformDescriptorMapping::addBufferInfo(const Buffer &buffer) {
        addBufferInfo({buffer.getVkBuffer(), 0, buffer.getSize()});
        return *this;
    }

} // vklite