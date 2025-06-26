//
// Created by leixing on 2025/1/7.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/command_pool/CommandPool.h"

#include "vklite/image/Image.h"

namespace vklite {

    class Sampler {
    private:
        vk::Device mDevice;
        vk::Sampler mSampler;

    public:

        explicit Sampler(vk::Device device, vk::Sampler sampler);

        ~Sampler();

        Sampler(const Sampler &other) = delete;

        Sampler &operator=(const Sampler &other) = delete;

        Sampler(Sampler &&other) noexcept;

        Sampler &operator=(Sampler &&other) noexcept;

        [[nodiscard]]
        const vk::Sampler &getVkSampler() const;
    };

} // vklite
