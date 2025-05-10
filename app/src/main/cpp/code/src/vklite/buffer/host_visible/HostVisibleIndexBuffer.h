//
// Created by leixing on 2025/3/13.
//

#pragma once

#include <vector>
#include "HostVisibleBuffer.h"

namespace vklite {

    class HostVisibleIndexBuffer {
    private:
        HostVisibleBuffer mIndexBuffer;
        uint32_t mIndicesCount;

    public:
        HostVisibleIndexBuffer(const Device &device, vk::DeviceSize bufferSize);

        ~HostVisibleIndexBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        [[nodiscard]]
        const vk::DeviceSize &getBufferSize() const;

        [[nodiscard]]
        const vk::DeviceMemory &getDeviceMemory() const;

        [[nodiscard]]
        uint32_t getIndicesCount() const;

        void update(const std::vector<uint32_t>& indices);
    };

} // vklite
