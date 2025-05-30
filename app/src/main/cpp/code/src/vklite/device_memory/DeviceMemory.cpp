//
// Created by leixing on 2025/5/30.
//

#include "DeviceMemory.h"
#include <utility>

namespace vklite {

    DeviceMemory::DeviceMemory(const vk::Device &device, const vk::DeviceMemory &deviceMemory)
            : mDevice(device), mDeviceMemory(deviceMemory) {}

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
              mDeviceMemory(std::exchange(other.mDeviceMemory, nullptr)) {}

    DeviceMemory &DeviceMemory::operator=(DeviceMemory &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mDeviceMemory = std::exchange(other.mDeviceMemory, nullptr);
        }
        return *this;
    }

    [[nodiscard]]
    const vk::DeviceMemory &DeviceMemory::getDeviceMemory() const {
        return mDeviceMemory;
    }

} // vklite