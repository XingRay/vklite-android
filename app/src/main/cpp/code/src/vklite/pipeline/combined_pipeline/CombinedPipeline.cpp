//
// Created by leixing on 2025-06-19.
//

#include "CombinedPipeline.h"

namespace vklite {

    CombinedPipeline::CombinedPipeline(Pipeline &&pipeline,
                                       DescriptorSetLayouts &&descriptorSetLayouts,
                                       PipelineLayout &&pipelineLayout,
                                       std::vector<std::vector<vk::DescriptorSet>> &&descriptorSets,
                                       std::vector<PushConstant> &&pushConstants)
            : mPipeline(std::move(pipeline)),
              mDescriptorSetLayouts(std::move(descriptorSetLayouts)),
              mPipelineLayout(std::move(pipelineLayout)),
              mDescriptorSets(std::move(descriptorSets)),
              mPushConstants(std::move(pushConstants)) {}

    CombinedPipeline::~CombinedPipeline() = default;

    CombinedPipeline::CombinedPipeline(CombinedPipeline &&other) noexcept
            : mPipeline(std::move(other.mPipeline)),
              mDescriptorSetLayouts(std::move(other.mDescriptorSetLayouts)),
              mPipelineLayout(std::move(other.mPipelineLayout)),
              mDescriptorSets(std::move(other.mDescriptorSets)),
              mPushConstants(std::move(other.mPushConstants)) {}

    CombinedPipeline &CombinedPipeline::operator=(CombinedPipeline &&other) noexcept {
        if (this != &other) {
            mPipeline = std::move(other.mPipeline);
            mDescriptorSetLayouts = std::move(other.mDescriptorSetLayouts);
            mPipelineLayout = std::move(other.mPipelineLayout);
            mDescriptorSets = std::move(other.mDescriptorSets);
            mPushConstants = std::move(other.mPushConstants);
        }
        return *this;
    }

    const std::vector<std::vector<vk::DescriptorSet>> &CombinedPipeline::getDescriptorSets() const {
        return mDescriptorSets;
    }

    const std::vector<vk::DescriptorSet> &CombinedPipeline::getDescriptorSets(uint32_t frameIndex) const {
        return mDescriptorSets[frameIndex];
    }

    const vk::DescriptorSet &CombinedPipeline::getDescriptorSet(uint32_t frameIndex, uint32_t set) const {
        return mDescriptorSets[frameIndex][set];
    }

    const Pipeline &CombinedPipeline::getPipeline() const {
        return mPipeline;
    }

    const vk::Pipeline &CombinedPipeline::getVkPipeline() const {
        return mPipeline.getVkPipeline();
    }


    const PipelineLayout &CombinedPipeline::getPipelineLayout() {
        return mPipelineLayout;
    }

    const vk::PipelineLayout &CombinedPipeline::getVkPipelineLayout() {
        return mPipelineLayout.getVkPipelineLayout();
    }

    std::vector<PushConstant> &CombinedPipeline::getPushConstants() {
        return mPushConstants;
    }

} // vklite