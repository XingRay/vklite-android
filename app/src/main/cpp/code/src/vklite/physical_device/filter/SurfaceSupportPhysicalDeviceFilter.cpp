//
// Created by leixing on 2025/4/28.
//

#include "SurfaceSupportPhysicalDeviceFilter.h"

namespace vklite {

    SurfaceSupportPhysicalDeviceFilter::SurfaceSupportPhysicalDeviceFilter(const Surface &surface)
            : mSurface(surface) {}

    SurfaceSupportPhysicalDeviceFilter::~SurfaceSupportPhysicalDeviceFilter() = default;

    bool SurfaceSupportPhysicalDeviceFilter::test(const PhysicalDevice &physicalDevice) const {
        std::vector<uint32_t> indices = physicalDevice.queryQueueFamilyIndicesBySurface(mSurface.getVkSurface());
        return !indices.empty();
    }

} // vklite