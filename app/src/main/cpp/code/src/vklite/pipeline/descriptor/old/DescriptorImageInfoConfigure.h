//
// Created by leixing on 2025/4/8.
//

#pragma once

#include <memory>

//#include "image/ImageInterface.h"

#include "vklite/device/Device.h"
#include "vklite/command_buffer/CommandPool.h"
#include "vklite/image/ImageBuilderInterface.h"
#include "vklite/sampler/SamplerBuilderInterface.h"

namespace vklite {

    class DescriptorImageInfoConfigure {
    private:

//        // set ImageInfo
//        std::unique_ptr<ImageInfo> mDescriptorBufferInfo;
//
//        //create ImageInfo
//        std::unique_ptr<ImageBuilderInterface> mImageBuilder;
//        std::unique_ptr<SamplerBuilderInterface> mSamplerBuilder;

        // image data
//        std::shared_ptr<ImageInterface> mImage;

    public:
//        DescriptorImageInfoConfigure(
//                std::unique_ptr<ImageBuilderInterface> &&imageBuilder,
//                std::unique_ptr<SamplerBuilderInterface> &&samplerBuilder/*,
//                std::unique_ptr<vklite::ImageInterface> &&image*/);
//
//        ~DescriptorImageInfoConfigure();
//
//        std::unique_ptr<ImageInfo> provideDescriptorImageInfo(const Device &device, const CommandPool &commandPool);
    };

} // vklite
