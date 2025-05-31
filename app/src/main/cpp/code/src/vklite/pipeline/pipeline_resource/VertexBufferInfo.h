//
// Created by leixing on 2025/5/16.
//

#pragma once

#include "vklite/buffer/vertex_buffer/VertexBuffer.h"

namespace vklite {

    class VertexBufferInfo {
    private:
        const VertexBuffer &mVertexBuffer;
        vk::DeviceSize mOffset;

    public:
        explicit VertexBufferInfo(const VertexBuffer &vertexBuffer, vk::DeviceSize offset = 0);

        VertexBufferInfo(const VertexBufferInfo &other);

        VertexBufferInfo(VertexBufferInfo &&other) noexcept;

        VertexBufferInfo &operator=(const VertexBufferInfo &other) = delete;

        VertexBufferInfo &operator=(VertexBufferInfo &&other) noexcept = delete;

        ~VertexBufferInfo();

        [[nodiscard]]
        const VertexBuffer &getVertexBuffer() const;

        [[nodiscard]]
        vk::DeviceSize getOffset() const;

    };

} // vklite
