//
// Created by leixing on 2025/3/7.
//

#pragma once

#include "GraphicsPipeline.h"
#include "vklite/pipeline/vertex/VertexConfigure.h"
#include "vklite/index_buffer/IndexBufferBuilder.h"
#include "vklite/configure/push_constants/PushConstantConfigures.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayoutBuilder.h"
#include "vklite/pipeline/vertex/VertexBindingConfigure.h"
#include "vklite/pipeline/descriptor/DescriptorSetConfigure.h"

namespace vklite {

    class GraphicsPipelineBuilder {

    private:
        /**
         * shader code
         */
        std::vector<char> mVertexShaderCode;
        std::vector<char> mFragmentShaderCode;

        /**
         * vertex
         */
        VertexConfigure mVertexConfigure;

    public:
        explicit GraphicsPipelineBuilder();

        ~GraphicsPipelineBuilder();

        /**
         *
         * shader code
         *
         */
        GraphicsPipelineBuilder &vertexShaderCode(std::vector<char> &&code);

        GraphicsPipelineBuilder &fragmentShaderCode(std::vector<char> &&code);


        /**
         * vertex
         */
        GraphicsPipelineBuilder &addVertexBinding(const std::function<void(VertexBindingConfigure &)> &configure);

        GraphicsPipelineBuilder &addVertexBinding(VertexBindingConfigure &&configure);

        [[nodiscard]]
        GraphicsPipeline build(const Device &device,
                               const RenderPass &renderPass,
                               const PipelineLayout &pipelineLayout,
                               const std::vector<vk::Viewport> &viewports,
                               const std::vector<vk::Rect2D> &scissors);

        [[nodiscard]]
        std::unique_ptr<GraphicsPipeline> buildUnique(const Device &device,
                                                      const RenderPass &renderPass,
                                                      const PipelineLayout &pipelineLayout,
                                                      const std::vector<vk::Viewport> &viewports,
                                                      const std::vector<vk::Rect2D> &scissors);
    };

} // vklite
