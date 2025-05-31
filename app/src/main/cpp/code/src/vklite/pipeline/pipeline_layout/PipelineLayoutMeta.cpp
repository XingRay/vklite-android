//
// Created by leixing on 2025/5/31.
//

#include "PipelineLayoutMeta.h"

namespace vklite {

    PipelineLayoutMeta::PipelineLayoutMeta(std::vector<vk::DescriptorSetLayout> &&descriptorSetLayouts,
                                           std::vector<vk::PushConstantRange> &&pushConstantRanges)
            : mDescriptorSetLayouts(std::move(descriptorSetLayouts)),
              mPushConstantRanges(std::move(pushConstantRanges)) {}


    PipelineLayoutMeta::~PipelineLayoutMeta() = default;

    PipelineLayoutMeta::PipelineLayoutMeta(const PipelineLayoutMeta &other) = default;

    PipelineLayoutMeta &PipelineLayoutMeta::operator=(const PipelineLayoutMeta &other) {
        if (this != &other) {
            mDescriptorSetLayouts = other.mDescriptorSetLayouts;
            mPushConstantRanges = other.mPushConstantRanges;
        }
        return *this;
    }

    PipelineLayoutMeta::PipelineLayoutMeta(PipelineLayoutMeta &&other) noexcept
            : mDescriptorSetLayouts(std::move(other.mDescriptorSetLayouts)),
              mPushConstantRanges(std::move(other.mPushConstantRanges)) {}

    PipelineLayoutMeta &PipelineLayoutMeta::operator=(PipelineLayoutMeta &&other) noexcept {
        if (this != &other) {
            mDescriptorSetLayouts = std::move(other.mDescriptorSetLayouts);
            mPushConstantRanges = std::move(other.mPushConstantRanges);
        }
        return *this;
    }

    const std::vector<vk::DescriptorSetLayout> &PipelineLayoutMeta::getDescriptorSetLayouts() const {
        return mDescriptorSetLayouts;
    }

    const std::vector<vk::PushConstantRange> &PipelineLayoutMeta::getPushConstantRanges() const {
        return mPushConstantRanges;
    }

} // vklite