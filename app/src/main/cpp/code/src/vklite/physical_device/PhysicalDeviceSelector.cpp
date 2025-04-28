//
// Created by leixing on 2025/4/27.
//

#include "PhysicalDeviceSelector.h"

namespace vklite {

    PhysicalDeviceSelector::PhysicalDeviceSelector(std::unique_ptr<PhysicalDeviceScoreCalculator> scoreCalculator)
            : mScoreCalculator(std::move(scoreCalculator)) {}

    PhysicalDeviceSelector::~PhysicalDeviceSelector() = default;

    PhysicalDeviceSelector &PhysicalDeviceSelector::addPhysicalDeviceFilter(std::unique_ptr<PhysicalDeviceFilter> filter) {
        mPhysicalDeviceFilters.push_back(std::move(filter));
        return *this;
    }

    std::unique_ptr<PhysicalDevice> PhysicalDeviceSelector::select(const std::vector<PhysicalDevice> &physicalDevices) {
        if (physicalDevices.empty()) {
            LOG_D("No physical devices found!");
            return nullptr;
        }

        uint32_t maxScore = 0;
        const PhysicalDevice *bestPhysicalDevice;

        for (const PhysicalDevice &physicalDevice: physicalDevices) {
            bool filterPassed = true;
            for (const std::unique_ptr<PhysicalDeviceFilter> &filter: mPhysicalDeviceFilters) {
                if (!filter->test(physicalDevice)) {
                    filterPassed = false;
                    break;
                }
            }
            if (!filterPassed) {
                continue;
            }

            uint32_t score = mScoreCalculator->calculateScore(physicalDevice);
            if (score > maxScore) {
                maxScore = score;
                bestPhysicalDevice = &physicalDevice;
            }
        }

        if (maxScore == 0) {
            LOG_E("suitable PhysicalDevice not found !");
            throw std::runtime_error("suitable PhysicalDevice not found !");
        }

        return std::make_unique<PhysicalDevice>(*bestPhysicalDevice);
    }

    std::unique_ptr<PhysicalDeviceSelector> PhysicalDeviceSelector::makeDefault(const Surface &surface, vk::QueueFlags queueFlags) {
        std::unique_ptr<PhysicalDeviceScoreCalculator> calculator = std::make_unique<PhysicalDeviceScoreCalculator>();
        return std::make_unique<PhysicalDeviceSelector>(std::move(calculator));
    }

} // vklite