//
// Created by leixing on 2025/1/16.
//

#include "HostVisibleBuffer.h"

#include "vklite/util/VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

    HostVisibleBuffer::HostVisibleBuffer(const Device &device, vk::DeviceSize bufferSize, vk::BufferUsageFlagBits bufferUsageFlagBits)
            : mDevice(device), mBufferSize(bufferSize) {
        std::tie(mBuffer, mDeviceMemory) = device.createBuffer(bufferSize, bufferUsageFlagBits,
                                                                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
        mMappedMemoryPointer = device.getDevice().mapMemory(mDeviceMemory, 0, bufferSize, vk::MemoryMapFlags{});
    }

    vklite::HostVisibleBuffer::~HostVisibleBuffer() {
        vk::Device device = mDevice.getDevice();

        device.unmapMemory(mDeviceMemory);
        device.destroy(mBuffer);
        device.freeMemory(mDeviceMemory);
        mMappedMemoryPointer = nullptr;
    }

    const vk::Buffer &HostVisibleBuffer::getBuffer() const {
        return mBuffer;
    }

    const vk::DeviceSize &HostVisibleBuffer::getBufferSize() const {
        return mBufferSize;
    }

    const vk::DeviceMemory &HostVisibleBuffer::getDeviceMemory() const {
        return mDeviceMemory;
    }

    void HostVisibleBuffer::updateBuffer(const void *data, uint32_t size) {
        if (mMappedMemoryPointer == nullptr) {
            LOG_E("staging buffer memory is not mapped!");
            return;
        }
        if (data == nullptr) {
            LOG_E("Input data is null!");
            return;
        }
        if (size > mBufferSize) {
            LOG_E("Data size (%u) exceeds buffer size (%d)!", size, (uint32_t) mBufferSize);
            return;
        }

        memcpy(mMappedMemoryPointer, data, size);
    }

}