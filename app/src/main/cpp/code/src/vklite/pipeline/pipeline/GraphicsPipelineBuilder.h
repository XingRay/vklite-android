//
// Created by leixing on 2025/3/7.
//

#pragma once

#include <vector>
#include <cstdint>

#include "Pipeline.h"
#include "vklite/pipeline/vertex/VertexConfigure.h"
#include "vklite/pipeline/vertex/VertexBindingConfigure.h"
#include "vklite/pipeline/shader_module/ShaderModule.h"
#include "vklite/pipeline/shader/ShaderConfigure.h"

namespace vklite {

    class GraphicsPipelineBuilder {
    private:
        vk::Device mDevice;

        /*
         * InputAssembly
         */
        vk::PipelineInputAssemblyStateCreateInfo mInputAssemblyStateCreateInfo;


        std::vector<vk::PipelineShaderStageCreateInfo> mShaderStageCreateInfos;
        std::unordered_map<vk::ShaderStageFlagBits, std::reference_wrapper<vk::PipelineShaderStageCreateInfo>> mShaderStageCreateInfoMap;

        /**
         * VertexShader
         */
        std::unique_ptr<ShaderModule> mVertexShaderModule;
        vk::PipelineVertexInputStateCreateInfo mVertexInputStateCreateInfo;
        std::vector<vk::VertexInputBindingDescription> mVertexBindingDescriptions;
        std::vector<vk::VertexInputAttributeDescription> mVertexAttributeDescriptions;

        /*
         * FragmentShader
         */
        std::unique_ptr<ShaderModule> mFragmentShaderModule;

        // Multisampling
        vk::PipelineMultisampleStateCreateInfo mMultiSampleStateCreateInfo;

        // PipelineDynamicState
        std::vector<vk::DynamicState> mDynamicStages;
        vk::PipelineDynamicStateCreateInfo mDynamicStateCreateInfo;

        // PipelineViewportState
        std::vector<vk::Viewport> mViewports;
        std::vector<vk::Rect2D> mScissors;
        vk::PipelineViewportStateCreateInfo mViewportStateCreateInfo;

        // RasterizationState
        vk::PipelineRasterizationStateCreateInfo mRasterizationStateCreateInfo;

        // depth & stencil
        vk::PipelineDepthStencilStateCreateInfo mDepthStencilStateCreateInfo;

        // colorBlend
        std::vector<vk::PipelineColorBlendAttachmentState> mColorBlendAttachmentStates;
        vk::PipelineColorBlendStateCreateInfo mColorBlendStateCreateInfo;
        std::array<float, 4> mBlendConstants;

        vk::GraphicsPipelineCreateInfo mGraphicsPipelineCreateInfo;

        vk::PipelineCache mPipelineCache;

    public:
        explicit GraphicsPipelineBuilder();

        ~GraphicsPipelineBuilder();

        GraphicsPipelineBuilder &device(const vk::Device &device);

        GraphicsPipelineBuilder &renderPass(const vk::RenderPass &renderPass);

        GraphicsPipelineBuilder &subpass(uint32_t subpass);

        GraphicsPipelineBuilder &pipelineLayout(const vk::PipelineLayout &pipelineLayout);

        GraphicsPipelineBuilder &viewports(const std::vector<vk::Viewport> &viewports);

        GraphicsPipelineBuilder &viewports(std::vector<vk::Viewport> &&viewports);

        GraphicsPipelineBuilder &scissors(const std::vector<vk::Rect2D> &scissors);

        GraphicsPipelineBuilder &scissors(std::vector<vk::Rect2D> &&scissors);

        /**
         *
         * InputAssembly
         *
         */
        GraphicsPipelineBuilder &topology(vk::PrimitiveTopology topology);

        /**
         *
         * VertexShader
         *
         */
        GraphicsPipelineBuilder &vertexShader(std::unique_ptr<ShaderModule> vertexShaderModule);

        GraphicsPipelineBuilder &vertexBindingDescriptions(std::vector<vk::VertexInputBindingDescription> &&vertexBindingDescriptions);

        GraphicsPipelineBuilder &vertexAttributeDescriptions(std::vector<vk::VertexInputAttributeDescription> &&vertexAttributeDescriptions);


        /**
         *
         * FragmentShader
         *
         */
        GraphicsPipelineBuilder &fragmentShader(std::unique_ptr<ShaderModule> fragmentShaderModule);


        /**
         * pipeline
         */
        GraphicsPipelineBuilder &sampleShadingEnable(bool sampleShadingEnable);

        GraphicsPipelineBuilder &sampleCount(vk::SampleCountFlagBits sampleCount);


        // rasterization
        GraphicsPipelineBuilder &polygonMode(vk::PolygonMode polygonMode);

        // 使用填充以外的任何模式都需要设置 lineWidth :
        GraphicsPipelineBuilder &lineWidth(float lineWidth);

        // 设置面剔除策略, 这里设置为反面被剔除
        GraphicsPipelineBuilder &cullMode(vk::CullModeFlags cullMode);

        // 设置正面的方向
        GraphicsPipelineBuilder &frontFace(vk::FrontFace frontFace);


        // depth & stencil
        GraphicsPipelineBuilder &depthTestEnable(bool depthTestEnable);

        GraphicsPipelineBuilder &basePipelineHandle(vk::Pipeline basePipelineHandle);

        GraphicsPipelineBuilder &basePipelineIndex(int32_t basePipelineIndex);

        GraphicsPipelineBuilder &pipelineCache(vk::PipelineCache pipelineCache);

        GraphicsPipelineBuilder &configShader(vklite::ShaderConfigure &graphicShaderConfigure);

        [[nodiscard]]
        Pipeline build();

        [[nodiscard]]
        std::unique_ptr<Pipeline> buildUnique();
    };

} // vklite
