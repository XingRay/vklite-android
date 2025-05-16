//
// Created by leixing on 2025/3/30.
//

#pragma once

#include <cstdint>

#include "vulkan/vulkan.hpp"
#include "vklite/command_buffer/CommandPool.h"

namespace vklite {

    class BufferInterface {
    private:

    public:
        BufferInterface();

        virtual ~BufferInterface() = 0;

        [[nodiscard]]
        virtual vk::DeviceSize getSize() const = 0;

        [[nodiscard]]
        virtual const vk::Buffer &getBuffer() const = 0;

        virtual void update(const CommandPool &commandPool, const void *data, uint32_t size) = 0;
    };

} // vklite