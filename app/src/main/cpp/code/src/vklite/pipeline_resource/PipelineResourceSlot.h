//
// Created by leixing on 2025/4/18.
//

#pragma once

#include <cstdint>
#include "vulkan/vulkan.hpp"

#include "BufferInfo.h"
#include "ImageInfo.h"

namespace vklite {

    class PipelineResourceSlot {
    private:
        // buffer info
        std::unique_ptr<BufferInfo> mDescriptorBufferInfo;

        // image info
        std::unique_ptr<ImageInfo> mDescriptorImageInfo;

        // immutable sampler
        std::unique_ptr<ImageInfo> mImmutableSamplerInfo;

    public:
        PipelineResourceSlot();

        PipelineResourceSlot(PipelineResourceSlot &&other) noexcept;

        ~PipelineResourceSlot();
    };

} // vklite
