//
// Created by leixing on 2025/6/7.
//

#include "StorageBufferDescriptorMapping.h"

namespace vklite {

    StorageBufferDescriptorMapping::StorageBufferDescriptorMapping()
            : mBinding(0), mDescriptorIndex(0) {}

    StorageBufferDescriptorMapping::~StorageBufferDescriptorMapping() = default;

    StorageBufferDescriptorMapping::StorageBufferDescriptorMapping(StorageBufferDescriptorMapping &&other) noexcept
            : mBinding(other.mBinding),
              mDescriptorIndex(other.mDescriptorIndex),
              mDescriptorBufferInfos(std::move(other.mDescriptorBufferInfos)) {}

    StorageBufferDescriptorMapping &StorageBufferDescriptorMapping::operator=(StorageBufferDescriptorMapping &&other) noexcept {
        if (this != &other) {
            mBinding = other.mBinding;
            mDescriptorIndex = other.mDescriptorIndex;
            mDescriptorBufferInfos = std::move(other.mDescriptorBufferInfos);
        }
        return *this;
    }

    uint32_t StorageBufferDescriptorMapping::getBinding() const {
        return mBinding;
    }

    uint32_t StorageBufferDescriptorMapping::getDescriptorIndex() const {
        return mDescriptorIndex;
    }

    std::vector<vk::DescriptorBufferInfo> &StorageBufferDescriptorMapping::getDescriptorBufferInfos() {
        return mDescriptorBufferInfos;
    }

    StorageBufferDescriptorMapping &StorageBufferDescriptorMapping::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    StorageBufferDescriptorMapping &StorageBufferDescriptorMapping::descriptorIndex(uint32_t descriptorIndex) {
        mDescriptorIndex = descriptorIndex;
        return *this;
    }

    // add buffer info
    StorageBufferDescriptorMapping &StorageBufferDescriptorMapping::addBufferInfo(vk::DescriptorBufferInfo bufferInfo) {
        mDescriptorBufferInfos.push_back(bufferInfo);
        return *this;
    }

    StorageBufferDescriptorMapping &StorageBufferDescriptorMapping::addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range) {
        addBufferInfo({buffer, offset, range});
        return *this;
    }

    StorageBufferDescriptorMapping &StorageBufferDescriptorMapping::addBufferInfo(vk::Buffer buffer, vk::DeviceSize range) {
        addBufferInfo({buffer, 0, range});
        return *this;
    }

    StorageBufferDescriptorMapping &StorageBufferDescriptorMapping::addBufferInfo(const Buffer &buffer) {
        addBufferInfo({buffer.getVkBuffer(), 0, buffer.getSize()});
        return *this;
    }

} // vklite