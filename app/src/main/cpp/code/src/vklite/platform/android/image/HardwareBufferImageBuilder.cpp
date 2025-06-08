//
// Created by leixing on 2025/4/15.
//

#include "HardwareBufferImageBuilder.h"

namespace vklite {

    HardwareBufferImageBuilder::HardwareBufferImageBuilder() {
        mExternalMemoryImageCreateInfo.setHandleTypes(vk::ExternalMemoryHandleTypeFlagBitsKHR::eAndroidHardwareBufferANDROID);
        mExternalFormat.setPNext(&mExternalMemoryImageCreateInfo);

        mImageBuilder
                .next(&mExternalFormat)
                .flags(vk::ImageCreateFlags{})
                .imageType(vk::ImageType::e2D)
                .mipLevels(1)
                .sampleCount(vk::SampleCountFlagBits::e1)
                .tiling(vk::ImageTiling::eOptimal)
                .usage(vk::ImageUsageFlagBits::eSampled)
                .sharingMode(vk::SharingMode::eExclusive)
                .initialLayout(vk::ImageLayout::eUndefined);
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

    HardwareBufferImageBuilder &HardwareBufferImageBuilder::formatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties) {
        if (formatProperties.format == vk::Format::eUndefined) {
            externalFormat(mExternalFormat.externalFormat);
        }
        mImageBuilder.format(formatProperties.format);
        return *this;
    }


    HardwareBufferImageBuilder &HardwareBufferImageBuilder::hardwareBufferDescription(AHardwareBuffer_Desc hardwareBufferDescription) {
        mImageBuilder
                .extent(vk::Extent3D(hardwareBufferDescription.width, hardwareBufferDescription.height, 1))
                .arrayLayers(hardwareBufferDescription.layers);
        return *this;
    }

    Image HardwareBufferImageBuilder::build() {
        return mImageBuilder.build();
    }

    std::unique_ptr<Image> HardwareBufferImageBuilder::buildUnique() {
        return std::make_unique<Image>(build());
    }

} // vklite