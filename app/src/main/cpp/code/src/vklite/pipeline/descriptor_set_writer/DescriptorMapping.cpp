//
// Created by leixing on 2025/5/12.
//

#include "DescriptorMapping.h"

#include <utility>

namespace vklite {

    DescriptorMapping::DescriptorMapping()
            : mBinding(0), mDescriptorIndex(0), mDescriptorType(vk::DescriptorType::eUniformBuffer) {}

    DescriptorMapping::~DescriptorMapping() = default;

    DescriptorMapping::DescriptorMapping(DescriptorMapping &&other) noexcept
            : mBinding(other.mBinding),
              mDescriptorIndex(other.mDescriptorIndex),
              mDescriptorType(other.mDescriptorType),
              mBufferInfos(std::move(other.mBufferInfos)),
              mImageInfos(std::move(other.mImageInfos)) {}

    DescriptorMapping &DescriptorMapping::operator=(DescriptorMapping &&other) noexcept {
        if (this != &other) {
            mBinding = other.mBinding;
            mDescriptorIndex = other.mDescriptorIndex;
            mDescriptorType = other.mDescriptorType;
            mBufferInfos = std::move(other.mBufferInfos);
            mImageInfos = std::move(other.mImageInfos);
        }
        return *this;
    }

    uint32_t DescriptorMapping::getBinding() const {
        return mBinding;
    }

    uint32_t DescriptorMapping::getDescriptorIndex() const {
        return mDescriptorIndex;
    }

    uint32_t DescriptorMapping::getDescriptorCount() const {
        if (!mBufferInfos.empty()) {
            return mBufferInfos.size();
        }
        if (!mImageInfos.empty()) {
            return mImageInfos.size();
        }
        throw std::runtime_error("DescriptorMapping::getDescriptorCount(): no bufferInfos and imageInfos");
    }

    vk::DescriptorType DescriptorMapping::getVkDescriptorType() const {
        return mDescriptorType;
    }

    const std::vector<vk::DescriptorBufferInfo> &DescriptorMapping::getDescriptorBufferInfos() const {
        return mBufferInfos;
    }

    const std::vector<vk::DescriptorImageInfo> &DescriptorMapping::getDescriptorImageInfos() const {
        return mImageInfos;
    }

//    DescriptorMapping &DescriptorMapping::descriptorSet(vk::DescriptorSet descriptorSet) {
//        mDescriptorSet = descriptorSet;
//        return *this;
//    }

    DescriptorMapping &DescriptorMapping::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    DescriptorMapping &DescriptorMapping::descriptorIndex(uint32_t descriptorIndex) {
        mDescriptorIndex = descriptorIndex;
        return *this;
    }

    DescriptorMapping &DescriptorMapping::descriptorType(vk::DescriptorType descriptorType) {
        mDescriptorType = descriptorType;
        return *this;
    }

    DescriptorMapping &DescriptorMapping::addBufferInfo(vk::DescriptorBufferInfo bufferInfo) {
        mBufferInfos.push_back(bufferInfo);
        return *this;
    }

    DescriptorMapping &DescriptorMapping::addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range) {
        addBufferInfo({buffer, offset, range});
        return *this;
    }

    DescriptorMapping &DescriptorMapping::addBufferInfo(const Buffer &buffer) {
        addBufferInfo({buffer.getVkBuffer(), 0, buffer.getSize()});
        return *this;
    }

    DescriptorMapping &DescriptorMapping::bufferInfos(std::vector<vk::DescriptorBufferInfo> &&bufferInfos) {
        mBufferInfos = std::move(bufferInfos);
        return *this;
    }

    DescriptorMapping &DescriptorMapping::addImageInfo(vk::DescriptorImageInfo imageInfo) {
        mImageInfos.push_back(imageInfo);
        return *this;
    }

    DescriptorMapping &DescriptorMapping::addImageInfo(vk::Sampler sampler, vk::ImageView imageView, vk::ImageLayout imageLayout) {
        addImageInfo({sampler, imageView, imageLayout});
        return *this;
    }

    DescriptorMapping &DescriptorMapping::addImageInfo(const Sampler &sampler, const ImageView &imageView, vk::ImageLayout imageLayout) {
        addImageInfo(sampler.getVkSampler(), imageView.getVkImageView(), imageLayout);
        return *this;
    }

    DescriptorMapping &DescriptorMapping::imageInfos(std::vector<vk::DescriptorImageInfo> &&imageInfos) {
        mImageInfos = std::move(imageInfos);
        return *this;
    }

} // vklite