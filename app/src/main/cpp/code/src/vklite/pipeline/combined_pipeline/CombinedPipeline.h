//
// Created by leixing on 2025-06-19.
//

#pragma once

#include "vklite/pipeline/pipeline/Pipeline.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayout.h"
#include "vklite/pipeline/descriptor_set_layout/DescriptorSetLayouts.h"
#include "vklite/push_constant/PushConstant.h"

namespace vklite {

    class CombinedPipeline {
    private:
        Pipeline mPipeline;
        DescriptorSetLayouts mDescriptorSetLayouts;
        PipelineLayout mPipelineLayout;

        // frameIndex -> set -> descriptorSet
        std::vector<std::vector<vk::DescriptorSet>> mDescriptorSets;
        std::vector<PushConstant> mPushConstants;

    public:
        explicit CombinedPipeline(Pipeline &&pipeline,
                                  DescriptorSetLayouts &&descriptorSetLayouts,
                                  PipelineLayout &&pipelineLayout,
                                  std::vector<std::vector<vk::DescriptorSet>> &&descriptorSets,
                                  std::vector<PushConstant> &&pushConstants);

        ~CombinedPipeline();

        CombinedPipeline(const CombinedPipeline &other) = delete;

        CombinedPipeline &operator=(const CombinedPipeline &other) = delete;

        CombinedPipeline(CombinedPipeline &&other) noexcept;

        CombinedPipeline &operator=(CombinedPipeline &&other) noexcept;

        [[nodiscard]]
        const std::vector<std::vector<vk::DescriptorSet>> &getDescriptorSets() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorSet> &getDescriptorSets(uint32_t frameIndex) const;

        [[nodiscard]]
        const vk::DescriptorSet &getDescriptorSet(uint32_t frameIndex, uint32_t set) const;

        [[nodiscard]]
        const Pipeline &getPipeline() const;

        [[nodiscard]]
        const vk::Pipeline &getVkPipeline() const;

        [[nodiscard]]
        const PipelineLayout &getPipelineLayout();

        [[nodiscard]]
        const vk::PipelineLayout &getVkPipelineLayout();

        [[nodiscard]]
        std::vector<PushConstant> &getPushConstants();
    };

} // vklite
