//
// Created by leixing on 2025/5/20.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class PipelineBarrier {
    private:
        vk::PipelineStageFlags mSrcStage;
        vk::PipelineStageFlags mDstStage;
        vk::DependencyFlags mDependency;
        std::vector<vk::MemoryBarrier> mMemoryBarriers;
        std::vector<vk::BufferMemoryBarrier> mBufferMemoryBarriers;
        std::vector<vk::ImageMemoryBarrier> mImageMemoryBarriers;

    public:
        explicit PipelineBarrier(vk::PipelineStageFlags srcStage,
                                 vk::PipelineStageFlags dstStage,
                                 vk::DependencyFlags dependency,
                                 std::vector<vk::MemoryBarrier> &&memoryBarriers,
                                 std::vector<vk::BufferMemoryBarrier> &&bufferMemoryBarriers,
                                 std::vector<vk::ImageMemoryBarrier> &&imageMemoryBarriers);

        ~PipelineBarrier();

        PipelineBarrier(const PipelineBarrier &other);

        PipelineBarrier &operator=(const PipelineBarrier &other);

        PipelineBarrier(PipelineBarrier &&other) noexcept;

        PipelineBarrier &operator=(PipelineBarrier &&other) noexcept;


        void record(const vk::CommandBuffer &commandBuffer) const;

        void exec(const CommandPool &commandPool) const;

    };

} // vklite