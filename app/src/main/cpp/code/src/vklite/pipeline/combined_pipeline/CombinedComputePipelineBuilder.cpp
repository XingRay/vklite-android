//
// Created by leixing on 2025-06-19.
//

#include "CombinedComputePipelineBuilder.h"

namespace vklite {

    CombinedComputePipelineBuilder::CombinedComputePipelineBuilder() : mFrameCount(0) {};

    CombinedComputePipelineBuilder::~CombinedComputePipelineBuilder() = default;

    CombinedComputePipelineBuilder &CombinedComputePipelineBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    CombinedComputePipelineBuilder &CombinedComputePipelineBuilder::device(vk::Device device) {
        mDevice = device;
        mDescriptorSetLayoutsBuilder.device(device);
        mPipelineLayoutBuilder.device(device);
        mComputeShaderModuleBuilder.device(device);
        mComputePipelineBuilder.device(device);

        return *this;
    }

    CombinedComputePipelineBuilder &CombinedComputePipelineBuilder::descriptorPool(vk::DescriptorPool descriptorPool) {
        mDescriptorPool = descriptorPool;
        return *this;
    }

    CombinedComputePipelineBuilder &CombinedComputePipelineBuilder::pushConstantRange(std::vector<vk::PushConstantRange> &&pushConstantRanges) {
        mPushConstantRanges = std::move(std::move(pushConstantRanges));
        return *this;
    }

    CombinedComputePipelineBuilder &CombinedComputePipelineBuilder::bindings(std::vector<std::vector<vk::DescriptorSetLayoutBinding>> &&descriptorSetLayoutBindings) {
        mDescriptorSetLayoutsBuilder.bindings(std::move(descriptorSetLayoutBindings));
        return *this;
    }

    CombinedComputePipelineBuilder &CombinedComputePipelineBuilder::computeShaderCode(std::vector<uint32_t> &&code) {
        mComputeShaderModuleBuilder.code(std::move(code));
        return *this;
    }


    CombinedComputePipelineBuilder &CombinedComputePipelineBuilder::shaderConfigure(ShaderConfigure &shaderConfigure) {
        pushConstantRange(std::move(shaderConfigure.getPushConstantRanges()));
        bindings(shaderConfigure.createDescriptorSetLayoutBindings());

        computeShaderCode(std::move(shaderConfigure.getComputeShaderCode()));

        return *this;
    }

    CombinedPipeline CombinedComputePipelineBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("CombinedComputePipelineBuilder::build(): mDevice == nullptr");
        }
        if (mFrameCount == 0) {
            throw std::runtime_error("CombinedComputePipelineBuilder::build(): mFrameCount == 0");
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
                throw std::runtime_error("CombinedComputePipelineBuilder::build(): mDescriptorPool == nullptr");
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

        Pipeline pipeline = mComputePipelineBuilder
                .computeShader(mComputeShaderModuleBuilder.buildUnique())
                .pipelineLayout(pipelineLayout.getVkPipelineLayout())
                .build();

        return CombinedPipeline{
                std::move(pipeline),
                std::move(descriptorSetLayouts),
                std::move(pipelineLayout),
                std::move(descriptorSets),
                std::move(pushConstants),
        };
    }

    std::unique_ptr<CombinedPipeline> CombinedComputePipelineBuilder::buildUnique() {
        return std::make_unique<CombinedPipeline>(build());
    }

} // vklite