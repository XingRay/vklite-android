//
// Created by leixing on 2025/1/7.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/command_buffer/CommandPool.h"

#include "vklite/image/Image.h"
#include "SamplerInterface.h"

namespace vklite {

    class DefaultSampler : public SamplerInterface {
    private:
        const Device &mDevice;

        vk::Sampler mSampler;

    public:

        DefaultSampler(const Device &device, float maxLoad);

        ~DefaultSampler() override;

        [[nodiscard]]
        const vk::Sampler &getSampler() const override;
    };

} // vklite
