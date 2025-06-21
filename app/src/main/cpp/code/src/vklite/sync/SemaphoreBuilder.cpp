//
// Created by leixing on 2025/5/28.
//

#include "SemaphoreBuilder.h"

namespace vklite {

    SemaphoreBuilder::SemaphoreBuilder()
            : mSemaphoreCreateInfo{} {}

    SemaphoreBuilder::~SemaphoreBuilder() = default;

    SemaphoreBuilder &SemaphoreBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    Semaphore SemaphoreBuilder::build() {
        return Semaphore{mDevice, mDevice.createSemaphore(mSemaphoreCreateInfo)};
    }

    std::unique_ptr<Semaphore> SemaphoreBuilder::buildUnique() {
        return std::make_unique<Semaphore>(build());
    }

    std::vector<Semaphore> SemaphoreBuilder::build(uint32_t count) {
        std::vector<Semaphore> semaphores;
        semaphores.reserve(count);
        for (uint32_t i = 0; i < count; i++) {
            semaphores.emplace_back(mDevice, mDevice.createSemaphore(mSemaphoreCreateInfo));
        }
        return semaphores;
    }

} // vklite