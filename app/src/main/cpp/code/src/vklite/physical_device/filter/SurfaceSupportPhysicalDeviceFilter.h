//
// Created by leixing on 2025/4/28.
//

#pragma once

#include "PhysicalDeviceFilter.h"
#include "vklite/surface/Surface.h"

namespace vklite {

    class SurfaceSupportPhysicalDeviceFilter : public PhysicalDeviceFilter {
    private:
        const Surface &mSurface;
        vk::QueueFlags mQueueFlags;

    public:
        explicit SurfaceSupportPhysicalDeviceFilter(const Surface &surface, vk::QueueFlags queueFlags = vk::QueueFlagBits::eGraphics);

        ~SurfaceSupportPhysicalDeviceFilter() override;

        [[nodiscard]]
        bool test(const PhysicalDevice &physicalDevice) const override;
    };

} // vklite
