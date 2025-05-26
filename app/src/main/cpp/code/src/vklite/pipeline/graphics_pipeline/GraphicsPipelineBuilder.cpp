//
// Created by leixing on 2025/3/7.
//

#include "GraphicsPipelineBuilder.h"
#include "vklite/shader/ShaderModule.h"

namespace vklite {

    GraphicsPipelineBuilder::GraphicsPipelineBuilder()
            : mSampleShadingEnable(false), mSampleCount(vk::SampleCountFlagBits::e1), mDepthTestEnable(true) {};

    GraphicsPipelineBuilder::~GraphicsPipelineBuilder() = default;


    GraphicsPipelineBuilder &GraphicsPipelineBuilder::vertexShaderCode(std::vector<uint32_t> &&code) {
        mVertexShaderCode = std::move(code);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::fragmentShaderCode(std::vector<uint32_t> &&code) {
        mFragmentShaderCode = std::move(code);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::addVertexBinding(const std::function<void(VertexBindingConfigure &)> &configure) {
        VertexBindingConfigure config{};
        configure(config);
        addVertexBinding(std::move(config));
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::addVertexBinding(VertexBindingConfigure &&configure) {
        mVertexConfigure.add(std::move(configure));
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::sampleShadingEnable(bool sampleShadingEnable) {
        mSampleShadingEnable = sampleShadingEnable;
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mSampleCount = sampleCount;
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::depthTestEnable(bool depthTestEnable) {
        mDepthTestEnable = depthTestEnable;
        return *this;
    }

    GraphicsPipeline GraphicsPipelineBuilder::build(const Device &device,
                                                    const RenderPass &renderPass,
                                                    const PipelineLayout &pipelineLayout,
                                                    const std::vector<vk::Viewport> &viewports,
                                                    const std::vector<vk::Rect2D> &scissors) {
        // shader code
        ShaderModule vertexShaderModule = ShaderModule(device, mVertexShaderCode);
        ShaderModule fragmentShaderModule = ShaderModule(device, mFragmentShaderCode);

        // vertex buffer description
        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions = mVertexConfigure.createVertexInputBindingDescriptions();
        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions = mVertexConfigure.createVertexInputAttributeDescriptions();

        return {device,
                renderPass,
                vertexShaderModule,
                fragmentShaderModule,
                vertexInputBindingDescriptions,
                vertexInputAttributeDescriptions,
                pipelineLayout,
                viewports,
                scissors,
                mSampleShadingEnable,
                mSampleCount,
                mDepthTestEnable};
    }

    std::unique_ptr<GraphicsPipeline> GraphicsPipelineBuilder::buildUnique(const Device &device,
                                                                           const RenderPass &renderPass,
                                                                           const PipelineLayout &pipelineLayout,
                                                                           const std::vector<vk::Viewport> &viewports,
                                                                           const std::vector<vk::Rect2D> &scissors) {

        // shader code
        ShaderModule vertexShaderModule = ShaderModule(device, mVertexShaderCode);
        ShaderModule fragmentShaderModule = ShaderModule(device, mFragmentShaderCode);

        // vertex buffer description
        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions = mVertexConfigure.createVertexInputBindingDescriptions();
        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions = mVertexConfigure.createVertexInputAttributeDescriptions();

        return std::make_unique<GraphicsPipeline>(device,
                                                  renderPass,
                                                  vertexShaderModule,
                                                  fragmentShaderModule,
                                                  vertexInputBindingDescriptions,
                                                  vertexInputAttributeDescriptions,
                                                  pipelineLayout,
                                                  viewports,
                                                  scissors,
                                                  mSampleShadingEnable,
                                                  mSampleCount,
                                                  mDepthTestEnable);
    }

} // vklite