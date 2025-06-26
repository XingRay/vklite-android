//
// Created by leixing on 2025/5/16.
//

#include "PooledCommandBuffer.h"
#include "vklite/command_pool/CommandPool.h"
#include "PooledCommandBuffer.h"


#include <utility>

namespace vklite {

    PooledCommandBuffer::PooledCommandBuffer(vk::CommandBuffer commandBuffer)
            : mCommandBuffer(commandBuffer) {}

    // 由 CommandPool 负责回收
    PooledCommandBuffer::~PooledCommandBuffer() {
        mCommandBuffer = nullptr;
    }

    PooledCommandBuffer::PooledCommandBuffer(PooledCommandBuffer &&other) noexcept
            : mCommandBuffer(std::exchange(other.mCommandBuffer, nullptr)) {}

    PooledCommandBuffer &PooledCommandBuffer::operator=(PooledCommandBuffer &&other) noexcept {
        if (this != &other) {
            mCommandBuffer = std::exchange(other.mCommandBuffer, nullptr);
        }
        return *this;
    }

    const vk::CommandBuffer &PooledCommandBuffer::getVkCommandBuffer() const {
        return mCommandBuffer;
    }

} // vklite