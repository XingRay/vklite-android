//
// Created by leixing on 2025/1/26.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/sampler/Sampler.h"
#include "vklite/platform/android/hardware_buffer/HardwareBuffer.h"
#include "HardwareBufferYcbcrConversion.h"
#include "vklite/platform/android/image/HardwareBufferImage.h"
#include "vklite/sampler/SamplerInterface.h"

namespace vklite {

    class HardwareBufferSampler : public SamplerInterface {
    private:
        const Device &mDevice;
        vk::Sampler mSampler;

    public:

        HardwareBufferSampler(const Device &device,
                              const HardwareBufferYcbcrConversion &samplerYcbcrConversion);

        ~HardwareBufferSampler() override;

        [[nodiscard]]
        const vk::Sampler &getSampler() const override;

    };

} // vklite
