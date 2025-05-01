//
// Created by leixing on 2025/5/1.
//

#include "CommandPoolBuilder.h"

namespace vklite {

    CommandPoolBuilder::CommandPoolBuilder()
            : mFrameCount(1) {}

    CommandPoolBuilder::~CommandPoolBuilder() = default;

    CommandPoolBuilder &CommandPoolBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    std::unique_ptr<CommandPool> CommandPoolBuilder::build(const Device &device) {
        return std::make_unique<CommandPool>(device, mFrameCount);
    }
} // vklite