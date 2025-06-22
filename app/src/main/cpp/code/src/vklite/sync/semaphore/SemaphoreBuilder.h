//
// Created by leixing on 2025/5/28.
//

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "Semaphore.h"

namespace vklite {

    class SemaphoreBuilder {
    private:
        vk::Device mDevice;
        vk::SemaphoreCreateInfo mSemaphoreCreateInfo;

    public:
        SemaphoreBuilder();

        ~SemaphoreBuilder();

        SemaphoreBuilder &device(vk::Device device);

        Semaphore build();

        std::unique_ptr<Semaphore> buildUnique();

        std::vector<Semaphore> build(uint32_t count);
    };

} // vklite
