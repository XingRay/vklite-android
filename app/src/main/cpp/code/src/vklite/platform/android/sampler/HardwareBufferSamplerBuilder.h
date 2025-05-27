//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include "vklite/sampler/SamplerBuilderInterface.h"
#include "HardwareBufferYcbcrConversion.h"

namespace vklite {

    class HardwareBufferSamplerBuilder : public SamplerBuilderInterface {
    private:
        const HardwareBufferYcbcrConversion &mAndroidSamplerYcbcrConversion;

    public:

        HardwareBufferSamplerBuilder(const HardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion);

        ~HardwareBufferSamplerBuilder() override;

        std::unique_ptr<SamplerInterface> build(const PhysicalDevice &physicalDevice, const Device &device) override;

    };

} // vklite
