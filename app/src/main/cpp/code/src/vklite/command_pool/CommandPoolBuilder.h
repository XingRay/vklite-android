//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vklite/command_pool/CommandPool.h"
#include "vklite/device/Device.h"

namespace vklite {

    class CommandPoolBuilder {
    private:
        uint32_t mFrameCount;

    public:
        CommandPoolBuilder();

        ~CommandPoolBuilder();

        CommandPoolBuilder &frameCount(uint32_t frameCount);

        std::unique_ptr<CommandPool> build(const Device& device);
    };

} // vklite
