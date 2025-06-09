//
// Created by leixing on 2025/5/1.
//

#include "CommandPoolBuilder.h"

#include "vklite/Log.h"

namespace vklite {

    CommandPoolBuilder::CommandPoolBuilder() {
        mCommandPoolCreateInfo
                .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                .setQueueFamilyIndex(0);
    }

    CommandPoolBuilder::~CommandPoolBuilder() = default;

    CommandPoolBuilder &CommandPoolBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    CommandPoolBuilder &CommandPoolBuilder::flags(vk::CommandPoolCreateFlags flags) {
        mCommandPoolCreateInfo.flags = flags;
        return *this;
    }

    CommandPoolBuilder &CommandPoolBuilder::queueFamilyIndex(uint32_t queueFamilyIndex) {
        mCommandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
        return *this;
    }

    CommandPool CommandPoolBuilder::build() {
        if (mDevice == nullptr) {
            LOG_E("mDevice== nullptr, must invoke CommandPoolBuilder::device(vk::Device device)");
            throw std::runtime_error("mDevice== nullptr, must invoke CommandPoolBuilder::device(vk::Device device)");
        }

        vk::CommandPool commandPool = mDevice.createCommandPool(mCommandPoolCreateInfo);
        CommandPoolMeta meta{};
        meta.queueFamilyIndex = mCommandPoolCreateInfo.queueFamilyIndex;

        return CommandPool(mDevice, commandPool, meta);
    }

    std::unique_ptr<CommandPool> CommandPoolBuilder::buildUnique() {
        return std::make_unique<CommandPool>(build());
    }

} // vklite