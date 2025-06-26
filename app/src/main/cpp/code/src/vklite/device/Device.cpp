//
// Created by leixing on 2024/12/23.
//

#include "Device.h"

#include <map>
#include <utility>
#include <unordered_set>

#include "vklite/Log.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/util/StringUtil.h"

namespace vklite {

    Device::Device(vk::Device device)
            : mDevice(device) {}

    Device::~Device() {
        if (mDevice != nullptr) {
            mDevice.destroy();
            mDevice = nullptr;
        }
    }

    Device::Device(Device &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)) {}

    Device &Device::operator=(Device &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
        }
        return *this;
    }

    const vk::Device &Device::getVkDevice() const {
        return mDevice;
    }

    vk::Queue Device::getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex) const {
        return mDevice.getQueue(queueFamilyIndex, queueIndex);
    }

} // vklite