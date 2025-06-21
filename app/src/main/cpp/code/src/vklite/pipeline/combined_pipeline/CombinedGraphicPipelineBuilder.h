//
// Created by leixing on 2025-06-19.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vklite/pipeline/shader/ShaderConfigure.h"
#include "vklite/pipeline/combined_pipeline/CombinedPipeline.h"
#include "vklite/push_constant/PushConstant.h"
#include "vklite/pipeline/descriptor_set_layout/DescriptorSetLayoutsBuilder.h"
#include "vklite/pipeline/pipeline_layout/PipelineLayoutBuilder.h"
#include "vklite/pipeline/shader_module/ShaderModuleBuilder.h"
#include "vklite/pipeline/pipeline/GraphicsPipelineBuilder.h"

namespace vklite {

    class CombinedGraphicPipelineBuilder {
    private:
        uint32_t mFrameCount;
        vk::Device mDevice;
        vk::DescriptorPool mDescriptorPool;

        std::vector<vk::PushConstantRange> mPushConstantRanges;
        DescriptorSetLayoutsBuilder mDescriptorSetLayoutsBuilder;
        PipelineLayoutBuilder mPipelineLayoutBuilder;

        ShaderModuleBuilder mVertexShaderModuleBuilder;
        ShaderModuleBuilder mFragmentShaderModuleBuilder;

        GraphicsPipelineBuilder mGraphicsPipelineBuilder;

    public:
        CombinedGraphicPipelineBuilder();

        ~CombinedGraphicPipelineBuilder();

        CombinedGraphicPipelineBuilder &frameCount(uint32_t frameCount);

        CombinedGraphicPipelineBuilder &device(vk::Device device);

        CombinedGraphicPipelineBuilder &descriptorPool(vk::DescriptorPool descriptorPool);


        CombinedGraphicPipelineBuilder &pushConstantRange(std::vector<vk::PushConstantRange> &&pushConstantRanges);

        CombinedGraphicPipelineBuilder &bindings(std::vector<std::vector<vk::DescriptorSetLayoutBinding>> &&descriptorSetLayoutBindings);

        CombinedGraphicPipelineBuilder &vertexShaderCode(std::vector<uint32_t> &&code);

        CombinedGraphicPipelineBuilder &fragmentShaderCode(std::vector<uint32_t> &&code);

        CombinedGraphicPipelineBuilder &renderPass(vk::RenderPass renderPass, uint32_t subpassIndex = 0);

        CombinedGraphicPipelineBuilder &viewports(const std::vector<vk::Viewport> &viewports);

        CombinedGraphicPipelineBuilder &viewports(std::vector<vk::Viewport> &&viewports);

        CombinedGraphicPipelineBuilder &scissors(const std::vector<vk::Rect2D> &scissors);

        CombinedGraphicPipelineBuilder &scissors(std::vector<vk::Rect2D> &&scissors);

        CombinedGraphicPipelineBuilder &topology(vk::PrimitiveTopology topology);

        CombinedGraphicPipelineBuilder &vertexBindingDescriptions(std::vector<vk::VertexInputBindingDescription> &&vertexBindingDescriptions);

        CombinedGraphicPipelineBuilder &vertexAttributeDescriptions(std::vector<vk::VertexInputAttributeDescription> &&vertexAttributeDescriptions);

        CombinedGraphicPipelineBuilder &sampleCount(vk::SampleCountFlagBits sampleCount);

        CombinedGraphicPipelineBuilder &polygonMode(vk::PolygonMode polygonMode);

        CombinedGraphicPipelineBuilder &lineWidth(float lineWidth);

        CombinedGraphicPipelineBuilder &cullMode(vk::CullModeFlags cullMode);

        CombinedGraphicPipelineBuilder &frontFace(vk::FrontFace frontFace);

        CombinedGraphicPipelineBuilder &depthTestEnable(bool depthTestEnable);


        CombinedGraphicPipelineBuilder &shaderConfigure(ShaderConfigure &shaderConfigure);


        CombinedPipeline build();

        std::unique_ptr<CombinedPipeline> buildUnique();
    };

} // vklite
