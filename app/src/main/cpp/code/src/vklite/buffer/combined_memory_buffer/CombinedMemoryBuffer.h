//
// Created by leixing on 2025/5/31.
//

#pragma once

#include "vklite/buffer/Buffer.h"
#include "vklite/device_memory/DeviceMemory.h"

namespace vklite {

    class CombinedMemoryBuffer {
    private:
        Buffer mBuffer;
        DeviceMemory mDeviceMemory;

    public:
        CombinedMemoryBuffer(Buffer&& buffer, DeviceMemory&& deviceMemory);

        ~CombinedMemoryBuffer();

        CombinedMemoryBuffer(const CombinedMemoryBuffer &other) = delete;

        CombinedMemoryBuffer &operator=(const CombinedMemoryBuffer &other) = delete;

        CombinedMemoryBuffer(CombinedMemoryBuffer &&other) noexcept;

        CombinedMemoryBuffer &operator=(CombinedMemoryBuffer &&other) noexcept;

        const Buffer &getBuffer() const;

        const DeviceMemory &getDeviceMemory() const;
    };

} // vklite
