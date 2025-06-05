//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class PipelineLayoutMeta {
    private:
        std::vector<vk::DescriptorSetLayout> mDescriptorSetLayouts;
        std::vector<vk::PushConstantRange> mPushConstantRanges;

    public:
        PipelineLayoutMeta(std::vector<vk::DescriptorSetLayout> &&descriptorSetLayouts,
                           std::vector<vk::PushConstantRange> &&pushConstantRanges);

        ~PipelineLayoutMeta();

        PipelineLayoutMeta(const PipelineLayoutMeta &other);

        PipelineLayoutMeta &operator=(const PipelineLayoutMeta &other);

        PipelineLayoutMeta(PipelineLayoutMeta &&other) noexcept;

        PipelineLayoutMeta &operator=(PipelineLayoutMeta &&other) noexcept;

        [[nodiscard]]
        const std::vector<vk::DescriptorSetLayout> &getDescriptorSetLayouts() const;

        [[nodiscard]]
        const std::vector<vk::PushConstantRange> &getPushConstantRanges() const;

    };

} // vklite