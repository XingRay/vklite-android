//
// Created by leixing on 2025/3/7.
//

#include "ComputePipelineBuilder.h"

#include <stdexcept>
#include <format>

#include "vklite/pipeline/shader_module/ShaderModuleBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    ComputePipelineBuilder::ComputePipelineBuilder() {
        mComputeShaderStageInfo
                .setStage(vk::ShaderStageFlagBits::eCompute)
                .setPName("main");

        mComputePipelineCreateInfo
                .setStage(mComputeShaderStageInfo);

        mPipelineCache = nullptr;
    };

    ComputePipelineBuilder::~ComputePipelineBuilder() = default;

    ComputePipelineBuilder &ComputePipelineBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    ComputePipelineBuilder &ComputePipelineBuilder::computeShader(std::unique_ptr<ShaderModule> &&computeShaderModule) {
        mComputeShaderModule = std::move(computeShaderModule);
        mComputeShaderStageInfo.setModule(mComputeShaderModule->getVkShaderModule());
        mComputePipelineCreateInfo.setStage(mComputeShaderStageInfo);
        return *this;
    }

    ComputePipelineBuilder &ComputePipelineBuilder::pipelineLayout(vk::PipelineLayout pipelineLayout) {
        mComputePipelineCreateInfo.setLayout(pipelineLayout);
        return *this;
    }

    ComputePipelineBuilder &ComputePipelineBuilder::pipelineCache(vk::PipelineCache pipelineCache) {
        mPipelineCache = pipelineCache;
        return *this;
    }

    Pipeline ComputePipelineBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("ComputePipelineBuilder::build() mDevice == nullptr");
        }
        if (mComputeShaderStageInfo.module == nullptr) {
            throw std::runtime_error("ComputePipelineBuilder::build() mComputeShaderStageInfo.module== nullptr");
        }
        if (mComputePipelineCreateInfo.layout == nullptr) {
            throw std::runtime_error("ComputePipelineBuilder::build() mComputePipelineCreateInfo.layout == nullptr");
        }

        auto [result, pipeline] = mDevice.createComputePipeline(mPipelineCache, mComputePipelineCreateInfo);
        if (result != vk::Result::eSuccess) {
            LOG_E("createComputePipeline() failed: %d", make_error_code(result).value());
            throw std::runtime_error("ComputePipelineBuilder::build(): createComputePipeline() failed");
        }

        return Pipeline(mDevice, pipeline);
    }

    std::unique_ptr<Pipeline> ComputePipelineBuilder::buildUnique() {
        return std::make_unique<Pipeline>(build());
    }

} // vklite