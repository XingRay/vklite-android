//
// Created by leixing on 2025/4/18.
//

#include "PipelineLayout.h"

#include <vulkan/vulkan.hpp>

#include "vklite/Log.h"

namespace vklite {

    PipelineLayout::PipelineLayout(vk::Device device, vk::PipelineLayout pipelineLayout)
            : mDevice(device), mPipelineLayout(pipelineLayout) {}

    PipelineLayout::~PipelineLayout() {
        if (mDevice != nullptr && mPipelineLayout != nullptr) {
            LOG_D("mDevice.destroy(mPipelineLayout), mPipelineLayout:%p", (void *) mPipelineLayout);
            mDevice.destroy(mPipelineLayout);
            mDevice = nullptr;
            mPipelineLayout = nullptr;
        }
    }

    PipelineLayout::PipelineLayout(PipelineLayout &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mPipelineLayout(std::exchange(other.mPipelineLayout, nullptr)) {}

    PipelineLayout &PipelineLayout::operator=(PipelineLayout &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mPipelineLayout = std::exchange(other.mPipelineLayout, nullptr);
        }
        return *this;
    }

    const vk::PipelineLayout &PipelineLayout::getVkPipelineLayout() const {
        return mPipelineLayout;
    }
} // vklite