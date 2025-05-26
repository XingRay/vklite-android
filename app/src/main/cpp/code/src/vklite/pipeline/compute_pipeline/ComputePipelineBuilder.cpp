//
// Created by leixing on 2025/3/7.
//

#include "ComputePipelineBuilder.h"

#include <stdexcept>
#include <format>

#include "vklite/Log.h"
#include "vklite/device/Device.h"
#include "vklite/shader/ShaderModule.h"

namespace vklite {

    ComputePipelineBuilder::ComputePipelineBuilder() = default;

    ComputePipelineBuilder::~ComputePipelineBuilder() = default;

    ComputePipelineBuilder &ComputePipelineBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    ComputePipelineBuilder &ComputePipelineBuilder::computeShaderCode(std::vector<uint32_t> &&computeShaderCode) {
        mComputeShaderCode = std::move(computeShaderCode);
        return *this;
    }

    ComputePipelineBuilder &ComputePipelineBuilder::pipelineLayout(vk::PipelineLayout pipelineLayout) {
        mPipelineLayout = pipelineLayout;
        return *this;
    }

    std::optional<ComputePipeline> ComputePipelineBuilder::build() {
        if (mDevice == nullptr || mComputeShaderCode.empty() || mPipelineLayout == nullptr) {
            LOG_EF("ComputePipelineBuilder::build(), mDevice:{}, mComputeShaderCode.size():{}, mPipelineLayout:{}",
                   static_cast<void *>(mDevice),
                   mComputeShaderCode.size(),
                   static_cast<void *>(mPipelineLayout));

            throw std::runtime_error(
                    std::format("ComputePipelineBuilder::build(), mDevice:{}, mComputeShaderCode.size():{}, mPipelineLayout:{}",
                                static_cast<void *>(mDevice),
                                mComputeShaderCode.size(),
                                static_cast<void *>(mPipelineLayout)
                    )
            );
        }

        ShaderModule computeShaderModule = ShaderModule(mDevice, mComputeShaderCode);

        vk::PipelineShaderStageCreateInfo computeShaderStageInfo{};
        computeShaderStageInfo
                .setStage(vk::ShaderStageFlagBits::eCompute)
                .setModule(computeShaderModule.getShaderModule())
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

        return ComputePipeline(mDevice, pipeline);
    }

    std::unique_ptr<ComputePipeline> ComputePipelineBuilder::buildUnique() {
        std::optional<ComputePipeline> computePipeline = build();
        if (!computePipeline.has_value()) {
            return nullptr;
        }
        return std::make_unique<ComputePipeline>(std::move(computePipeline.value()));
    }

} // vklite