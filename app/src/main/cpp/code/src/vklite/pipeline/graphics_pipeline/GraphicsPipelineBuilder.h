//
// Created by leixing on 2025/3/7.
//

#pragma once

#include "GraphicsPipeline.h"
#include "vklite/pipeline/vertex/VertexConfigure.h"
#include "vklite/index_buffer/IndexBufferBuilder.h"
#include "vklite/configure/push_constants/PushConstantConfigures.h"
#include "vklite/pipeline/descriptor/PipelineLayoutConfigure.h"
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

        /**
         * descriptor sets
         */
        PipelineLayoutConfigure mPipelineLayoutConfigure;

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

        /**
         *
         * DescriptorSet
         *
         */
        GraphicsPipelineBuilder &addDescriptorSet(const std::function<void(DescriptorSetConfigure &)> &configure);


        /**
         * push constant
         */
        GraphicsPipelineBuilder &addPushConstant(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits);

        [[nodiscard]]
        std::unique_ptr<GraphicsPipeline> build(const Device &device,
                                                const Swapchain &swapchain,
                                                const RenderPass &renderPass);
    };

} // vklite
