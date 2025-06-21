//
// Created by leixing on 2025/4/28.
//

#pragma once

#include "vklite/physical_device/PhysicalDevice.h"

namespace vklite {

    class PhysicalDeviceFilter {
    private:
    public:
        explicit PhysicalDeviceFilter();

        virtual ~PhysicalDeviceFilter();

        /**
         * return true if PhysicalDevice is suitable
         */
        [[nodiscard]]
        virtual bool test(const PhysicalDevice &physicalDevice) const = 0;
    };

} // vklite
