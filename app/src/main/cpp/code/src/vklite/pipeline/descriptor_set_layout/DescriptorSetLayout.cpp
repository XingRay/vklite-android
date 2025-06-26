//
// Created by leixing on 2025-06-19.
//

#include "DescriptorSetLayout.h"

#include <utility>

namespace vklite {

    DescriptorSetLayout::DescriptorSetLayout(vk::Device device, vk::DescriptorSetLayout descriptorSetLayout)
            : mDevice(device), mDescriptorSetLayout(descriptorSetLayout) {}

    DescriptorSetLayout::~DescriptorSetLayout() {
        if (mDevice != nullptr && mDescriptorSetLayout != nullptr) {
            mDevice.destroy(mDescriptorSetLayout);
        }
    }

    DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mDescriptorSetLayout(std::exchange(other.mDescriptorSetLayout, nullptr)) {}

    DescriptorSetLayout &DescriptorSetLayout::operator=(DescriptorSetLayout &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mDescriptorSetLayout = std::exchange(other.mDescriptorSetLayout, nullptr);
        }
        return *this;
    }

    const vk::DescriptorSetLayout &DescriptorSetLayout::getVkDescriptorSetLayout() const {
        return mDescriptorSetLayout;
    }

} // vklite