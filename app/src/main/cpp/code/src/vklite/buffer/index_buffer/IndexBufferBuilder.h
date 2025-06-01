//
// Created by leixing on 2025/3/22.
//

#pragma once

#include <vector>
#include <memory>

#include "vklite/buffer/index_buffer/IndexBuffer.h"
#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBufferBuilder.h"

namespace vklite {

    class IndexBufferBuilder {
    private:
        vk::Device mDevice;
        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

        vk::IndexType mIndexType;

    public:
        IndexBufferBuilder();

        ~IndexBufferBuilder();

        IndexBufferBuilder &device(vk::Device device);

        IndexBufferBuilder &size(vk::DeviceSize size);

        IndexBufferBuilder &configDeviceMemory(vk::PhysicalDevice physicalDevice);

        IndexBufferBuilder &configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        IndexBufferBuilder &indexType(vk::IndexType indexType);

        [[nodiscard]]
        IndexBuffer build();

        [[nodiscard]]
        std::unique_ptr<IndexBuffer> buildUnique();
    };

} // vklite
