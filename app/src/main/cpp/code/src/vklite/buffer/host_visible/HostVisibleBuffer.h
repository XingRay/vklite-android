//
// Created by leixing on 2025/1/15.
//

#pragma once

#include <cstdint>
#include <vulkan/vulkan.hpp>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBuffer.h"

namespace vklite {

    /**
     * buffer visible on cpu
     */
    class HostVisibleBuffer {
    private:
        CombinedMemoryBuffer mCombinedMemoryBuffer;

    public:
        explicit HostVisibleBuffer(CombinedMemoryBuffer &&combinedMemoryBuffer);

        ~HostVisibleBuffer();

        HostVisibleBuffer(const HostVisibleBuffer &other) = delete;

        HostVisibleBuffer &operator=(const HostVisibleBuffer &other) = delete;

        HostVisibleBuffer(HostVisibleBuffer &&other) noexcept;

        HostVisibleBuffer &operator=(HostVisibleBuffer &&other) noexcept;

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        vk::DeviceSize getBufferSize() const;

        void updateBuffer(const void *data, uint32_t size, uint32_t offset, vk::MemoryMapFlags flags = vk::MemoryMapFlags{}) const;

        void updateBuffer(const void *data, uint32_t size) const;
    };

} // vklite
