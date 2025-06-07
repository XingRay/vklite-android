//
// Created by leixing on 2025/6/7.
//

#include "StorageDescriptorMapping.h"

namespace vklite {

    StorageDescriptorMapping::StorageDescriptorMapping()
            : mBinding(0), mDescriptorIndex(0) {}

    StorageDescriptorMapping::~StorageDescriptorMapping() = default;

    StorageDescriptorMapping::StorageDescriptorMapping(StorageDescriptorMapping &&other) noexcept
            : mBinding(other.mBinding),
              mDescriptorIndex(other.mDescriptorIndex),
              mDescriptorBufferInfos(std::move(other.mDescriptorBufferInfos)) {}

    StorageDescriptorMapping &StorageDescriptorMapping::operator=(StorageDescriptorMapping &&other) noexcept {
        if (this != &other) {
            mBinding = other.mBinding;
            mDescriptorIndex = other.mDescriptorIndex;
            mDescriptorBufferInfos = std::move(other.mDescriptorBufferInfos);
        }
        return *this;
    }

    uint32_t StorageDescriptorMapping::getBinding() const {
        return mBinding;
    }

    uint32_t StorageDescriptorMapping::getDescriptorIndex() const {
        return mDescriptorIndex;
    }

    std::vector<vk::DescriptorBufferInfo> &StorageDescriptorMapping::getDescriptorBufferInfos() {
        return mDescriptorBufferInfos;
    }

    StorageDescriptorMapping &StorageDescriptorMapping::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    StorageDescriptorMapping &StorageDescriptorMapping::descriptorIndex(uint32_t descriptorIndex) {
        mDescriptorIndex = descriptorIndex;
        return *this;
    }

    // add buffer info
    StorageDescriptorMapping &StorageDescriptorMapping::addBufferInfo(vk::DescriptorBufferInfo bufferInfo) {
        mDescriptorBufferInfos.push_back(bufferInfo);
        return *this;
    }

    StorageDescriptorMapping &StorageDescriptorMapping::addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range) {
        addBufferInfo({buffer, offset, range});
        return *this;
    }

    StorageDescriptorMapping &StorageDescriptorMapping::addBufferInfo(const Buffer &buffer) {
        addBufferInfo({buffer.getVkBuffer(), 0, buffer.getSize()});
        return *this;
    }
    
} // vklite