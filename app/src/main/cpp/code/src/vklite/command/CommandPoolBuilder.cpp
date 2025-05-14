//
// Created by leixing on 2025/5/1.
//

#include "CommandPoolBuilder.h"

namespace vklite {

    CommandPoolBuilder::CommandPoolBuilder()
            : mFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer), mQueueFamilyIndex(0) {}

    CommandPoolBuilder::~CommandPoolBuilder() = default;

    CommandPoolBuilder &CommandPoolBuilder::flags(vk::CommandPoolCreateFlags flags) {
        mFlags = flags;
        return *this;
    }

    CommandPoolBuilder &CommandPoolBuilder::queueFamilyIndex(uint32_t queueFamilyIndex) {
        mQueueFamilyIndex = queueFamilyIndex;
        return *this;
    }

    std::unique_ptr<CommandPool> CommandPoolBuilder::build(const Device &device) {
        return std::make_unique<CommandPool>(device, mFlags, mQueueFamilyIndex);
    }
} // vklite