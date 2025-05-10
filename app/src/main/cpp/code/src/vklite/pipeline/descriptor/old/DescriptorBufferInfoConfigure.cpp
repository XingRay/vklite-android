//
// Created by leixing on 2025/3/31.
//

#include "DescriptorBufferInfoConfigure.h"

namespace vklite {

    DescriptorBufferInfoConfigure::DescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo, const void *data, uint32_t size)
            : mDescriptorBufferInfo(std::move(bufferInfo)),
              mBufferCapacity(0), mBufferOffset(0), mBufferRange(0) {

    }

    DescriptorBufferInfoConfigure::DescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo, std::vector<uint8_t> &&data)
            : mDescriptorBufferInfo(std::move(bufferInfo)), mBufferData(std::move(data)),
              mBufferCapacity(0), mBufferOffset(0), mBufferRange(0) {}

    DescriptorBufferInfoConfigure::DescriptorBufferInfoConfigure(std::unique_ptr<BufferInfo> &&bufferInfo)
            : mDescriptorBufferInfo(std::move(bufferInfo)),
              mBufferCapacity(0), mBufferOffset(0), mBufferRange(0) {}

    DescriptorBufferInfoConfigure::DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&bufferBuilder, vk::DeviceSize bufferCapacity,
                                                                 uint32_t bufferOffset, uint32_t bufferRange, const void *data, uint32_t size)
            : mBufferBuilder(std::move(bufferBuilder)),
              mBufferCapacity(bufferCapacity), mBufferOffset(bufferOffset), mBufferRange(bufferRange) {
        copyData(data, size);
    }

    DescriptorBufferInfoConfigure::DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&bufferBuilder, vk::DeviceSize bufferCapacity,
                                                                 uint32_t bufferOffset, uint32_t bufferRange, std::vector<uint8_t> &&data)
            : mBufferBuilder(std::move(bufferBuilder)),
              mBufferCapacity(bufferCapacity), mBufferOffset(bufferOffset), mBufferRange(bufferRange),
              mBufferData(std::move(data)) {}

    DescriptorBufferInfoConfigure::DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&bufferBuilder, vk::DeviceSize bufferCapacity,
                                                                 uint32_t bufferOffset, uint32_t bufferRange)
            : mBufferBuilder(std::move(bufferBuilder)),
              mBufferCapacity(bufferCapacity), mBufferOffset(bufferOffset), mBufferRange(bufferRange) {}

    DescriptorBufferInfoConfigure::DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&bufferBuilder, vk::DeviceSize bufferCapacity,
                                                                 const void *data, uint32_t size)
            : mBufferBuilder(std::move(bufferBuilder)),
              mBufferCapacity(bufferCapacity), mBufferOffset(0), mBufferRange(size) {
        copyData(data, size);
    }

    DescriptorBufferInfoConfigure::DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&bufferBuilder, vk::DeviceSize bufferCapacity,
                                                                 std::vector<uint8_t> &&data)
            : mBufferBuilder(std::move(bufferBuilder)),
              mBufferCapacity(bufferCapacity), mBufferOffset(0), mBufferRange(data.size()),
              mBufferData(std::move(data)) {}

    DescriptorBufferInfoConfigure::DescriptorBufferInfoConfigure(std::unique_ptr<BufferBuilderInterface> &&bufferBuilder, vk::DeviceSize bufferCapacity)
            : mBufferBuilder(std::move(bufferBuilder)), mBufferCapacity(bufferCapacity), mBufferOffset(0), mBufferRange(bufferCapacity) {}

    DescriptorBufferInfoConfigure::~DescriptorBufferInfoConfigure() = default;

    std::unique_ptr<BufferInfo> DescriptorBufferInfoConfigure::provideDescriptorBufferInfo(const Device &device, const CommandPool &commandPool) {
        if (mDescriptorBufferInfo != nullptr) {
            if (!mBufferData.empty()) {
                mDescriptorBufferInfo->update(commandPool, mBufferData.data(), mBufferData.size());
            }
            return std::move(mDescriptorBufferInfo);
        }

        if (mBufferBuilder == nullptr) {
            return nullptr;
        }

        std::unique_ptr<BufferInterface> buffer = mBufferBuilder->build(device, mBufferCapacity);
        if (!mBufferData.empty()) {
            buffer->update(commandPool, mBufferData.data(), mBufferData.size());
        }
        return std::make_unique<BufferInfo>(std::move(buffer), mBufferOffset, mBufferRange);
    }

    DescriptorBufferInfoConfigure &DescriptorBufferInfoConfigure::copyData(const void *data, uint32_t size) {
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