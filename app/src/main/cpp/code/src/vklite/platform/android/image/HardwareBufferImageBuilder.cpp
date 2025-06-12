//
// Created by leixing on 2025/4/15.
//

#include "HardwareBufferImageBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    HardwareBufferImageBuilder::HardwareBufferImageBuilder() :
            mExternalFormat{}, mExternalMemoryImageCreateInfo{} {
        mExternalMemoryImageCreateInfo.setHandleTypes(vk::ExternalMemoryHandleTypeFlagBitsKHR::eAndroidHardwareBufferANDROID);
        mExternalFormat.setPNext(&mExternalMemoryImageCreateInfo);

        mImageCreateInfo
                .setPNext(&mExternalFormat)
                .setFlags(vk::ImageCreateFlags{})
                .setImageType(vk::ImageType::e2D)
                .setMipLevels(1)
                .setSamples(vk::SampleCountFlagBits::e1)
                .setTiling(vk::ImageTiling::eOptimal)
                .setUsage(vk::ImageUsageFlagBits::eSampled)
                .setSharingMode(vk::SharingMode::eExclusive)
                .setInitialLayout(vk::ImageLayout::eUndefined);
    }

    HardwareBufferImageBuilder::~HardwareBufferImageBuilder() = default;

    HardwareBufferImageBuilder &HardwareBufferImageBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    HardwareBufferImageBuilder &HardwareBufferImageBuilder::externalFormat(uint64_t externalFormat) {
        mExternalFormat.setExternalFormat(externalFormat);
        return *this;
    }

    HardwareBufferImageBuilder &HardwareBufferImageBuilder::format(vk::Format format) {
        mImageCreateInfo.setFormat(format);
        return *this;
    }

    HardwareBufferImageBuilder &HardwareBufferImageBuilder::hardwareBufferFormatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID hardwareBufferFormatProperties) {
        mHardwareBufferFormatProperties = hardwareBufferFormatProperties;

        if (hardwareBufferFormatProperties.format == vk::Format::eUndefined) {
            externalFormat(hardwareBufferFormatProperties.externalFormat);
        }
        format(hardwareBufferFormatProperties.format);

        return *this;
    }


    HardwareBufferImageBuilder &HardwareBufferImageBuilder::hardwareBufferDescription(AHardwareBuffer_Desc hardwareBufferDescription) {
        mHardwareBufferDescription = hardwareBufferDescription;

        mImageCreateInfo
                .setExtent(vk::Extent3D(hardwareBufferDescription.width, hardwareBufferDescription.height, 1))
                .setArrayLayers(hardwareBufferDescription.layers);
        return *this;
    }

    HardwareBufferImage HardwareBufferImageBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("HardwareBufferImageBuilder::build(): mDevice == nullptr");
        }
        if (!mHardwareBufferFormatProperties.has_value()) {
            throw std::runtime_error("HardwareBufferImageBuilder::build(): mHardwareBufferFormatProperties not set");
        }
        if (!mHardwareBufferDescription.has_value()) {
            throw std::runtime_error("HardwareBufferImageBuilder::build(): mHardwareBufferDescription not set");
        }

        vk::Image image = mDevice.createImage(mImageCreateInfo);
//        LOG_D("mDevice.createImage(mImageCreateInfo) => %p", (void *) image);

        HardwareBufferImageMeta meta{mImageCreateInfo.format, mImageCreateInfo.extent, mImageCreateInfo.mipLevels};

        return HardwareBufferImage{mDevice, image, meta};
    }

    std::unique_ptr<HardwareBufferImage> HardwareBufferImageBuilder::buildUnique() {
        return std::make_unique<HardwareBufferImage>(build());
    }

} // vklite