//
// Created by leixing on 2025/4/11.
//

#pragma once

#include <memory>

#include <vulkan//vulkan.hpp>

#include "vklite/sampler/Sampler.h"

namespace vklite {

    class SamplerBuilder {
    private:
        vk::Device mDevice;
        vk::SamplerCreateInfo mSamplerCreateInfo;

    public:

        SamplerBuilder();

        ~SamplerBuilder();

        SamplerBuilder &device(vk::Device device);

        SamplerBuilder &maxLoad(float maxLoad);

        SamplerBuilder &maxAnisotropy(float maxAnisotropy);

        [[nodiscard]]
        Sampler build();

        [[nodiscard]]
        std::unique_ptr<Sampler> buildUnique();

    };

} // vklite
