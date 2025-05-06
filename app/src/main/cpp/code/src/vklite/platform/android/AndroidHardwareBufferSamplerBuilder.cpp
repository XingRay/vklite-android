//
// Created by leixing on 2025/4/15.
//

#include "AndroidHardwareBufferSamplerBuilder.h"
#include "AndroidHardwareBufferSampler.h"

namespace vklite {

    AndroidHardwareBufferSamplerBuilder::AndroidHardwareBufferSamplerBuilder(const AndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion)
            : mAndroidSamplerYcbcrConversion(vulkanAndroidSamplerYcbcrConversion) {}

    AndroidHardwareBufferSamplerBuilder::~AndroidHardwareBufferSamplerBuilder() = default;

    std::unique_ptr<SamplerInterface> AndroidHardwareBufferSamplerBuilder::build(const Device &device) {
        return std::make_unique<AndroidHardwareBufferSampler>(device, mAndroidSamplerYcbcrConversion);
    }

} // vklite