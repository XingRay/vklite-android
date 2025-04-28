//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include "vklite/image/VulkanImageBuilderInterface.h"
#include "VulkanAndroidHardwareBufferYcbcrConversion.h"
#include "AndroidHardwareBuffer.h"

namespace vklite {

    class VulkanAndroidHardwareBufferImageBuilder : public VulkanImageBuilderInterface {
    private:
        const VulkanAndroidHardwareBufferYcbcrConversion &mVulkanAndroidSamplerYcbcrConversion;
        const AndroidHardwareBuffer &mAndroidHardwareBuffer;

    public:
        VulkanAndroidHardwareBufferImageBuilder(const VulkanAndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion,
                                                const AndroidHardwareBuffer &androidHardwareBuffer);

        ~VulkanAndroidHardwareBufferImageBuilder() override;

        std::unique_ptr<VulkanImageInterface> build(const Device &vulkanDevice, uint32_t width, uint32_t height, vk::Format format) override;
    };

} // vklite
