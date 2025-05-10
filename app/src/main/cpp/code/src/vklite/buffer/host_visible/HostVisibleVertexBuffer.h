//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "HostVisibleBuffer.h"

namespace vklite {

    class HostVisibleVertexBuffer {
    private:
        HostVisibleBuffer mVertexBuffer;

    public:
        HostVisibleVertexBuffer(const Device &device, vk::DeviceSize bufferSize);

        ~HostVisibleVertexBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceSize &getBufferSize() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void updateBuffer(const void *data, uint32_t size);
    };

} // vklite
