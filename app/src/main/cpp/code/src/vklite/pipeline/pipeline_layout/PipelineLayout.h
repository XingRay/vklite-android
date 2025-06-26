//
// Created by leixing on 2025/4/18.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class PipelineLayout {
    private:
        vk::Device mDevice;
        vk::PipelineLayout mPipelineLayout;

    public:
        explicit PipelineLayout(vk::Device device, vk::PipelineLayout pipelineLayout);

        ~PipelineLayout();

        PipelineLayout(const PipelineLayout &other) = delete;

        PipelineLayout &operator=(const PipelineLayout &other) = delete;

        PipelineLayout(PipelineLayout &&other) noexcept;

        PipelineLayout &operator=(PipelineLayout &&other) noexcept;

        [[nodiscard]]
        const vk::PipelineLayout &getVkPipelineLayout() const;

//        [[nodiscard]]
//        const std::vector<vk::DescriptorSetLayout> &getDescriptorSetLayouts() const;
//
//        [[nodiscard]]
//        const std::vector<vk::PushConstantRange> &getPushConstantRanges() const;
    };

} // vklite
