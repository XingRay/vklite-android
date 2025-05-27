//
// Created by leixing on 2025/4/27.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/instance/Instance.h"
#include "vklite/surface/Surface.h"
#include "vklite/physical_device/PhysicalDevice.h"

#include "vklite/physical_device/score_calculator/PhysicalDeviceScoreCalculator.h"
#include "vklite/Log.h"
#include "vklite/physical_device/filter/PhysicalDeviceFilter.h"

namespace vklite {

    class PhysicalDeviceSelector {

    private:
        std::unique_ptr<PhysicalDeviceScoreCalculator> mScoreCalculator;
        std::vector<std::unique_ptr<PhysicalDeviceFilter>> mPhysicalDeviceFilters;

    public:
        explicit PhysicalDeviceSelector(std::unique_ptr<PhysicalDeviceScoreCalculator> scoreCalculator);

        ~PhysicalDeviceSelector();

        PhysicalDeviceSelector &addPhysicalDeviceFilter(std::unique_ptr<PhysicalDeviceFilter> filter);

        [[nodiscard]]
        std::unique_ptr<PhysicalDevice> select(const std::vector<PhysicalDevice> &physicalDevices);

        [[nodiscard]]
        std::unique_ptr<PhysicalDevice> select(const std::vector<vk::PhysicalDevice> &physicalDevices);

        // static
    public:
        static std::unique_ptr<PhysicalDeviceSelector> makeDefault(const Surface &surface,
                                                  vk::QueueFlags queueFlags = vk::QueueFlagBits::eGraphics);
    };


} // vklite
