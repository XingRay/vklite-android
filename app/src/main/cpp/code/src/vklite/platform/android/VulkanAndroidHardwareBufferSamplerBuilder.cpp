//
// Created by leixing on 2025/4/15.
//

#include "VulkanAndroidHardwareBufferSamplerBuilder.h"
#include "VulkanAndroidHardwareBufferSampler.h"

namespace vklite {

    VulkanAndroidHardwareBufferSamplerBuilder::VulkanAndroidHardwareBufferSamplerBuilder(const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion)
            : mAndroidSamplerYcbcrConversion(vulkanAndroidSamplerYcbcrConversion) {}

    VulkanAndroidHardwareBufferSamplerBuilder::~VulkanAndroidHardwareBufferSamplerBuilder() = default;

    std::unique_ptr<SamplerInterface> VulkanAndroidHardwareBufferSamplerBuilder::build(const Device &device) {
        return std::make_unique<VulkanAndroidHardwareBufferSampler>(device, mAndroidSamplerYcbcrConversion);
    }

} // vklite