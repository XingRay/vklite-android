//
// Created by leixing on 2025/3/7.
//

#include "GraphicsPipelineBuilder.h"
#include "vklite/shader/VulkanShaderModule.h"
#include "vklite/pipeline/descriptor/old/DescriptorBindingSets.h"

namespace vklite {

    GraphicsPipelineBuilder::GraphicsPipelineBuilder() = default;

    GraphicsPipelineBuilder::~GraphicsPipelineBuilder() = default;


    GraphicsPipelineBuilder &GraphicsPipelineBuilder::vertexShaderCode(std::vector<char> &&code) {
        mVertexShaderCode = std::move(code);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::fragmentShaderCode(std::vector<char> &&code) {
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

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::addDescriptorSet(const std::function<void(DescriptorSetConfigure &)> &configure) {
        DescriptorSetConfigure config = DescriptorSetConfigure();
        configure(config);
        addDescriptorSet(std::move(config));
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::addDescriptorSet(DescriptorSetConfigure &&descriptorSetConfigure) {
        mPipelineLayoutBuilder.addDescriptorSetConfigure(std::move(descriptorSetConfigure));
        return *this;
    }


    GraphicsPipelineBuilder &GraphicsPipelineBuilder::addPushConstant(uint32_t size, uint32_t offset, vk::ShaderStageFlagBits stageFlagBits) {
//        mPushConstantConfigures.addPushConstantConfigures(PushConstantConfigure(size, offset, stageFlagBits));
        return *this;
    }

    GraphicsPipeline GraphicsPipelineBuilder::build(const Device &device,
                                                    const RenderPass &renderPass,
                                                    const PipelineLayout &pipelineLayout,
                                                    const std::vector<vk::Viewport> &viewports,
                                                    const std::vector<vk::Rect2D> &scissors) {
        // shader code
        VulkanShaderModule vertexShaderModule = VulkanShaderModule(device, mVertexShaderCode);
        VulkanShaderModule fragmentShaderModule = VulkanShaderModule(device, mFragmentShaderCode);

        // vertex buffer description
        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions = mVertexConfigure.createVertexInputBindingDescriptions();
        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions = mVertexConfigure.createVertexInputAttributeDescriptions();

        return GraphicsPipeline(device,
                                renderPass,
                                vertexShaderModule,
                                fragmentShaderModule,
                                vertexInputBindingDescriptions,
                                vertexInputAttributeDescriptions,
                                pipelineLayout,
                                viewports,
                                scissors);
    }

    std::unique_ptr<GraphicsPipeline> GraphicsPipelineBuilder::buildUnique(const Device &device,
                                                                           const RenderPass &renderPass,
                                                                           const PipelineLayout &pipelineLayout,
                                                                           const std::vector<vk::Viewport> &viewports,
                                                                           const std::vector<vk::Rect2D> &scissors) {

        // shader code
        VulkanShaderModule vertexShaderModule = VulkanShaderModule(device, mVertexShaderCode);
        VulkanShaderModule fragmentShaderModule = VulkanShaderModule(device, mFragmentShaderCode);

        // vertex buffer description
        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions = mVertexConfigure.createVertexInputBindingDescriptions();
        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions = mVertexConfigure.createVertexInputAttributeDescriptions();

        // descriptor -> uniform / sampler ...
//        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = mDescriptorSetConfigures.createDescriptorSetLayouts(device);

        //        std::unique_ptr<VulkanDescriptorPool> vulkanDescriptorPool = std::make_unique<VulkanDescriptorPool>(device,
//                                                                                                            mDescriptorSetConfigures.createDescriptorPoolSizes(frameCount),
//                                                                                                            mDescriptorSetConfigures.getSetCount(frameCount));

        // frame -> set -> binding
//        std::vector<std::unique_ptr<VulkanDescriptorBindingSets>> vulkanDescriptorBindingSets;
//        for (uint32_t frameIndex = 0; frameIndex < frameCount; frameIndex++) {
//            std::unique_ptr<VulkanDescriptorBindingSets> bindingSetsOfFrame = mDescriptorSetConfigures.createVulkanDescriptorBindingSets(device, commandPool);
//            vulkanDescriptorBindingSets.push_back(std::move(bindingSetsOfFrame));
//        }

//        // push constant
//        std::vector<vk::PushConstantRange> pushConstantRanges = mPushConstantConfigures.createPushConstantRanges();
//        // 检查总大小是否超出设备限制
//        uint32_t maxPushConstantsSize = device.getMaxPushConstantsSize();
//        uint32_t totalPushConstantSize = pipelineLayout->calcTotalPushConstantSize();
//        if (totalPushConstantSize > maxPushConstantsSize) {
//            throw std::runtime_error(
//                    "Total Push Constant size (" + std::to_string(totalPushConstantSize) +
//                    ") exceeds device limit: " + std::to_string(maxPushConstantsSize)
//            );
//        }

        return std::make_unique<GraphicsPipeline>(device,
                                                  renderPass,
                                                  vertexShaderModule,
                                                  fragmentShaderModule,
                                                  vertexInputBindingDescriptions,
                                                  vertexInputAttributeDescriptions,
                                                  pipelineLayout,
                                                  viewports,
                                                  scissors);
    }

} // vklite