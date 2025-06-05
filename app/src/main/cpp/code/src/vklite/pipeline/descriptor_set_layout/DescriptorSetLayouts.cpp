//
// Created by leixing on 2025/6/5.
//

#include "DescriptorSetLayouts.h"

#include <utility>

#include "vklite/Log.h"

namespace vklite {

    DescriptorSetLayouts::DescriptorSetLayouts(vk::Device device, std::vector<vk::DescriptorSetLayout> &&descriptorSetLayouts)
            : mDevice(device),
              mDescriptorSetLayouts(std::move(descriptorSetLayouts)) {}

    DescriptorSetLayouts::~DescriptorSetLayouts() {
        if (mDevice != nullptr && !mDescriptorSetLayouts.empty()) {
            for (const vk::DescriptorSetLayout &descriptorSetLayout: mDescriptorSetLayouts) {
                LOG_D("mDevice.destroy(descriptorSetLayout), descriptorSetLayout:%p", (void *) descriptorSetLayout);
                mDevice.destroy(descriptorSetLayout);
            }
            mDevice = nullptr;
            mDescriptorSetLayouts.clear();
        }
    }

    DescriptorSetLayouts::DescriptorSetLayouts(DescriptorSetLayouts &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mDescriptorSetLayouts(std::move(other.mDescriptorSetLayouts)) {}

    DescriptorSetLayouts &DescriptorSetLayouts::operator=(DescriptorSetLayouts &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mDescriptorSetLayouts = std::move(other.mDescriptorSetLayouts);
        }
        return *this;
    }

    const std::vector<vk::DescriptorSetLayout> &DescriptorSetLayouts::getDescriptorSetLayouts() const {
        return mDescriptorSetLayouts;
    }

} // vklite