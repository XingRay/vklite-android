//
// Created by leixing on 2025/4/4.
//

#include "UniformBufferBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    UniformBufferBuilder::UniformBufferBuilder() {
        mCombinedMemoryBufferBuilder.asDeviceLocal().addUsage(vk::BufferUsageFlagBits::eUniformBuffer);
    };

    UniformBufferBuilder::~UniformBufferBuilder() = default;

    UniformBufferBuilder &UniformBufferBuilder::device(vk::Device device) {
        mDevice = device;
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    UniformBufferBuilder &UniformBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    UniformBufferBuilder &UniformBufferBuilder::addUsage(vk::BufferUsageFlags usage) {
        mCombinedMemoryBufferBuilder.addUsage(usage);
        return *this;
    }

    UniformBufferBuilder &UniformBufferBuilder::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    UniformBuffer UniformBufferBuilder::build() {
        LOG_D("UniformBufferBuilder::build()");
        if (mDevice == nullptr) {
            throw std::runtime_error("UniformBufferBuilder::build(): mDevice == nullptr");
        }
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("UniformBufferBuilder::build(): mPhysicalDeviceMemoryProperties not set");
        }
        return UniformBuffer(mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties);
    }

    std::unique_ptr<UniformBuffer> UniformBufferBuilder::buildUnique() {
        return std::make_unique<UniformBuffer>(build());
    }

    std::vector<UniformBuffer> UniformBufferBuilder::build(uint32_t count) {
        std::vector<UniformBuffer> buffers;
        buffers.reserve(count);

        for (int i = 0; i < count; i++) {
            buffers.emplace_back(build());
        }

        return buffers;
    }

} // vklite