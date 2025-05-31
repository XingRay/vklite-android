//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "vklite/buffer/uniform_buffer/UniformBuffer.h"
#include "vklite/buffer/combined_memory_buffer/CombinedMemoryBufferBuilder.h"

namespace vklite {

    class UniformBufferBuilder {
    private:
        CombinedMemoryBufferBuilder mCombinedMemoryBufferBuilder;

    public:

        UniformBufferBuilder();

        ~UniformBufferBuilder();

        UniformBuffer build();

        std::unique_ptr<UniformBuffer> buildUnique();
    };

} // vklite
