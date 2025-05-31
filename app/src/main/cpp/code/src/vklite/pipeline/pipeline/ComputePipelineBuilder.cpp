//
// Created by leixing on 2025/3/7.
//

#include "ComputePipelineBuilder.h"

#include <stdexcept>
#include <format>

#include "vklite/pipeline/shader_module/ShaderModuleBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    ComputePipelineBuilder::ComputePipelineBuilder() = default;

    ComputePipelineBuilder::~ComputePipelineBuilder() = default;

    ComputePipelineBuilder &ComputePipelineBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    ComputePipelineBuilder &ComputePipelineBuilder::computeShader(std::unique_ptr<ShaderModule> computeShader) {
        mComputeShaderModule = std::move(computeShader);
        return *this;
    }

    ComputePipelineBuilder &ComputePipelineBuilder::computeShader(std::vector<uint32_t> &&computeShaderCode) {
        mComputeShaderModule = ShaderModuleBuilder().device(mDevice).code(std::move(computeShaderCode)).buildUnique();
        return *this;
    }

    ComputePipelineBuilder &ComputePipelineBuilder::pipelineLayout(vk::PipelineLayout pipelineLayout) {
        mPipelineLayout = pipelineLayout;
        return *this;
    }

    std::optional<Pipeline> ComputePipelineBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("ComputePipelineBuilder::build() mDevice == nullptr");
        }
        if (mComputeShaderModule == nullptr) {
            throw std::runtime_error("ComputePipelineBuilder::build() mComputeShaderModule == nullptr");
        }
        if (mPipelineLayout == nullptr) {
            throw std::runtime_error("ComputePipelineBuilder::build() mPipelineLayout == nullptr");
        }

        vk::PipelineShaderStageCreateInfo computeShaderStageInfo{};
        computeShaderStageInfo
                .setStage(vk::ShaderStageFlagBits::eCompute)
                .setModule(mComputeShaderModule->getShaderModule())
                .setPName("main");

        vk::ComputePipelineCreateInfo computePipelineCreateInfo{};
        computePipelineCreateInfo
                .setLayout(mPipelineLayout)
                .setStage(computeShaderStageInfo);

        auto [result, pipeline] = mDevice.createComputePipeline(nullptr, computePipelineCreateInfo);
        if (result != vk::Result::eSuccess) {
            LOG_E("createComputePipeline() failed");
            return std::nullopt;
        }

        return Pipeline(mDevice, pipeline);
    }

    std::unique_ptr<Pipeline> ComputePipelineBuilder::buildUnique() {
        std::optional<Pipeline> computePipeline = build();
        if (!computePipeline.has_value()) {
            return nullptr;
        }
        return std::make_unique<Pipeline>(std::move(computePipeline.value()));
    }

} // vklite