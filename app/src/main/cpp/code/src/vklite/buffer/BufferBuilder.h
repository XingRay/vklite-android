//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <memory>

#include <vulkan/vulkan.hpp>

#include "vklite/buffer/Buffer.h"

namespace vklite {

    class BufferBuilder {
    private:
        vk::Device mDevice;
        vk::BufferCreateInfo mBufferCreateInfo;

    public:
        BufferBuilder();

        ~BufferBuilder();

        BufferBuilder &device(vk::Device device);

        BufferBuilder &bufferSize(vk::DeviceSize bufferSize);

        BufferBuilder &sharingMode(vk::SharingMode sharingMode);

        BufferBuilder &usage(vk::BufferUsageFlags usage);

        BufferBuilder &addUsage(vk::BufferUsageFlags usage);

        [[nodiscard]]
        Buffer build() const;

        [[nodiscard]]
        std::unique_ptr<Buffer> buildUnique() const;
    };

} // vklite