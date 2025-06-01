//
// Created by leixing on 2025/5/30.
//

#include "DeviceMemory.h"

#include <utility>
#include <format>

namespace vklite {

    DeviceMemory::DeviceMemory(const vk::Device &device, const vk::DeviceMemory &deviceMemory, DeviceMemoryMeta &&meta)
            : mDevice(device), mDeviceMemory(deviceMemory), mMeta(std::move(meta)) {
        mMappedMemoryPointer = mDevice.mapMemory(mDeviceMemory, 0, mMeta.getAllocationSize(), vk::MemoryMapFlags{});
    }

    DeviceMemory::~DeviceMemory() {
        if (mDevice != nullptr && mDeviceMemory != nullptr) {
            mDevice.unmapMemory(mDeviceMemory);
            mDevice.free(mDeviceMemory);
            mDevice = nullptr;
            mDeviceMemory = nullptr;
        }
    }

    DeviceMemory::DeviceMemory(DeviceMemory &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mDeviceMemory(std::exchange(other.mDeviceMemory, nullptr)),
              mMappedMemoryPointer(std::exchange(other.mMappedMemoryPointer, nullptr)),
              mMeta(std::move(other.mMeta)) {}

    DeviceMemory &DeviceMemory::operator=(DeviceMemory &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mDeviceMemory = std::exchange(other.mDeviceMemory, nullptr);
            mMappedMemoryPointer = std::exchange(other.mMappedMemoryPointer, nullptr);
            mMeta = std::move(other.mMeta);
        }
        return *this;
    }

    [[nodiscard]]
    const vk::DeviceMemory &DeviceMemory::getDeviceMemory() const {
        return mDeviceMemory;
    }

    void DeviceMemory::updateBuffer(const void *data, uint32_t size, uint32_t offset, vk::MemoryMapFlags flags)const{
//        if (data == nullptr) {
//            throw std::runtime_error("DeviceMemory::updateBuffer: data == nullptr");
//            return;
//        }
//        if (size == 0) {
//            throw std::runtime_error("DeviceMemory::updateBuffer: size == 0");
//            return;
//        }
//        if (size + offset > mMeta.getAllocationSize()) {
//            throw std::runtime_error(std::format("DeviceMemory::updateBuffer: Data size ({}) exceeds buffer size ({})!", size, (uint32_t) mMeta.getAllocationSize()));
//        }
//
//        mMappedMemoryPointer = mDevice.mapMemory(mDeviceMemory, offset, size, flags);

//        void *mappedMemoryPointer = mDevice.mapMemory(mDeviceMemory, offset, size, flags);
//
//        if (mappedMemoryPointer == nullptr) {
//            throw std::runtime_error("DeviceMemory::updateBuffer: mDevice.mapMemory() => null, buffer memory is not mapped!");
//            return;
//        }
//
//        memcpy(mappedMemoryPointer, data, size);
        memcpy(mMappedMemoryPointer, data, size);

//        mDevice.unmapMemory(mDeviceMemory);
    }

    void DeviceMemory::updateBuffer(const void *data, uint32_t size) const{
        updateBuffer(data, size, 0);
    }

} // vklite