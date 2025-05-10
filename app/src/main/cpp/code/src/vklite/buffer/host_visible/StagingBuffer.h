//
// Created by leixing on 2025/3/6.
//

#pragma once

#include "HostVisibleBuffer.h"

namespace vklite {

    class StagingBuffer {
    private:
        HostVisibleBuffer mHostVisibleBuffer;

    public:
        StagingBuffer(const Device &device, vk::DeviceSize bufferSize);

        ~StagingBuffer();

        [[nodiscard]]
        const vk::Buffer &getBuffer() const;

        void updateBuffer(const void *data, uint32_t size);
    };

} // vklite
