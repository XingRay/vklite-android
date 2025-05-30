//
// Created by leixing on 2024/12/28.
//

#include "ComputePipeline.h"
#include "vklite/Log.h"

#include <utility>

namespace vklite {

    ComputePipeline::ComputePipeline(vk::Device device, vk::Pipeline pipeline)
            : mDevice(device), mPipeline(pipeline) {}

    ComputePipeline::~ComputePipeline() {
        if (mDevice != nullptr && mPipeline != nullptr) {
            mDevice.destroy(mPipeline);
            mDevice = nullptr;
            mPipeline = nullptr;
        }
    }

    ComputePipeline::ComputePipeline(ComputePipeline &&other) noexcept
            : mDevice(other.mDevice), mPipeline(std::exchange(other.mPipeline, nullptr)) {}

    ComputePipeline &ComputePipeline::operator=(ComputePipeline &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mPipeline = std::exchange(other.mPipeline, nullptr);
        }
        return *this;
    }

    const vk::Pipeline &ComputePipeline::getPipeline() const {
        return mPipeline;
    }

} // vklite