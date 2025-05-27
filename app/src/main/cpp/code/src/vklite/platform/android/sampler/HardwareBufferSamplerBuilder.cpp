//
// Created by leixing on 2025/4/15.
//

#include "HardwareBufferSamplerBuilder.h"
#include "HardwareBufferSampler.h"

namespace vklite {

    HardwareBufferSamplerBuilder::HardwareBufferSamplerBuilder(const HardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion)
            : mAndroidSamplerYcbcrConversion(vulkanAndroidSamplerYcbcrConversion) {}

    HardwareBufferSamplerBuilder::~HardwareBufferSamplerBuilder() = default;

    std::unique_ptr<SamplerInterface> HardwareBufferSamplerBuilder::build(const PhysicalDevice &physicalDevice, const Device &device) {
        return std::make_unique<HardwareBufferSampler>(device, mAndroidSamplerYcbcrConversion);
    }

} // vklite