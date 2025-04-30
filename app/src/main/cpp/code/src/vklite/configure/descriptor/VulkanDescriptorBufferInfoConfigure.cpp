//
// Created by leixing on 2025/3/31.
//

#include "vklite/configure/descriptor/VulkanDescriptorBufferInfoConfigure.h"

namespace vklite {

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo, const void *data, uint32_t size)
            : mDescriptorBufferInfo(std::move(bufferInfo)),
              mBufferCapacity(0), mBufferOffset(0), mBufferRange(0) {

    }

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo, std::vector<uint8_t> &&data)
            : mDescriptorBufferInfo(std::move(bufferInfo)), mBufferData(std::move(data)),
              mBufferCapacity(0), mBufferOffset(0), mBufferRange(0) {}

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo)
            : mDescriptorBufferInfo(std::move(bufferInfo)),
              mBufferCapacity(0), mBufferOffset(0), mBufferRange(0) {}

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, const void *data, uint32_t size)
            : mBufferBuilder(std::move(vulkanBufferBuilder)),
              mBufferCapacity(vulkanBufferCapacity), mBufferOffset(vulkanBufferOffset), mBufferRange(vulkanBufferRange) {
        copyData(data, size);
    }

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange, std::vector<uint8_t> &&data)
            : mBufferBuilder(std::move(vulkanBufferBuilder)),
              mBufferCapacity(vulkanBufferCapacity), mBufferOffset(vulkanBufferOffset), mBufferRange(vulkanBufferRange),
              mBufferData(std::move(data)) {}

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         uint32_t vulkanBufferOffset, uint32_t vulkanBufferRange)
            : mBufferBuilder(std::move(vulkanBufferBuilder)),
              mBufferCapacity(vulkanBufferCapacity), mBufferOffset(vulkanBufferOffset), mBufferRange(vulkanBufferRange) {}

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         const void *data, uint32_t size)
            : mBufferBuilder(std::move(vulkanBufferBuilder)),
              mBufferCapacity(vulkanBufferCapacity), mBufferOffset(0), mBufferRange(size) {
        copyData(data, size);
    }

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity,
                                                         std::vector<uint8_t> &&data)
            : mBufferBuilder(std::move(vulkanBufferBuilder)),
              mBufferCapacity(vulkanBufferCapacity), mBufferOffset(0), mBufferRange(data.size()),
              mBufferData(std::move(data)) {}

    VulkanDescriptorBufferInfoConfigure::VulkanDescriptorBufferInfoConfigure(std::unique_ptr<VulkanBufferBuilderInterface> &&vulkanBufferBuilder, vk::DeviceSize vulkanBufferCapacity)
            : mBufferBuilder(std::move(vulkanBufferBuilder)), mBufferCapacity(vulkanBufferCapacity), mBufferOffset(0), mBufferRange(vulkanBufferCapacity) {}

    VulkanDescriptorBufferInfoConfigure::~VulkanDescriptorBufferInfoConfigure() = default;

    std::unique_ptr<BufferInfo> VulkanDescriptorBufferInfoConfigure::provideVulkanDescriptorBufferInfo(const Device &device, const CommandPool &commandPool) {
        if (mDescriptorBufferInfo != nullptr) {
            if (!mBufferData.empty()) {
                mDescriptorBufferInfo->update(commandPool, mBufferData.data(), mBufferData.size());
            }
            return std::move(mDescriptorBufferInfo);
        }

        if (mBufferBuilder == nullptr) {
            return nullptr;
        }

        std::unique_ptr<VulkanBufferInterface> vulkanBuffer = mBufferBuilder->build(device, mBufferCapacity);
        if (!mBufferData.empty()) {
            vulkanBuffer->update(commandPool, mBufferData.data(), mBufferData.size());
        }
        return std::make_unique<BufferInfo>(std::move(vulkanBuffer), mBufferOffset, mBufferRange);
    }

    VulkanDescriptorBufferInfoConfigure &VulkanDescriptorBufferInfoConfigure::copyData(const void *data, uint32_t size) {
        if (data == nullptr) {
            mBufferData.clear();
            return *this;
        }

        if (size == 0) {
            mBufferData.clear();
            return *this;
        }

        mBufferData.resize(size);
        std::memcpy(mBufferData.data(), data, size);

        // or:
        // const uint8_t* byteData = static_cast<const uint8_t*>(data);
        // std::copy(byteData, byteData + size, mBufferData.begin());

        return *this;
    }

} // vklite