//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vklite/command_buffer/CommandPool.h"
#include "vklite/device/Device.h"

namespace vklite {

    class CommandPoolBuilder {
    private:
        vk::CommandPoolCreateFlags mFlags;
        uint32_t mQueueFamilyIndex;

    public:
        CommandPoolBuilder();

        ~CommandPoolBuilder();

        CommandPoolBuilder &flags(vk::CommandPoolCreateFlags flags);

        CommandPoolBuilder &queueFamilyIndex(uint32_t queueFamilyIndex);

        [[nodiscard]]
        std::unique_ptr<CommandPool> build(const Device &device);
    };

} // vklite
