//
// Created by leixing on 2025/5/1.
//

#include "VertexBufferBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    VertexBufferBuilder::VertexBufferBuilder() {
        mCombinedMemoryBufferBuilder.asDeviceLocal().addUsage(vk::BufferUsageFlagBits::eVertexBuffer);
    };

    VertexBufferBuilder::~VertexBufferBuilder() = default;

    VertexBufferBuilder &VertexBufferBuilder::device(vk::Device device) {
        mDevice = device;
        mCombinedMemoryBufferBuilder.device(device);
        return *this;
    }

    VertexBufferBuilder &VertexBufferBuilder::size(vk::DeviceSize size) {
        mCombinedMemoryBufferBuilder.size(size);
        return *this;
    }

    VertexBufferBuilder &VertexBufferBuilder::addUsage(vk::BufferUsageFlags usage) {
        mCombinedMemoryBufferBuilder.addUsage(usage);
        return *this;
    }

    VertexBufferBuilder &VertexBufferBuilder::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        mCombinedMemoryBufferBuilder.physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        return *this;
    }

    VertexBuffer VertexBufferBuilder::build() {
        LOG_D("VertexBufferBuilder::build()");
        if (mDevice == nullptr) {
            throw std::runtime_error("VertexBufferBuilder::build(): mDevice == nullptr");
        }
        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("VertexBufferBuilder::build(): mPhysicalDeviceMemoryProperties not set");
        }
        return VertexBuffer{mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties};
    }

    std::unique_ptr<VertexBuffer> VertexBufferBuilder::buildUnique() {
        return std::make_unique<VertexBuffer>(build());
    }

    std::vector<VertexBuffer> VertexBufferBuilder::build(uint32_t count) {
        std::vector<VertexBuffer> buffers;
        buffers.reserve(count);

        for (int i = 0; i < count; i++) {
            buffers.emplace_back(mDevice, mCombinedMemoryBufferBuilder.build(), mPhysicalDeviceMemoryProperties);
        }

        return buffers;
    }

} // vklite