//
// Created by leixing on 2024/12/28.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class ComputePipeline {
    private:
        vk::Device mDevice;
        vk::Pipeline mPipeline;

    public:
        ComputePipeline(vk::Device device, vk::Pipeline pipeline);

        ~ComputePipeline();

        ComputePipeline(const ComputePipeline &computePipeline) = delete;

        ComputePipeline &operator=(const ComputePipeline &other) = delete;

        ComputePipeline(ComputePipeline &&other) noexcept;

        ComputePipeline &operator=(ComputePipeline &&other) noexcept;

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;
    };

} // vklite
