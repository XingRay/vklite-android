//
// Created by leixing on 2025/3/15.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class SamplerYcbcrConversion {
    private:
        vk::Device mDevice;

        vk::SamplerYcbcrConversion mSamplerYcbcrConversion;

    public:
        explicit SamplerYcbcrConversion(vk::Device device, vk::SamplerYcbcrConversion samplerYcbcrConversion);

        ~SamplerYcbcrConversion();

        SamplerYcbcrConversion(const SamplerYcbcrConversion &other) = delete;

        SamplerYcbcrConversion &operator=(const SamplerYcbcrConversion &other) = delete;

        SamplerYcbcrConversion(SamplerYcbcrConversion &&other) noexcept;

        SamplerYcbcrConversion &operator=(SamplerYcbcrConversion &&other) noexcept;

        [[nodiscard]]
        const vk::SamplerYcbcrConversion &getVkSamplerYcbcrConversion() const;
    };

} // vklite
