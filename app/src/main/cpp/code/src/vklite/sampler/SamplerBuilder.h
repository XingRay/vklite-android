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

        SamplerBuilder &maxAnisotropy(float maxAnisotropy);

        SamplerBuilder &magFilter(vk::Filter filter);

        SamplerBuilder &minFilter(vk::Filter filter);

        SamplerBuilder &addressModeU(vk::SamplerAddressMode mode);

        SamplerBuilder &addressModeV(vk::SamplerAddressMode mode);

        SamplerBuilder &addressModeW(vk::SamplerAddressMode mode);

        SamplerBuilder &anisotropyEnable(vk::Bool32 enable);

        SamplerBuilder &borderColor(vk::BorderColor borderColor);

        SamplerBuilder &unnormalizedCoordinates(vk::Bool32 unnormalizedCoordinates);

        SamplerBuilder &compareEnable(vk::Bool32 enable);

        SamplerBuilder &compareOp(vk::CompareOp op);

        SamplerBuilder &mipmapMode(vk::SamplerMipmapMode mode);

        SamplerBuilder &mipLodBias(float bias);

        SamplerBuilder &minLod(float minLod);

        SamplerBuilder &maxLod(float maxLod);

        SamplerBuilder &next(const void *next);

        SamplerBuilder &filter(vk::Filter filter);

        SamplerBuilder &addressMode(vk::SamplerAddressMode mode);

        SamplerBuilder &lod(float min, float max);

        [[nodiscard]]
        Sampler build();

        [[nodiscard]]
        std::vector<Sampler> build(uint32_t count);

        [[nodiscard]]
        std::unique_ptr<Sampler> buildUnique();

    };

} // vklite
