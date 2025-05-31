//
// Created by leixing on 2025/6/1.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class IndexBufferMeta {
    private:
        vk::IndexType mIndexType;
        uint32_t mIndicesCount;

    public:
        IndexBufferMeta(vk::IndexType indexType, uint32_t indicesCount);

        ~IndexBufferMeta();

        IndexBufferMeta(const IndexBufferMeta &other);

        IndexBufferMeta &operator=(const IndexBufferMeta &other);

        IndexBufferMeta(IndexBufferMeta &&other) noexcept;

        IndexBufferMeta &operator=(IndexBufferMeta &&other) noexcept;

        IndexBufferMeta &indicesCount(uint32_t indicesCount);

        [[nodiscard]]
        vk::IndexType getIndexType() const;

        [[nodiscard]]
        uint32_t getIndicesCount() const;

    };

} // vklite
