//
// Created by leixing on 2025/4/8.
//

#include "vklite/configure/descriptor/VulkanDescriptorImmutableSamplerConfigure.h"

namespace vklite {

    VulkanDescriptorImmutableSamplerConfigure::VulkanDescriptorImmutableSamplerConfigure(
            std::unique_ptr<ImageBuilderInterface> &&vulkanImageBuilder,
            std::unique_ptr<SamplerBuilderInterface> &&vulkanSamplerBuilder/*,
            std::unique_ptr<vklite::ImageInterface> &&image*/)
            : mImageBuilder(std::move(vulkanImageBuilder)),
              mSamplerBuilder(std::move(vulkanSamplerBuilder))/*,
              mImage(std::move(image))*/ {}

    VulkanDescriptorImmutableSamplerConfigure::~VulkanDescriptorImmutableSamplerConfigure() = default;

    std::unique_ptr<ImageInfo> VulkanDescriptorImmutableSamplerConfigure::provideVulkanDescriptorImageInfo(const Device &device, const CommandPool &commandPool) {
        if (mDescriptorBufferInfo != nullptr) {
            return std::move(mDescriptorBufferInfo);
        }

//        std::unique_ptr<ImageInterface> vulkanImage = mImageBuilder->build(device, mImage->getWidth(), mImage->getHeight(), mImage->getFormat());
//        vulkanImage->transitionImageLayout(commandPool);
//        vulkanImage->update(commandPool, mImage->getPixels(), mImage->getPixelBytes());
//        std::unique_ptr<SamplerInterface> sampler = mSamplerBuilder->build(device);
//        return std::make_unique<ImageInfo>(std::move(vulkanImage), std::move(sampler));
        return nullptr;
    }

} // vklite