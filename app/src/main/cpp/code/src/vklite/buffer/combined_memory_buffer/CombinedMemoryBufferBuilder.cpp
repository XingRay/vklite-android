//
// Created by leixing on 2025/5/31.
//

#include "CombinedMemoryBufferBuilder.h"

namespace vklite {

    CombinedMemoryBufferBuilder::CombinedMemoryBufferBuilder()
            : mMemoryOffset(0), mUseConfigDeviceMemory(false) {}

    CombinedMemoryBufferBuilder::~CombinedMemoryBufferBuilder() = default;

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::device(vk::Device device) {
        mBufferBuilder.device(device);
        mDeviceMemoryBuilder.device(device);
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::size(vk::DeviceSize size) {
        mBufferBuilder.bufferSize(size);
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::sharingMode(vk::SharingMode sharingMode) {
        mBufferBuilder.sharingMode(sharingMode);
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::usage(vk::BufferUsageFlags usage) {
        mBufferBuilder.usage(usage);
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::addUsage(vk::BufferUsageFlags usage) {
        mBufferBuilder.addUsage(usage);
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::memoryOffset(vk::DeviceSize memoryOffset) {
        mMemoryOffset = memoryOffset;
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::configDeviceMemory(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties,
                                                                                 vk::MemoryPropertyFlags memoryPropertyFlags) {
        this->physicalDeviceMemoryProperties(physicalDeviceMemoryProperties);
        this->memoryPropertyFlags(memoryPropertyFlags);
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::memoryPropertyFlags(vk::MemoryPropertyFlags memoryPropertyFlags) {
        mUseConfigDeviceMemory = true;
        mMemoryPropertyFlags = memoryPropertyFlags;
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::physicalDeviceMemoryProperties(vk::PhysicalDeviceMemoryProperties physicalDeviceMemoryProperties) {
        mUseConfigDeviceMemory = true;
        mPhysicalDeviceMemoryProperties = physicalDeviceMemoryProperties;
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::bufferBuilder(std::function<void(BufferBuilder &builder)> &&configure) {
        mBufferBuilderConfigure = std::move(configure);
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::deviceMemoryBuilder(std::function<void(Buffer &buffer, DeviceMemoryBuilder &builder)> &&configure) {
        mDeviceMemoryBuilderConfigure = std::move(configure);
        return *this;
    }

    CombinedMemoryBuffer CombinedMemoryBufferBuilder::build(){
        mBufferBuilderConfigure(mBufferBuilder);
        Buffer buffer = mBufferBuilder.build();

        if (mUseConfigDeviceMemory) {
            if (!mPhysicalDeviceMemoryProperties.has_value()) {
                throw std::runtime_error("mPhysicalDeviceMemoryProperties not set, must invoke physicalDeviceMemoryProperties()");
            }
            if (!mMemoryPropertyFlags.has_value()) {
                throw std::runtime_error("mMemoryPropertyFlags not set, must invoke memoryPropertyFlags()");
            }
            mDeviceMemoryBuilder.config(mPhysicalDeviceMemoryProperties.value(), buffer.getBuffer(), mMemoryPropertyFlags.value());
        }

        mDeviceMemoryBuilderConfigure(buffer, mDeviceMemoryBuilder);
        DeviceMemory deviceMemory = mDeviceMemoryBuilder.build();

        buffer.bindMemory(deviceMemory.getDeviceMemory(), mMemoryOffset);

//        return CombinedMemoryBuffer(std::move(buffer), std::move(deviceMemory));
        return {std::move(buffer), std::move(deviceMemory)};
    }

    std::unique_ptr<CombinedMemoryBuffer> CombinedMemoryBufferBuilder::buildUnique() {
        return std::make_unique<CombinedMemoryBuffer>(build());
    }


    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::asDefault() {
        (*this)
                .memoryOffset(0);

        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::asDeviceLocal() {
        (*this).asDefault()
                .usage(vk::BufferUsageFlagBits::eTransferDst)
                .memoryPropertyFlags(vk::MemoryPropertyFlagBits::eDeviceLocal);
        return *this;
    }

    CombinedMemoryBufferBuilder &CombinedMemoryBufferBuilder::asHostVisible() {
        (*this).asDefault()
                .memoryPropertyFlags(vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

        return *this;
    }

} // vklite