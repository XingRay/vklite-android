//
// Created by leixing on 2025/5/28.
//

#include "Semaphore.h"

namespace vklite {

    Semaphore::Semaphore(vk::Device device, vk::Semaphore semaphore)
            : mDevice(device), mSemaphore(semaphore) {}

    Semaphore::~Semaphore() {
        if (mDevice != nullptr && mSemaphore != nullptr) {
            mDevice.destroy(mSemaphore);
            mDevice = nullptr;
            mSemaphore = nullptr;
        }
    }

    Semaphore::Semaphore(Semaphore &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mSemaphore(std::exchange(other.mSemaphore, nullptr)) {}

    Semaphore &Semaphore::operator=(Semaphore &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mSemaphore = std::exchange(other.mSemaphore, nullptr);
        }
        return *this;
    }

    const vk::Semaphore &Semaphore::getVkSemaphore() const {
        return mSemaphore;
    }

} // vklite