//
// Created by leixing on 2025/5/16.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/index_buffer/IndexBuffer.h"

namespace vklite {

    class IndexBufferInfo {
    private:
        const IndexBuffer &mIndexBuffer;
        vk::DeviceSize mOffset;

    public:
        IndexBufferInfo(const IndexBuffer &indexBuffer, vk::DeviceSize offset = 0);

        IndexBufferInfo(const IndexBufferInfo &other);

        IndexBufferInfo(IndexBufferInfo &&other);

        ~IndexBufferInfo();

        const IndexBuffer &getIndexBuffer() const;

        vk::DeviceSize getOffset() const;
    };

} // vklite
