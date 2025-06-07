//
// Created by leixing on 2025/6/7.
//

#include "SamplerDescriptorMapping.h"

namespace vklite {

    SamplerDescriptorMapping::SamplerDescriptorMapping()
            : mBinding(0), mDescriptorIndex(0) {}

    SamplerDescriptorMapping::~SamplerDescriptorMapping() = default;

    SamplerDescriptorMapping::SamplerDescriptorMapping(SamplerDescriptorMapping &&other) noexcept
            : mBinding(other.mBinding),
              mDescriptorIndex(other.mDescriptorIndex),
              mDescriptorImageInfos(std::move(other.mDescriptorImageInfos)) {}

    SamplerDescriptorMapping &SamplerDescriptorMapping::operator=(SamplerDescriptorMapping &&other) noexcept {
        if (this != &other) {
            mBinding = other.mBinding;
            mDescriptorIndex = other.mDescriptorIndex;
            mDescriptorImageInfos = std::move(other.mDescriptorImageInfos);
        }
        return *this;
    }

    uint32_t SamplerDescriptorMapping::getBinding() const {
        return mBinding;
    }

    uint32_t SamplerDescriptorMapping::getDescriptorIndex() const {
        return mDescriptorIndex;
    }

    std::vector<vk::DescriptorImageInfo> &SamplerDescriptorMapping::getDescriptorImageInfos() {
        return mDescriptorImageInfos;
    }

    SamplerDescriptorMapping &SamplerDescriptorMapping::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    SamplerDescriptorMapping &SamplerDescriptorMapping::descriptorIndex(uint32_t descriptorIndex) {
        mDescriptorIndex = descriptorIndex;
        return *this;
    }

    SamplerDescriptorMapping &SamplerDescriptorMapping::addImageInfo(vk::DescriptorImageInfo imageInfo) {
        mDescriptorImageInfos.push_back(imageInfo);
        return *this;
    }

    SamplerDescriptorMapping &SamplerDescriptorMapping::addImageInfo(vk::Sampler sampler, vk::ImageView imageView, vk::ImageLayout imageLayout) {
        addImageInfo({sampler, imageView, imageLayout});
        return *this;
    }

    SamplerDescriptorMapping &SamplerDescriptorMapping::addImageInfo(const Sampler &sampler, const ImageView &imageView, vk::ImageLayout imageLayout) {
        addImageInfo(sampler.getSampler(), imageView.getImageView(), imageLayout);
        return *this;
    }
    
} // vklite