//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <cstdint>
#include <memory>

#include "CommandPool.h"
#include "vklite/command_pool/CommandPoolMeta.h"
#include "vklite/device/Device.h"

namespace vklite {

    class CommandPoolBuilder {
    private:
        vk::Device mDevice;
        vk::CommandPoolCreateInfo mCommandPoolCreateInfo;

    public:
        CommandPoolBuilder();

        ~CommandPoolBuilder();

        CommandPoolBuilder &device(vk::Device device);

        CommandPoolBuilder &flags(vk::CommandPoolCreateFlags flags);

        CommandPoolBuilder &queueFamilyIndex(uint32_t queueFamilyIndex);

        [[nodiscard]]
        CommandPool build();

        [[nodiscard]]
        std::unique_ptr<CommandPool> buildUnique();
    };

} // vklite
