//
// Created by leixing on 2024/12/28.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class Pipeline {
    private:
        vk::Device mDevice;
        vk::Pipeline mPipeline;

    public:

        Pipeline(vk::Device device, vk::Pipeline pipeline);

        ~Pipeline();

        Pipeline(const Pipeline &other) = delete;

        Pipeline &operator=(const Pipeline &other) = delete;

        Pipeline(Pipeline &&other) noexcept;

        Pipeline &operator=(Pipeline &&other) noexcept;

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

//        void drawFrame(const vk::CommandBuffer &commandBuffer,
//                       const PipelineLayout &pipelineLayout,
//                       const PipelineResource &pipelineResource,
//                       const std::vector<vk::Viewport> &viewports,
//                       const std::vector<vk::Rect2D> &scissors);
    };

} // vklite
