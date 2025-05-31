//
// Created by leixing on 2025/3/22.
//

#pragma once

#include <vector>

#include "vklite/buffer/index_buffer/IndexBuffer.h"
#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBufferBuilder.h"

namespace vklite {

    class IndexBufferBuilder {
    private:
        vk::Device mDevice;
        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;
        vk::IndexType mIndexType;

    public:
        IndexBufferBuilder();

        ~IndexBufferBuilder();

        IndexBufferBuilder &device(vk::Device device);

        IndexBufferBuilder &size(vk::DeviceSize size);

        IndexBufferBuilder &indexType(vk::IndexType indexType);

        [[nodiscard]]
        IndexBuffer build();

        [[nodiscard]]
        std::unique_ptr<IndexBuffer> buildUnique();
    };

} // vklite
