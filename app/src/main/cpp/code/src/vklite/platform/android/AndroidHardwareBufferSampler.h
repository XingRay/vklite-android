//
// Created by leixing on 2025/1/26.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/sampler/Sampler.h"
#include "AndroidHardwareBuffer.h"
#include "AndroidHardwareBufferYcbcrConversion.h"
#include "AndroidHardwareBufferImage.h"
#include "vklite/sampler/SamplerInterface.h"

namespace vklite {

    class AndroidHardwareBufferSampler : public SamplerInterface {
    private:
        const Device &mDevice;
        vk::Sampler mSampler;

    public:

        AndroidHardwareBufferSampler(const Device &device,
                                     const AndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion);

        ~AndroidHardwareBufferSampler() override;

        [[nodiscard]]
        const vk::Sampler &getSampler() const override;

    };

} // vklite
