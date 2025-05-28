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

    public:
        explicit SurfaceSupportPhysicalDeviceFilter(const Surface &surface);

        ~SurfaceSupportPhysicalDeviceFilter() override;

        [[nodiscard]]
        bool test(const PhysicalDevice &physicalDevice) const override;
    };

} // vklite
