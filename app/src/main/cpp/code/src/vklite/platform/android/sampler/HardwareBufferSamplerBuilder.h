//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include "vklite/sampler/Sampler.h"
#include "vklite/sampler/SamplerBuilder.h"

namespace vklite {

    class HardwareBufferSamplerBuilder {
    private:
        vk::Device mDevice;

        SamplerBuilder mSamplerBuilder;
        vk::SamplerYcbcrConversionInfo mConversionInfo;
    public:

        explicit HardwareBufferSamplerBuilder();

        ~HardwareBufferSamplerBuilder();

        HardwareBufferSamplerBuilder &device(vk::Device device);

        HardwareBufferSamplerBuilder &ycbcrConversion(vk::SamplerYcbcrConversion ycbcrConversion);

        Sampler build();

        std::unique_ptr<Sampler> buildUnique();

    };

} // vklite
