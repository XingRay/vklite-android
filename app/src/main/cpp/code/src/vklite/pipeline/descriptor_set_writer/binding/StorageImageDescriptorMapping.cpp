//
// Created by leixing on 2025-06-24.
//

#include "StorageImageDescriptorMapping.h"

namespace vklite {

    StorageImageDescriptorMapping::StorageImageDescriptorMapping()
            : mBinding(0), mDescriptorIndex(0) {}

    StorageImageDescriptorMapping::~StorageImageDescriptorMapping() = default;

    StorageImageDescriptorMapping::StorageImageDescriptorMapping(StorageImageDescriptorMapping &&other) noexcept
            : mBinding(other.mBinding),
              mDescriptorIndex(other.mDescriptorIndex),
              mDescriptorImageInfos(std::move(other.mDescriptorImageInfos)) {}

    StorageImageDescriptorMapping &StorageImageDescriptorMapping::operator=(StorageImageDescriptorMapping &&other) noexcept {
        if (this != &other) {
            mBinding = other.mBinding;
            mDescriptorIndex = other.mDescriptorIndex;
            mDescriptorImageInfos = std::move(other.mDescriptorImageInfos);
        }
        return *this;
    }

    uint32_t StorageImageDescriptorMapping::getBinding() const {
        return mBinding;
    }

    uint32_t StorageImageDescriptorMapping::getDescriptorIndex() const {
        return mDescriptorIndex;
    }

    std::vector<vk::DescriptorImageInfo> &StorageImageDescriptorMapping::getDescriptorImageInfos() {
        return mDescriptorImageInfos;
    }

    StorageImageDescriptorMapping &StorageImageDescriptorMapping::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    StorageImageDescriptorMapping &StorageImageDescriptorMapping::descriptorIndex(uint32_t descriptorIndex) {
        mDescriptorIndex = descriptorIndex;
        return *this;
    }

    StorageImageDescriptorMapping &StorageImageDescriptorMapping::addImageInfo(vk::DescriptorImageInfo imageInfo) {
        mDescriptorImageInfos.push_back(imageInfo);
        return *this;
    }

    StorageImageDescriptorMapping &StorageImageDescriptorMapping::addImageInfo(vk::ImageView imageView, vk::ImageLayout imageLayout) {
        addImageInfo({nullptr, imageView, imageLayout});
        return *this;
    }

    StorageImageDescriptorMapping &StorageImageDescriptorMapping::addImageInfo(const ImageView &imageView, vk::ImageLayout imageLayout) {
        addImageInfo(imageView.getVkImageView(), imageLayout);
        return *this;
    }

} // vklite