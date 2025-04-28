//
// Created by leixing on 2025/4/28.
//

#include "SurfaceSupportPhysicalDeviceFilter.h"

namespace vklite {

    SurfaceSupportPhysicalDeviceFilter::SurfaceSupportPhysicalDeviceFilter(const Surface &surface, vk::QueueFlags queueFlags)
            : mSurface(surface), mQueueFlags(queueFlags) {}

    SurfaceSupportPhysicalDeviceFilter::~SurfaceSupportPhysicalDeviceFilter() = default;

    bool SurfaceSupportPhysicalDeviceFilter::test(const PhysicalDevice &physicalDevice) const {
        return physicalDevice.querySurfaceSupport(mSurface, mQueueFlags).has_value();
    }

} // vklite