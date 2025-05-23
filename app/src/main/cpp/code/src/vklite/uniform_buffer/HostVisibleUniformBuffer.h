//
// Created by leixing on 2025/3/13.
//

#pragma once

#include "vklite/buffer/host_visible/HostVisibleBuffer.h"

namespace vklite {

    class HostVisibleUniformBuffer {
    private:
        HostVisibleBuffer mUniformBuffer;

    public:
        HostVisibleUniformBuffer(const Device &device, vk::DeviceSize bufferSize);

        ~HostVisibleUniformBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceSize &getBufferSize() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        void updateBuffer(const void *data, uint32_t size);
    };

} // vklite
