//
// Created by leixing on 2025/1/7.
//

#pragma once

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/command/VulkanCommandPool.h"

#include "vklite/image/VulkanImage.h"
#include "SamplerInterface.h"

namespace vklite {

    class DefaultSampler : public SamplerInterface {
    private:
        const Device &mVulkanDevice;

        vk::Sampler mSampler;

    public:

        DefaultSampler(const Device &device, float maxLoad);

        ~DefaultSampler() override;

        [[nodiscard]]
        const vk::Sampler &getSampler() const override;
    };

} // vklite
