//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBufferBuilder.h"
#include "vklite/buffer/vertex_buffer/VertexBuffer.h"

namespace vklite {

    class VertexBufferBuilder {
    private:
        vk::Device mDevice;
        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

    public:
        VertexBufferBuilder();

        ~VertexBufferBuilder();

        VertexBufferBuilder &device(vk::Device device);

        VertexBufferBuilder &size(vk::DeviceSize size);

        VertexBufferBuilder &addUsage(vk::BufferUsageFlags usage);

        VertexBufferBuilder &configDeviceMemory(vk::PhysicalDevice physicalDevice);

        VertexBufferBuilder &configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        [[nodiscard]]
        VertexBuffer build();

        [[nodiscard]]
        std::unique_ptr<VertexBuffer> buildUnique();

        [[nodiscard]]
        std::vector<VertexBuffer> build(uint32_t count);
    };

} // vklite
