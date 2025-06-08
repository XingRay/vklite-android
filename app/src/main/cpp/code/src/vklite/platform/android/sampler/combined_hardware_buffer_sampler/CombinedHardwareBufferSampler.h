//
// Created by leixing on 2025/6/7.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/platform/android/conversion/SamplerYcbcrConversion.h"
#include "vklite/image/Image.h"
#include "vklite/device_memory/DeviceMemory.h"
#include "vklite/image_view/ImageView.h"
#include "vklite/sampler/Sampler.h"

namespace vklite {

    class CombinedHardwareBufferSampler {
    private:
        SamplerYcbcrConversion mConversion;
        Image mImage;
        DeviceMemory mDeviceMemory;
        ImageView mImageView;
        Sampler mSampler;

    public:
        CombinedHardwareBufferSampler(SamplerYcbcrConversion &&conversion,
                                      Image &&image,
                                      DeviceMemory &&deviceMemory,
                                      ImageView &&imageView,
                                      Sampler &&sampler);

        ~CombinedHardwareBufferSampler();

        CombinedHardwareBufferSampler(const CombinedHardwareBufferSampler &other) = delete;

        CombinedHardwareBufferSampler &operator=(const CombinedHardwareBufferSampler &other) = delete;

        CombinedHardwareBufferSampler(CombinedHardwareBufferSampler &&other) noexcept;

        CombinedHardwareBufferSampler &operator=(CombinedHardwareBufferSampler &&other) noexcept;


        const SamplerYcbcrConversion &getConversion() const;

        const Image &getImage() const;

        const DeviceMemory &getDeviceMemory() const;

        const ImageView &getImageView() const;

        const Sampler &getSampler() const;

        const vk::Sampler &getVkSampler() const;


    };

} // vklite
