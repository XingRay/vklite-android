//
// Created by leixing on 2025/5/29.
//

#include "QueueSupportPhysicalDeviceFilter.h"

namespace vklite {

    QueueSupportPhysicalDeviceFilter::QueueSupportPhysicalDeviceFilter(vk::QueueFlags queueFlags)
            : mQueueFlags(queueFlags) {}

    QueueSupportPhysicalDeviceFilter::~QueueSupportPhysicalDeviceFilter() = default;

    [[nodiscard]]
    bool QueueSupportPhysicalDeviceFilter::test(const PhysicalDevice &physicalDevice) const {
        std::vector<uint32_t> indices = physicalDevice.queryQueueFamilyIndicesByFlags(mQueueFlags);
        return !indices.empty();
    }

} // vklite