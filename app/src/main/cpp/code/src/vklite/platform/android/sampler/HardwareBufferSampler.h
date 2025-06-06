//
// Created by leixing on 2025/1/26.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include "vklite/device/Device.h"
#include "vklite/sampler/Sampler.h"
#include "vklite/platform/android/hardware_buffer/HardwareBuffer.h"
#include "HardwareBufferYcbcrConversion.h"
#include "vklite/platform/android/image/HardwareBufferImage.h"

namespace vklite {

    class HardwareBufferSampler {
    private:
        const Device &mDevice;
        vk::Sampler mSampler;

    public:

        HardwareBufferSampler(const Device &device,
                              const HardwareBufferYcbcrConversion &samplerYcbcrConversion);

        ~HardwareBufferSampler();

        HardwareBufferSampler(const HardwareBufferSampler &other) = delete;

        HardwareBufferSampler &operator=(const HardwareBufferSampler &other) = delete;

        HardwareBufferSampler(HardwareBufferSampler &&other) noexcept;

        HardwareBufferSampler &operator=(HardwareBufferSampler &&other) noexcept = delete;


        [[nodiscard]]
        const vk::Sampler &getSampler() const;

    };

} // vklite
