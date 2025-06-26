//
// Created by leixing on 2025/4/27.
//

#include "PhysicalDeviceSelector.h"
#include "vklite/physical_device/filter/SurfaceSupportPhysicalDeviceFilter.h"
#include "vklite/physical_device/filter/QueueSupportPhysicalDeviceFilter.h"

namespace vklite {

    PhysicalDeviceSelector::PhysicalDeviceSelector() = default;

    PhysicalDeviceSelector::~PhysicalDeviceSelector() = default;

    PhysicalDeviceSelector::PhysicalDeviceSelector(PhysicalDeviceSelector &&other) noexcept
            : mScoreCalculator(std::move(other.mScoreCalculator)),
              mPhysicalDeviceFilters(std::move(other.mPhysicalDeviceFilters)) {}

    PhysicalDeviceSelector &PhysicalDeviceSelector::operator=(PhysicalDeviceSelector &&other) noexcept {
        if (this != &other) {
            mScoreCalculator = std::move(other.mScoreCalculator);
            mPhysicalDeviceFilters = std::move(other.mPhysicalDeviceFilters);
        }
        return *this;
    }

    PhysicalDeviceSelector &PhysicalDeviceSelector::scoreCalculator(std::unique_ptr<PhysicalDeviceScoreCalculator> scoreCalculator) {
        mScoreCalculator = std::move(scoreCalculator);
        return *this;
    }

    PhysicalDeviceSelector &PhysicalDeviceSelector::addPhysicalDeviceFilter(std::unique_ptr<PhysicalDeviceFilter> filter) {
        mPhysicalDeviceFilters.push_back(std::move(filter));
        return *this;
    }

    std::optional<PhysicalDevice *> PhysicalDeviceSelector::select(std::vector<PhysicalDevice> &physicalDevices) {
        if (physicalDevices.empty()) {
            LOG_D("PhysicalDeviceSelector::select: No physical devices found!");
            return std::nullopt;
        }

        uint32_t maxScore = 0;
        PhysicalDevice *bestPhysicalDevice;

        for (PhysicalDevice &physicalDevice: physicalDevices) {
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
            LOG_E("PhysicalDeviceSelector::select: suitable PhysicalDevice not found !");
            throw std::runtime_error("PhysicalDeviceSelector::select: suitable PhysicalDevice not found !");
        }

        return bestPhysicalDevice;
    }

    std::unique_ptr<PhysicalDevice> PhysicalDeviceSelector::selectUnique(const std::vector<vk::PhysicalDevice> &vkPhysicalDevices) {
        std::vector<PhysicalDevice> physicalDevices;
        physicalDevices.reserve(physicalDevices.size());
        for (const vk::PhysicalDevice &vkPhysicalDevice: vkPhysicalDevices) {
            physicalDevices.emplace_back(vkPhysicalDevice);
        }
        std::optional<PhysicalDevice *> selected = select(physicalDevices);
        if (!selected.has_value()) {
            return nullptr;
        }
        return std::make_unique<PhysicalDevice>(std::move(*(selected.value())));
    }

    std::optional<vk::PhysicalDevice> PhysicalDeviceSelector::select(const std::vector<vk::PhysicalDevice> &physicalDevices) {
        std::unique_ptr<PhysicalDevice> physicalDevice = selectUnique(physicalDevices);
        if (physicalDevice == nullptr) {
            return std::nullopt;
        }
        return physicalDevice->getVkPhysicalDevice();
    }


    PhysicalDeviceSelector PhysicalDeviceSelector::makeDefault(const Surface &surface, vk::QueueFlags queueFlags) {
        PhysicalDeviceSelector deviceSelector = PhysicalDeviceSelector();

        // set score calculator
        deviceSelector.scoreCalculator(std::make_unique<PhysicalDeviceScoreCalculator>());

        // surface support required
        deviceSelector.addPhysicalDeviceFilter(std::make_unique<SurfaceSupportPhysicalDeviceFilter>(surface));

        // queue flags support required
        deviceSelector.addPhysicalDeviceFilter(std::make_unique<QueueSupportPhysicalDeviceFilter>(queueFlags));

        return std::move(deviceSelector);
    }

} // vklite