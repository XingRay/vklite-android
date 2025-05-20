//
// Created by leixing on 2025/5/20.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class PipelineBarrier {
    private:
        vk::PipelineStageFlags mSrcStageMask;
        vk::PipelineStageFlags mDstStageMask;
        vk::DependencyFlags mDependencyFlags;
        std::vector<vk::MemoryBarrier> mMemoryBarriers;
        std::vector<vk::BufferMemoryBarrier> mBufferMemoryBarriers;
        std::vector<vk::ImageMemoryBarrier> mImageMemoryBarriers;

    public:
        PipelineBarrier(vk::PipelineStageFlags srcStageMask,
                        vk::PipelineStageFlags dstStageMask,
                        vk::DependencyFlags dependencyFlags,
                        std::vector<vk::MemoryBarrier> memoryBarriers,
                        std::vector<vk::BufferMemoryBarrier> bufferMemoryBarriers,
                        std::vector<vk::ImageMemoryBarrier> imageMemoryBarriers);

        PipelineBarrier();

        ~PipelineBarrier();

        PipelineBarrier(const PipelineBarrier &other) = delete;

        PipelineBarrier &operator=(const PipelineBarrier &other) = delete;

        PipelineBarrier(PipelineBarrier &&other) noexcept;

        PipelineBarrier &operator=(PipelineBarrier &&other) noexcept;

        PipelineBarrier &srcStageMask(vk::PipelineStageFlags srcStageMask);

        PipelineBarrier &dstStageMask(vk::PipelineStageFlags dstStageMask);

        PipelineBarrier &dependencyFlags(vk::DependencyFlags dependencyFlags);

        PipelineBarrier &addMemoryBarrier(const vk::MemoryBarrier &memoryBarrier);

        PipelineBarrier &addBufferMemoryBarrier(const vk::BufferMemoryBarrier &bufferMemoryBarrier);

        PipelineBarrier &addImageMemoryBarrier(const vk::ImageMemoryBarrier &imageMemoryBarriers);

        void record(const vk::CommandBuffer &commandBuffer);
    };

} // vklite