//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>

#include "vklite/swapchain/Swapchain.h"
#include "vklite/device/Device.h"
#include "vklite/surface/Surface.h"

namespace vklite {

    class SwapchainBuilder {
    private:

    public:
        SwapchainBuilder();

        ~SwapchainBuilder();

        std::unique_ptr<Swapchain> build(const PhysicalDevice &physicalDevice, const Device &device, const Surface &surface, std::vector<uint32_t> queueFamilyIndices);
    };

} // vklite
