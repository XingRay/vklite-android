//
// Created by leixing on 2025/1/14.
//

#include "VertexBindingConfigure.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    VertexBindingConfigure::VertexBindingConfigure()
            : mBinding(0),
              mStride(0),
              mVertexInputRate(vk::VertexInputRate::eVertex),
              mAttributes({}),
              mCurrentAttributeLocation(-1),
              mCurrentAttributeOffset(0) {}

    VertexBindingConfigure::~VertexBindingConfigure() = default;

    VertexBindingConfigure::VertexBindingConfigure(const VertexBindingConfigure &other) = default;

    VertexBindingConfigure &VertexBindingConfigure::operator=(const VertexBindingConfigure &other) {
        if (this != &other) {
            mBinding = other.mBinding;
            mStride = other.mStride;
            mVertexInputRate = other.mVertexInputRate;
            mAttributes = other.mAttributes;
            mCurrentAttributeLocation = other.mCurrentAttributeLocation;
            mCurrentAttributeOffset = other.mCurrentAttributeOffset;
        }
        return *this;
    }

    VertexBindingConfigure::VertexBindingConfigure(VertexBindingConfigure &&other) noexcept
            : mBinding(other.mBinding),
              mStride(other.mStride),
              mVertexInputRate(other.mVertexInputRate),
              mAttributes(std::move(other.mAttributes)),
              mCurrentAttributeLocation(other.mCurrentAttributeLocation),
              mCurrentAttributeOffset(other.mCurrentAttributeOffset) {}

    VertexBindingConfigure &VertexBindingConfigure::operator=(VertexBindingConfigure &&other) noexcept {
        if (this != &other) {
            mBinding = other.mBinding;
            mStride = other.mStride;
            mVertexInputRate = other.mVertexInputRate;
            mAttributes = std::move(other.mAttributes);
            mCurrentAttributeLocation = other.mCurrentAttributeLocation;
            mCurrentAttributeOffset = other.mCurrentAttributeOffset;
        }
        return *this;
    }

    uint32_t VertexBindingConfigure::getBinding() const {
        return mBinding;
    }

    // todo: calc stride by attributes size and align
    uint32_t VertexBindingConfigure::getStride() const {
        return mStride;
    }

    vk::VertexInputRate VertexBindingConfigure::getVertexInputRate() const {
        return mVertexInputRate;
    }

    const std::unordered_map<uint32_t, VertexAttributeConfigure> &VertexBindingConfigure::getAttributes() const {
        return mAttributes;
    }

    VertexBindingConfigure &VertexBindingConfigure::binding(uint32_t binding) {
        mBinding = binding;
        return *this;
    }

    VertexBindingConfigure &VertexBindingConfigure::stride(uint32_t stride) {
        mStride = stride;
        return *this;
    }

    VertexBindingConfigure &VertexBindingConfigure::inputRate(vk::VertexInputRate inputRate) {
        mVertexInputRate = inputRate;
        return *this;
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset) {
        mCurrentAttributeLocation = location;
        mAttributes.emplace(location, VertexAttributeConfigure{format, offset});
        mCurrentAttributeOffset += VulkanUtil::getFormatSize(format);

        return *this;
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset) {
        return addAttribute(location, binding, ShaderFormatHelper::toVkFormat(format), offset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, ShaderFormat format, uint32_t offset) {
        return addAttribute(location, mBinding, format, offset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, vk::Format format, uint32_t offset) {
        return addAttribute(location, mBinding, format, offset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, ShaderFormat format) {
        return addAttribute(location, mBinding, format, mCurrentAttributeOffset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(uint32_t location, vk::Format format) {
        return addAttribute(location, mBinding, format, mCurrentAttributeOffset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(ShaderFormat format) {
        return addAttribute(mCurrentAttributeLocation + 1, mBinding, format, mCurrentAttributeOffset);
    }

    VertexBindingConfigure &VertexBindingConfigure::addAttribute(vk::Format format) {
        return addAttribute(mCurrentAttributeLocation + 1, mBinding, format, mCurrentAttributeOffset);
    }

} // vklite