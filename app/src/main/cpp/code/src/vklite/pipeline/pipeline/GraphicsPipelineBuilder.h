//
// Created by leixing on 2025/3/7.
//

#pragma once

#include <vector>
#include <cstdint>

#include "Pipeline.h"
#include "vklite/pipeline/vertex/VertexConfigure.h"
#include "vklite/index_buffer/IndexBufferBuilder.h"
#include "vklite/pipeline/push_constants/PushConstantConfigures.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayoutBuilder.h"
#include "vklite/pipeline/vertex/VertexBindingConfigure.h"
#include "vklite/pipeline/descriptor/DescriptorSetConfigure.h"

namespace vklite {

    class GraphicsPipelineBuilder {

    private:

        vk::Device mDevice;
        vk::RenderPass mRenderPass;
        vk::PipelineLayout mPipelineLayout;
        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;

        /**
         * shader code
         */
        std::vector<uint32_t> mVertexShaderCode;
        std::vector<uint32_t> mFragmentShaderCode;

        /**
         * vertex
         */
        VertexConfigure mVertexConfigure;

        // msaa
        bool mSampleShadingEnable;
        vk::SampleCountFlagBits mSampleCount;

        // depth & stencil
        bool mDepthTestEnable;

    public:
        explicit GraphicsPipelineBuilder();

        ~GraphicsPipelineBuilder();

        GraphicsPipelineBuilder &device(const vk::Device &device);

        GraphicsPipelineBuilder &renderPass(const vk::RenderPass &renderPass);

        GraphicsPipelineBuilder &pipelineLayout(const vk::PipelineLayout &pipelineLayout);

        GraphicsPipelineBuilder &viewports(const std::vector<vk::Viewport> &viewports);

        GraphicsPipelineBuilder &viewports(std::vector<vk::Viewport> &&viewports);

        GraphicsPipelineBuilder &scissors(const std::vector<vk::Rect2D> &scissors);

        GraphicsPipelineBuilder &scissors(std::vector<vk::Rect2D> &&scissors);

        /**
         *
         * shader code
         *
         */
        GraphicsPipelineBuilder &vertexShaderCode(std::vector<uint32_t> &&code);

        GraphicsPipelineBuilder &fragmentShaderCode(std::vector<uint32_t> &&code);


        /**
         * vertex
         */
        GraphicsPipelineBuilder &addVertexBinding(const std::function<void(VertexBindingConfigure &)> &configure);

        GraphicsPipelineBuilder &addVertexBinding(VertexBindingConfigure &&configure);

        GraphicsPipelineBuilder &sampleShadingEnable(bool sampleShadingEnable);

        GraphicsPipelineBuilder &sampleCount(vk::SampleCountFlagBits sampleCount);

        GraphicsPipelineBuilder &depthTestEnable(bool depthTestEnable);

        [[nodiscard]]
        Pipeline build();

        [[nodiscard]]
        std::unique_ptr<Pipeline> buildUnique();
    };

} // vklite
