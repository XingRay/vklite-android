//
// Created by leixing on 2025/4/8.
//

#pragma once

#include <memory>

#include "vklite/ImageInterface.h"

#include "vklite/device/Device.h"
#include "vklite/command_pool/CommandPool.h"
#include "vklite/image/VulkanImageBuilderInterface.h"
#include "vklite/sampler/builder/SamplerBuilderInterface.h"
#include "vklite/pipeline_resource/ImageInfo.h"

namespace vklite {

    class VulkanDescriptorImageInfoConfigure {
    private:

        // set ImageInfo
        std::unique_ptr<ImageInfo> mDescriptorBufferInfo;

        //create ImageInfo
        std::unique_ptr<VulkanImageBuilderInterface> mImageBuilder;
        std::unique_ptr<SamplerBuilderInterface> mSamplerBuilder;

        // image data
        std::shared_ptr<ImageInterface> mImage;

    public:
        VulkanDescriptorImageInfoConfigure(
                std::unique_ptr<VulkanImageBuilderInterface> &&vulkanImageBuilder,
                std::unique_ptr<SamplerBuilderInterface> &&vulkanSamplerBuilder,
                std::unique_ptr<vklite::ImageInterface> &&image);

        ~VulkanDescriptorImageInfoConfigure();

        std::unique_ptr<ImageInfo> provideVulkanDescriptorImageInfo(const Device &device, const CommandPool &commandPool);
    };

} // vklite
