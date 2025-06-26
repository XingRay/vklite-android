//
// Created by leixing on 2025/6/8.
//

#include "CombinedHardwareBufferImageViewBuilder.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/platform/android/api/VulkanAndroidApi.h"

namespace vklite {

    CombinedHardwareBufferImageViewBuilder::CombinedHardwareBufferImageViewBuilder()
            : mMemoryOffset(0) {}

    CombinedHardwareBufferImageViewBuilder::~CombinedHardwareBufferImageViewBuilder() = default;

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::device(vk::Device device) {
        mDevice = device;
        mHardwareBufferImageBuilder.device(device);
        mHardwareBufferDeviceMemoryBuilder.device(device);
        mHardwareBufferImageViewBuilder.device(device);
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::hardwareBufferDescription(AHardwareBuffer_Desc hardwareBufferDescription) {
        mHardwareBufferDescription = hardwareBufferDescription;
        mHardwareBufferImageBuilder.hardwareBufferDescription(hardwareBufferDescription);
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::hardwareBufferFormatProperties(vk::AndroidHardwareBufferFormatPropertiesANDROID formatProperties) {
        mHardwareBufferFormatProperties = formatProperties;
        mHardwareBufferImageBuilder.hardwareBufferFormatProperties(formatProperties);
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::hardwareBufferProperties(vk::AndroidHardwareBufferPropertiesANDROID hardwareBufferProperties) {
        mHardwareBufferProperties = hardwareBufferProperties;
        mHardwareBufferProperties = hardwareBufferProperties;
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::hardwareBuffer(AHardwareBuffer *hardwareBuffer) {
        mHardwareBuffer = hardwareBuffer;
        mHardwareBufferDeviceMemoryBuilder.hardwareBuffer(hardwareBuffer);
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::hardwareBuffer(const HardwareBuffer &hardwareBuffer) {
        hardwareBufferDescription(hardwareBuffer.getAndroidHardwareBufferDescription());
        hardwareBufferProperties(hardwareBuffer.getProperties());
        hardwareBufferFormatProperties(hardwareBuffer.getFormatProperties());
        this->hardwareBuffer(hardwareBuffer.getHardwareBuffer());
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::memoryProperties(vk::PhysicalDeviceMemoryProperties memoryProperties) {
        mPhysicalDeviceMemoryProperties = memoryProperties;
        return *this;
    }

    CombinedHardwareBufferImageViewBuilder &CombinedHardwareBufferImageViewBuilder::conversion(const vk::SamplerYcbcrConversion &conversion) {
        mSamplerYcbcrConversion = conversion;
        mHardwareBufferImageViewBuilder.conversion(conversion);
        return *this;
    }


    CombinedHardwareBufferImageView CombinedHardwareBufferImageViewBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("CombinedHardwareBufferImageViewBuilder::build(): mDevice == nullptr");
        }

        if (mHardwareBuffer == nullptr) {
            throw std::runtime_error("CombinedHardwareBufferImageViewBuilder::build(): mHardwareBuffer == nullptr");
        }
        if (!mHardwareBufferDescription.has_value()) {
            throw std::runtime_error("CombinedHardwareBufferImageViewBuilder::build(): mHardwareBufferDescription not set");
        }
        if (!mHardwareBufferProperties.has_value()) {
            throw std::runtime_error("CombinedHardwareBufferImageViewBuilder::build(): mHardwareBufferProperties not set");
        }
        if (!mHardwareBufferFormatProperties.has_value()) {
            throw std::runtime_error("CombinedHardwareBufferImageViewBuilder::build(): mHardwareBufferFormatProperties not set");
        }

        if (!mPhysicalDeviceMemoryProperties.has_value()) {
            throw std::runtime_error("CombinedHardwareBufferImageViewBuilder::build(): mPhysicalDeviceMemoryProperties not set");
        }

        if (!mSamplerYcbcrConversion.has_value()) {
            throw std::runtime_error("CombinedHardwareBufferImageViewBuilder::build(): mSamplerYcbcrConversion not set");
        }

        HardwareBufferImage image = mHardwareBufferImageBuilder.build();


        vk::AndroidHardwareBufferPropertiesANDROID &hardwareBufferProperties = mHardwareBufferProperties.value();
        vk::PhysicalDeviceMemoryProperties &physicalDeviceMemoryProperties = mPhysicalDeviceMemoryProperties.value();
        uint32_t memoryTypeIndex = VulkanUtil::findMemoryTypeIndex(physicalDeviceMemoryProperties, hardwareBufferProperties.memoryTypeBits);

        mHardwareBufferDeviceMemoryBuilder.allocationSize(hardwareBufferProperties.allocationSize);
        mHardwareBufferDeviceMemoryBuilder.memoryTypeIndex(memoryTypeIndex);

        DeviceMemory deviceMemory = mHardwareBufferDeviceMemoryBuilder.build();

        image.bindMemory(deviceMemory.getVkDeviceMemory());

        ImageView imageView = mHardwareBufferImageViewBuilder
                .image(image.getImage())
                .build();

        return CombinedHardwareBufferImageView{std::move(image), std::move(deviceMemory), std::move(imageView)};
    }

    std::unique_ptr<CombinedHardwareBufferImageView> CombinedHardwareBufferImageViewBuilder::buildUnique() {
        return std::make_unique<CombinedHardwareBufferImageView>(build());
    }

} // vklite