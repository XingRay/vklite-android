//
// Created by leixing on 2025/3/15.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class HardwareBufferYcbcrConversion {
    private:
        vk::Device mDevice;

        vk::SamplerYcbcrConversion mSamplerYcbcrConversion;

    public:
        explicit HardwareBufferYcbcrConversion(vk::Device device, vk::SamplerYcbcrConversion samplerYcbcrConversion);

        ~HardwareBufferYcbcrConversion();

        HardwareBufferYcbcrConversion(const HardwareBufferYcbcrConversion &other) = delete;

        HardwareBufferYcbcrConversion &operator=(const HardwareBufferYcbcrConversion &other) = delete;

        HardwareBufferYcbcrConversion(HardwareBufferYcbcrConversion &&other) noexcept;

        HardwareBufferYcbcrConversion &operator=(HardwareBufferYcbcrConversion &&other) noexcept;

        [[nodiscard]]
        const vk::SamplerYcbcrConversion &getSamplerYcbcrConversion() const;
    };

} // vklite
