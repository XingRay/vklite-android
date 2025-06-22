//
// Created by leixing on 2025-06-22.
//

#pragma once

#include <vector>
#include <functional>

#include "vklite/sync/pipeline_barrier/PipelineBarrier.h"

#include "vklite/sync/pipeline_barrier/memory_barrier/MemoryBarrierBuilder.h"
#include "vklite/sync/pipeline_barrier/buffer_memory_barrier/BufferMemoryBarrierBuilder.h"
#include "vklite/sync/pipeline_barrier/image_memory_barrier/ImageMemoryBarrierBuilder.h"

namespace vklite {

    class PipelineBarrierBuilder {
    private:
        vk::PipelineStageFlags mSrcStage;
        vk::PipelineStageFlags mDstStage;
        vk::DependencyFlags mDependency;

        std::vector<vk::MemoryBarrier> mMemoryBarriers;
        std::vector<vk::BufferMemoryBarrier> mBufferMemoryBarriers;
        std::vector<vk::ImageMemoryBarrier> mImageMemoryBarriers;

    public:
        PipelineBarrierBuilder();

        ~PipelineBarrierBuilder();

        PipelineBarrierBuilder &srcStage(vk::PipelineStageFlags srcStage);

        PipelineBarrierBuilder &dstStage(vk::PipelineStageFlags dstStage);

        PipelineBarrierBuilder &dependency(vk::DependencyFlags dependency);


        PipelineBarrierBuilder &addMemoryBarrier(const vk::MemoryBarrier &barrier);

        PipelineBarrierBuilder &addMemoryBarrier(const std::function<void(MemoryBarrierBuilder &builder)> &builderConfig);


        PipelineBarrierBuilder &addBufferMemoryBarrier(const vk::BufferMemoryBarrier &barrier);

        PipelineBarrierBuilder &addBufferMemoryBarrier(const std::function<void(BufferMemoryBarrierBuilder &builder)> &builderConfig);


        PipelineBarrierBuilder &addImageMemoryBarrier(const vk::ImageMemoryBarrier &barrier);

        PipelineBarrierBuilder &addImageMemoryBarrier(const std::function<void(ImageMemoryBarrierBuilder &builder)> &builderConfig);


        PipelineBarrier build();

        // presets
        PipelineBarrierBuilder& asDefault();
    };

} // vklite
