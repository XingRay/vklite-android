//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBufferBuilder.h"
#include "vklite/buffer/uniform_buffer/UniformBuffer.h"

namespace vklite {

    class UniformBufferBuilder {
    private:
        vk::Device mDevice;
        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;
        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;

    public:
        UniformBufferBuilder();

        ~UniformBufferBuilder();

        UniformBufferBuilder &device(vk::Device device);

        UniformBufferBuilder &size(vk::DeviceSize size);

        UniformBufferBuilder &addUsage(vk::BufferUsageFlags usage);

        UniformBufferBuilder &configDeviceMemory(vk::PhysicalDevice physicalDevice);

        UniformBufferBuilder &configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties);

        [[nodiscard]]
        UniformBuffer build();

        [[nodiscard]]
        std::unique_ptr<UniformBuffer> buildUnique();

        [[nodiscard]]
        std::vector<UniformBuffer> build(uint32_t count);
    };

} // vklite
