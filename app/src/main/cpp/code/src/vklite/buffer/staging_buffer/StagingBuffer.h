//
// Created by leixing on 2025/3/6.
//

#pragma once

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBuffer.h"

namespace vklite {

    class StagingBuffer {
    private:
        CombinedMemoryBuffer mCombinedMemoryBuffer;

    public:
        StagingBuffer(CombinedMemoryBuffer &&combinedMemoryBuffer);

        ~StagingBuffer();

        StagingBuffer(const StagingBuffer &other) = delete;

        StagingBuffer &operator=(const StagingBuffer &other) = delete;

        StagingBuffer(StagingBuffer &&other) noexcept;

        StagingBuffer &operator=(StagingBuffer &&other) noexcept;

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        void updateBuffer(const void *data, uint32_t size, uint32_t offset, vk::MemoryMapFlags flags = vk::MemoryMapFlags{}) const;

        void updateBuffer(const void *data, uint32_t size) const;
    };

} // vklite
