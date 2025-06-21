//
// Created by leixing on 2025/5/31.
//

#include "BufferBuilder.h"
#include "vklite/Log.h"

namespace vklite {

    BufferBuilder::BufferBuilder()
            : mBufferCreateInfo(vk::BufferCreateInfo{}) {}

    BufferBuilder::~BufferBuilder() = default;

    BufferBuilder &BufferBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    BufferBuilder &BufferBuilder::bufferSize(vk::DeviceSize bufferSize) {
        mBufferCreateInfo.setSize(bufferSize);
        return *this;
    }

    BufferBuilder &BufferBuilder::sharingMode(vk::SharingMode sharingMode) {
        mBufferCreateInfo.setSharingMode(sharingMode);
        return *this;
    }

    BufferBuilder &BufferBuilder::usage(vk::BufferUsageFlags usage) {
        mBufferCreateInfo.usage = usage;
        return *this;
    }

    BufferBuilder &BufferBuilder::addUsage(vk::BufferUsageFlags usage) {
        mBufferCreateInfo.usage |= usage;
        return *this;
    }

    Buffer BufferBuilder::build() const {
//        LOG_D("BufferBuilder::build()");
        if (mDevice == nullptr) {
            throw std::runtime_error("DeviceLocalBufferBuilder::build() mDevice == nullptr");
        }
        if (mBufferCreateInfo.size == 0) {
            throw std::runtime_error("DeviceLocalBufferBuilder::build() mBufferCreateInfo.size == 0");
        }

        vk::Buffer buffer = mDevice.createBuffer(mBufferCreateInfo);
//        LOG_D("mDevice.createBuffer => %p", (void *) buffer);

        BufferMeta meta{mBufferCreateInfo.size};

//        return Buffer(mDevice, buffer, std::move(meta));
        return Buffer{mDevice, buffer, std::move(meta)};
    }

    std::unique_ptr<Buffer> BufferBuilder::buildUnique() const {
        return std::make_unique<Buffer>(build());
    }

} // vklite