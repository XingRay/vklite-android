//
// Created by leixing on 2025/5/20.
//

#include "PipelineBarrier.h"

namespace vklite {

    PipelineBarrier::PipelineBarrier(vk::PipelineStageFlags srcStage,
                                     vk::PipelineStageFlags dstStage,
                                     vk::DependencyFlags dependency,
                                     std::vector<vk::MemoryBarrier> &&memoryBarriers,
                                     std::vector<vk::BufferMemoryBarrier> &&bufferMemoryBarriers,
                                     std::vector<vk::ImageMemoryBarrier> &&imageMemoryBarriers)
            : mSrcStage(srcStage),
              mDstStage(dstStage),
              mDependency(dependency),
              mMemoryBarriers(std::move(memoryBarriers)),
              mBufferMemoryBarriers(std::move(bufferMemoryBarriers)),
              mImageMemoryBarriers(std::move(imageMemoryBarriers)) {}

    PipelineBarrier::~PipelineBarrier() = default;

    PipelineBarrier::PipelineBarrier(const PipelineBarrier &other) = default;

    PipelineBarrier &PipelineBarrier::operator=(const PipelineBarrier &other) {
        if (this != &other) {
            mSrcStage = other.mSrcStage;
            mDstStage = other.mDstStage;
            mDependency = other.mDependency;
            mMemoryBarriers = other.mMemoryBarriers;
            mBufferMemoryBarriers = other.mBufferMemoryBarriers;
            mImageMemoryBarriers = other.mImageMemoryBarriers;
        }
        return *this;
    }

    PipelineBarrier::PipelineBarrier(PipelineBarrier &&other) noexcept
            : mSrcStage(other.mSrcStage),
              mDstStage(other.mDstStage),
              mDependency(other.mDependency),
              mMemoryBarriers(std::move(other.mMemoryBarriers)),
              mBufferMemoryBarriers(std::move(other.mBufferMemoryBarriers)),
              mImageMemoryBarriers(std::move(other.mImageMemoryBarriers)) {}

    PipelineBarrier &PipelineBarrier::operator=(PipelineBarrier &&other) noexcept {
        if (this != &other) {
            mSrcStage = other.mSrcStage;
            mDstStage = other.mDstStage;
            mDependency = other.mDependency;
            mMemoryBarriers = std::move(other.mMemoryBarriers);
            mBufferMemoryBarriers = std::move(other.mBufferMemoryBarriers);
            mImageMemoryBarriers = std::move(other.mImageMemoryBarriers);
        }
        return *this;
    }

    void PipelineBarrier::record(const vk::CommandBuffer &commandBuffer) const {
        commandBuffer.pipelineBarrier(
                mSrcStage,
                mDstStage,
                mDependency,
                mMemoryBarriers,
                mBufferMemoryBarriers,
                mImageMemoryBarriers
        );
    }

    void PipelineBarrier::exec(const CommandPool &commandPool) const {
        commandPool.submit([&](const vk::CommandBuffer &commandBuffer) {
            record(commandBuffer);
        });
    }

} // vklite