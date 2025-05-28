//
// Created by leixing on 2025/5/29.
//

#pragma once

#include "PhysicalDeviceFilter.h"

namespace vklite {

    class QueueSupportPhysicalDeviceFilter : public PhysicalDeviceFilter {
    private:
        vk::QueueFlags mQueueFlags;

    public:
        explicit QueueSupportPhysicalDeviceFilter(vk::QueueFlags queueFlags = vk::QueueFlagBits::eGraphics);

        ~QueueSupportPhysicalDeviceFilter() override;

        [[nodiscard]]
        bool test(const PhysicalDevice &physicalDevice) const override;
    };

} // vklite
