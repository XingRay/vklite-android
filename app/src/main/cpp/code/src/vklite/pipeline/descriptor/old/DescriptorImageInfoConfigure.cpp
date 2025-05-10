//
// Created by leixing on 2025/4/8.
//

#include "DescriptorImageInfoConfigure.h"

namespace vklite {

    DescriptorImageInfoConfigure::DescriptorImageInfoConfigure(
            std::unique_ptr<ImageBuilderInterface> &&imageBuilder,
            std::unique_ptr<SamplerBuilderInterface> &&samplerBuilder/*,
            std::unique_ptr<vklite::ImageInterface> &&image*/)
            : mImageBuilder(std::move(imageBuilder)),
              mSamplerBuilder(std::move(samplerBuilder))/*,
              mImage(std::move(image))*/ {}

    DescriptorImageInfoConfigure::~DescriptorImageInfoConfigure() = default;

    std::unique_ptr<ImageInfo> DescriptorImageInfoConfigure::provideDescriptorImageInfo(const Device &device, const CommandPool &commandPool) {
        if(mDescriptorBufferInfo!= nullptr){
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