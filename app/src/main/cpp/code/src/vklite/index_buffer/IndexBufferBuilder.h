//
// Created by leixing on 2025/3/22.
//

#pragma once

#include <vector>

#include "IndexBuffer.h"

namespace vklite {

    class IndexBufferBuilder {
    private:
        vk::DeviceSize mBufferSize;

    public:
        IndexBufferBuilder();

        ~IndexBufferBuilder();

        IndexBufferBuilder &bufferSize(vk::DeviceSize bufferSize);

        [[nodiscard]]
        std::unique_ptr<IndexBuffer> build(const Device &device) const;
    };

} // vklite
