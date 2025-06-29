//
// Created by leixing on 2025/6/7.
//

#include "CombinedImageSamplerDescriptorMapping.h"

namespace vklite {

    CombinedImageSamplerDescriptorMapping::CombinedImageSamplerDescriptorMapping()
            : mBinding(0), mDescriptorIndex(0) {}

    CombinedImageSamplerDescriptorMapping::~CombinedImageSamplerDescriptorMapping() = default;

    CombinedImageSamplerDescriptorMapping::CombinedImageSamplerDescriptorMapping(CombinedImageSamplerDescriptorMapping &&other) noexcept
            : mBinding(other.mBinding),
              mDescriptorIndex(other.mDescriptorIndex),
              mDescriptorImageInfos(std::move(other.mDescriptorImageInfos)) {}

    CombinedImageSamplerDescriptorMapping &CombinedImageSamplerDescriptorMapping::operator=(CombinedImageSamplerDescriptorMapping &&other) noexcept {
        if (this != &other) {
            mBinding = other.mBinding;
            mDescriptorIndex = other.mDescriptorIndex;
            mDescriptorImageInfos = std::move(other.mDescriptorImageInfos);
        }
        return *this;
    }

    uint32_t CombinedImageSamplerDescriptorMapping::getBinding() const {
        return mBinding;
    }

    uint32_t CombinedImageSamplerDescriptorMapping::getDescriptorIndex() const {
        return mDescriptorIndex;
    }

    std::vector<vk::DescriptorImageInfo> &CombinedImageSamplerDescriptorMapping::getDescriptorImageInfos() {
        return mDescriptorImageInfos;
    }

    CombinedImageSamplerDescriptorMapping &CombinedImageSamplerDescriptorMapping::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    CombinedImageSamplerDescriptorMapping &CombinedImageSamplerDescriptorMapping::descriptorIndex(uint32_t descriptorIndex) {
        mDescriptorIndex = descriptorIndex;
        return *this;
    }

    CombinedImageSamplerDescriptorMapping &CombinedImageSamplerDescriptorMapping::addImageInfo(vk::DescriptorImageInfo imageInfo) {
        mDescriptorImageInfos.push_back(imageInfo);
        return *this;
    }

    CombinedImageSamplerDescriptorMapping &CombinedImageSamplerDescriptorMapping::addImageInfo(vk::Sampler sampler, vk::ImageView imageView, vk::ImageLayout imageLayout) {
        addImageInfo(vk::DescriptorImageInfo{sampler, imageView, imageLayout});
        return *this;
    }

    CombinedImageSamplerDescriptorMapping &CombinedImageSamplerDescriptorMapping::addImageInfo(const Sampler &sampler, const ImageView &imageView, vk::ImageLayout imageLayout) {
        addImageInfo(sampler.getVkSampler(), imageView.getVkImageView(), imageLayout);
        return *this;
    }

} // vklite