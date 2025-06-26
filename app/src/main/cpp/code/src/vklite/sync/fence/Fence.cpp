//
// Created by leixing on 2025/5/28.
//

#include "Fence.h"

#include <utility>
#include <limits>

namespace vklite {

    Fence::Fence(vk::Device device, vk::Fence fence)
            : mDevice(device), mFence(fence) {}

    Fence::~Fence() {
        if (mDevice != nullptr && mFence != nullptr) {
            mDevice.destroy(mFence);
            mDevice = nullptr;
            mFence = nullptr;
        }
    }

    Fence::Fence(Fence &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mFence(std::exchange(other.mFence, nullptr)) {}

    Fence &Fence::operator=(Fence &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mFence = std::exchange(other.mFence, nullptr);
        }
        return *this;
    }

    const vk::Fence &Fence::getVkFence() const {
        return mFence;
    }

    vk::Result Fence::reset() {
        return mDevice.resetFences(1, &mFence);
    }

    vk::Result Fence::wait(uint64_t timeout) {
        return mDevice.waitForFences(1, &mFence, vk::True, timeout);
    }

    vk::Result Fence::wait() {
        return wait(std::numeric_limits<uint64_t>::max());
    }

} // vklite