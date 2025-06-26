//
// Created by leixing on 2025-06-19.
//

#include "CombinedGraphicPipelineBuilder.h"

namespace vklite {

    CombinedGraphicPipelineBuilder::CombinedGraphicPipelineBuilder() : mFrameCount(0) {};

    CombinedGraphicPipelineBuilder::~CombinedGraphicPipelineBuilder() = default;

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::device(vk::Device device) {
        mDevice = device;
        mDescriptorSetLayoutsBuilder.device(device);
        mPipelineLayoutBuilder.device(device);
        mVertexShaderModuleBuilder.device(device);
        mFragmentShaderModuleBuilder.device(device);
        mGraphicsPipelineBuilder.device(device);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::descriptorPool(vk::DescriptorPool descriptorPool) {
        mDescriptorPool = descriptorPool;
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::pushConstantRange(std::vector<vk::PushConstantRange> &&pushConstantRanges) {
        mPushConstantRanges = std::move(std::move(pushConstantRanges));
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::bindings(std::vector<std::vector<vk::DescriptorSetLayoutBinding>> &&descriptorSetLayoutBindings) {
        mDescriptorSetLayoutsBuilder.bindings(std::move(descriptorSetLayoutBindings));
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::vertexShaderCode(std::vector<uint32_t> &&code) {
        mVertexShaderModuleBuilder.code(std::move(code));
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::fragmentShaderCode(std::vector<uint32_t> &&code) {
        mFragmentShaderModuleBuilder.code(std::move(code));
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::renderPass(vk::RenderPass renderPass, uint32_t subpassIndex) {
        mGraphicsPipelineBuilder
                .renderPass(renderPass)
                .subpass(subpassIndex);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::viewports(const std::vector<vk::Viewport> &viewports) {
        mGraphicsPipelineBuilder.viewports(viewports);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::viewports(std::vector<vk::Viewport> &&viewports) {
        mGraphicsPipelineBuilder.viewports(std::move(viewports));
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::scissors(const std::vector<vk::Rect2D> &scissors) {
        mGraphicsPipelineBuilder.scissors(scissors);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::scissors(std::vector<vk::Rect2D> &&scissors) {
        mGraphicsPipelineBuilder.scissors(std::move(scissors));
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::topology(vk::PrimitiveTopology topology) {
        mGraphicsPipelineBuilder.topology(topology);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::vertexBindingDescriptions(std::vector<vk::VertexInputBindingDescription> &&vertexBindingDescriptions) {
        mGraphicsPipelineBuilder.vertexBindingDescriptions(std::move(vertexBindingDescriptions));
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::vertexAttributeDescriptions(std::vector<vk::VertexInputAttributeDescription> &&vertexAttributeDescriptions) {
        mGraphicsPipelineBuilder.vertexAttributeDescriptions(std::move(vertexAttributeDescriptions));
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mGraphicsPipelineBuilder.sampleCount(sampleCount);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::polygonMode(vk::PolygonMode polygonMode) {
        mGraphicsPipelineBuilder.polygonMode(polygonMode);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::lineWidth(float lineWidth) {
        mGraphicsPipelineBuilder.lineWidth(lineWidth);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::cullMode(vk::CullModeFlags cullMode) {
        mGraphicsPipelineBuilder.cullMode(cullMode);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::frontFace(vk::FrontFace frontFace) {
        mGraphicsPipelineBuilder.frontFace(frontFace);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::depthTestEnable(bool depthTestEnable) {
        mGraphicsPipelineBuilder.depthTestEnable(depthTestEnable);
        return *this;
    }

    CombinedGraphicPipelineBuilder &CombinedGraphicPipelineBuilder::shaderConfigure(ShaderConfigure &shaderConfigure) {
        pushConstantRange(std::move(shaderConfigure.getPushConstantRanges()));
        bindings(std::move(shaderConfigure.createDescriptorSetLayoutBindings()));

        vertexShaderCode(std::move(shaderConfigure.getVertexShaderCode()));
        fragmentShaderCode(std::move(shaderConfigure.getFragmentShaderCode()));
        vertexBindingDescriptions(shaderConfigure.createVertexBindingDescriptions());
        vertexAttributeDescriptions(shaderConfigure.createVertexAttributeDescriptions());

        return *this;
    }

    CombinedPipeline CombinedGraphicPipelineBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("CombinedGraphicPipelineBuilder::build(): mDevice == nullptr");
        }
        if (mFrameCount == 0) {
            throw std::runtime_error("CombinedGraphicPipelineBuilder::build(): mFrameCount == 0");
        }

        std::vector<PushConstant> pushConstants;
        pushConstants.reserve(mPushConstantRanges.size());
        for (const vk::PushConstantRange &pushConstantRange: mPushConstantRanges) {
            pushConstants.emplace_back(pushConstantRange.size, pushConstantRange.offset, pushConstantRange.stageFlags);
        }
        DescriptorSetLayouts descriptorSetLayouts = mDescriptorSetLayoutsBuilder.build();
        std::vector<std::vector<vk::DescriptorSet>> descriptorSets;
        if (!descriptorSetLayouts.getDescriptorSetLayouts().empty()) {
            if (mDescriptorPool == nullptr) {
                throw std::runtime_error("CombinedGraphicPipelineBuilder::build(): mDescriptorPool == nullptr");
            }
            descriptorSets.reserve(mFrameCount);
            for (uint32_t i = 0; i < mFrameCount; i++) {
                vk::DescriptorSetAllocateInfo allocateInfo{};
                allocateInfo
                        .setDescriptorPool(mDescriptorPool)
                        .setSetLayouts(descriptorSetLayouts.getDescriptorSetLayouts());

                descriptorSets.push_back(mDevice.allocateDescriptorSets(allocateInfo));
            }
        }

        PipelineLayout pipelineLayout = mPipelineLayoutBuilder
                .descriptorSetLayouts(descriptorSetLayouts.getDescriptorSetLayouts())
                .pushConstantRanges(std::move(mPushConstantRanges))
                .build();

        Pipeline pipeline = mGraphicsPipelineBuilder
                .vertexShader(mVertexShaderModuleBuilder.buildUnique())
                .fragmentShader(mFragmentShaderModuleBuilder.buildUnique())
                .pipelineLayout(pipelineLayout.getVkPipelineLayout())
                .build();

        return CombinedPipeline(
                std::move(pipeline),
                std::move(descriptorSetLayouts),
                std::move(pipelineLayout),
                std::move(descriptorSets),
                std::move(pushConstants)
        );
    }

    std::unique_ptr<CombinedPipeline> CombinedGraphicPipelineBuilder::buildUnique() {
        return std::make_unique<CombinedPipeline>(build());
    }

} // vklite