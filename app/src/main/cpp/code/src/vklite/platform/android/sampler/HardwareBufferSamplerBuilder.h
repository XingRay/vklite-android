//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include "vklite/sampler/Sampler.h"
#include "HardwareBufferYcbcrConversion.h"
#include "vklite/physical_device/PhysicalDevice.h"

namespace vklite {

    class HardwareBufferSamplerBuilder {
    private:
        const HardwareBufferYcbcrConversion &mAndroidSamplerYcbcrConversion;

    public:

        HardwareBufferSamplerBuilder(const HardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion);

        ~HardwareBufferSamplerBuilder();

        std::unique_ptr<Sampler> build(const PhysicalDevice &physicalDevice, const Device &device);

    };

} // vklite
