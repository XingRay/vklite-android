//
// Created by leixing on 2025/5/12.
//

#include "DescriptorMapping.h"

namespace vklite {

    DescriptorMapping::DescriptorMapping()
            : mBinding(0), mDescriptorIndex(0), mDescriptorCount(1) {}

    DescriptorMapping::~DescriptorMapping() = default;

    const vk::DescriptorSet &DescriptorMapping::getDescriptorSet() const {
        return mDescriptorSet;
    }

    uint32_t DescriptorMapping::getBinding() const {
        return mBinding;
    }

    uint32_t DescriptorMapping::getDescriptorIndex() const {
        return mDescriptorIndex;
    }

    uint32_t DescriptorMapping::getDescriptorCount() const {
        return mDescriptorCount;
    }

    vk::DescriptorType DescriptorMapping::getDescriptorType() const {
        return mDescriptorType;
    }

    std::optional<std::reference_wrapper<const std::vector<vk::DescriptorBufferInfo>>>
    DescriptorMapping::getDescriptorBufferInfos() const {
        if (auto *ptr = std::get_if<std::vector<vk::DescriptorBufferInfo>>(&mTarget)) {
//            return *ptr; // 隐式转换为 std::optional 包含引用包装
            return std::optional(std::cref(*ptr));
        }
        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const std::vector<vk::DescriptorImageInfo>>>
    DescriptorMapping::getDescriptorImageInfos() const {
        if (auto *ptr = std::get_if<std::vector<vk::DescriptorImageInfo>>(&mTarget)) {
//            return *ptr;
            return std::optional(std::cref(*ptr));
        }
        return std::nullopt;
    }

    DescriptorMapping &DescriptorMapping::descriptorSet(vk::DescriptorSet descriptorSet) {
        mDescriptorSet = descriptorSet;
        return *this;
    }

    DescriptorMapping &DescriptorMapping::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    DescriptorMapping &DescriptorMapping::descriptorIndex(uint32_t descriptorIndex) {
        mDescriptorIndex = descriptorIndex;
        return *this;
    }

    DescriptorMapping &DescriptorMapping::descriptorCount(uint32_t descriptorCount) {
        mDescriptorCount = descriptorCount;
        return *this;
    }

    DescriptorMapping &DescriptorMapping::descriptorType(vk::DescriptorType descriptorType) {
        mDescriptorType = descriptorType;
        return *this;
    }

    DescriptorMapping &DescriptorMapping::addBufferInfo(vk::DescriptorBufferInfo bufferInfo) {
        auto *bufferInfos = std::get_if<std::vector<vk::DescriptorBufferInfo>>(&mTarget);
        if (bufferInfos == nullptr) {
            mTarget = std::vector<vk::DescriptorBufferInfo>{bufferInfo};
        } else {
            bufferInfos->push_back(bufferInfo);
        }
        return *this;
    }

    DescriptorMapping &DescriptorMapping::addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range) {
        addBufferInfo({buffer, offset, range});
        return *this;
    }

    DescriptorMapping &DescriptorMapping::addBufferInfo(const vklite::BufferInterface &bufferInterface) {
        addBufferInfo({bufferInterface.getBuffer(), 0, bufferInterface.getSize()});
        return *this;
    }

    DescriptorMapping &DescriptorMapping::addImageInfo(vk::DescriptorImageInfo imageInfo) {
        auto *imageInfos = std::get_if<std::vector<vk::DescriptorImageInfo>>(&mTarget);
        if (imageInfos == nullptr) {
            mTarget = std::vector<vk::DescriptorImageInfo>{imageInfo};
        } else {
            imageInfos->push_back(imageInfo);
        }
        return *this;
    }

} // vklite