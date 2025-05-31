//
// Created by leixing on 2025/4/18.
//

#include "PipelineLayout.h"

#include <vulkan/vulkan.hpp>

namespace vklite {

    PipelineLayout::PipelineLayout(vk::Device device, vk::PipelineLayout pipelineLayout, PipelineLayoutMeta &&meta)
            : mDevice(device), mPipelineLayout(pipelineLayout), mMeta(std::move(meta)) {}

    PipelineLayout::~PipelineLayout() {
        if (mDevice != nullptr && mPipelineLayout != nullptr) {
            mDevice.destroy(mPipelineLayout);
            mDevice = nullptr;
            mPipelineLayout = nullptr;
        }
    }

    PipelineLayout::PipelineLayout(PipelineLayout &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mPipelineLayout(std::exchange(other.mPipelineLayout, nullptr)),
              mMeta(std::move(other.mMeta)) {}

    PipelineLayout &PipelineLayout::operator=(PipelineLayout &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mPipelineLayout = std::exchange(other.mPipelineLayout, nullptr);
            mMeta = std::move(other.mMeta);
        }
        return *this;
    }

    const vk::PipelineLayout &PipelineLayout::getPipelineLayout() const {
        return mPipelineLayout;
    }

    const std::vector<vk::DescriptorSetLayout> &PipelineLayout::getDescriptorSetLayouts() const {
        return mMeta.getDescriptorSetLayouts();
    }

    const std::vector<vk::PushConstantRange> &PipelineLayout::getPushConstantRanges() const {
        return mMeta.getPushConstantRanges();
    }

} // vklite