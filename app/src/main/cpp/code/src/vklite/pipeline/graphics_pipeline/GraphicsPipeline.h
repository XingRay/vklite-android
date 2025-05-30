//
// Created by leixing on 2024/12/28.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class GraphicsPipeline {
    private:
        vk::Device mDevice;
        vk::Pipeline mPipeline;

    public:

        GraphicsPipeline(vk::Device device, vk::Pipeline pipeline);

        ~GraphicsPipeline();

        GraphicsPipeline(const GraphicsPipeline &other) = delete;

        GraphicsPipeline &operator=(const GraphicsPipeline &other) = delete;

        GraphicsPipeline(GraphicsPipeline &&other) noexcept;

        GraphicsPipeline &operator=(GraphicsPipeline &&other) noexcept;

        [[nodiscard]]
        const vk::Pipeline &getPipeline() const;

//        void drawFrame(const vk::CommandBuffer &commandBuffer,
//                       const PipelineLayout &pipelineLayout,
//                       const PipelineResource &pipelineResource,
//                       const std::vector<vk::Viewport> &viewports,
//                       const std::vector<vk::Rect2D> &scissors);
    };

} // vklite
