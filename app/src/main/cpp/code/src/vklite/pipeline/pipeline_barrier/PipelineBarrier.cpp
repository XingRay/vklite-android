//
// Created by leixing on 2025/5/20.
//

#include "PipelineBarrier.h"

namespace vklite {

    PipelineBarrier::PipelineBarrier(vk::PipelineStageFlags srcStageMask,
                                     vk::PipelineStageFlags dstStageMask,
                                     vk::DependencyFlags dependencyFlags,
                                     std::vector<vk::MemoryBarrier> memoryBarriers,
                                     std::vector<vk::BufferMemoryBarrier> bufferMemoryBarriers,
                                     std::vector<vk::ImageMemoryBarrier> imageMemoryBarriers)
            : mSrcStageMask(srcStageMask),
              mDstStageMask(dstStageMask),
              mDependencyFlags(dependencyFlags),
              mMemoryBarriers(std::move(memoryBarriers)),
              mBufferMemoryBarriers(std::move(bufferMemoryBarriers)),
              mImageMemoryBarriers(std::move(imageMemoryBarriers)) {}

    PipelineBarrier::PipelineBarrier()
            : PipelineBarrier(vk::PipelineStageFlagBits::eNone, vk::PipelineStageFlagBits::eNone, vk::DependencyFlagBits{}, {}, {}, {}) {}

    PipelineBarrier::~PipelineBarrier() = default;

    PipelineBarrier::PipelineBarrier(PipelineBarrier &&other) noexcept
            : mSrcStageMask(other.mSrcStageMask),
              mDstStageMask(other.mDstStageMask),
              mDependencyFlags(other.mDependencyFlags),
              mMemoryBarriers(std::move(other.mMemoryBarriers)),
              mBufferMemoryBarriers(std::move(other.mBufferMemoryBarriers)),
              mImageMemoryBarriers(std::move(other.mImageMemoryBarriers)) {}

    PipelineBarrier &PipelineBarrier::operator=(PipelineBarrier &&other) noexcept {
        if (this != &other) {
            mSrcStageMask = other.mSrcStageMask;
            mDstStageMask = other.mDstStageMask;
            mDependencyFlags = other.mDependencyFlags;
            mMemoryBarriers = std::move(other.mMemoryBarriers);
            mBufferMemoryBarriers = std::move(other.mBufferMemoryBarriers);
            mImageMemoryBarriers = std::move(other.mImageMemoryBarriers);
        }
        return *this;
    }

    PipelineBarrier &PipelineBarrier::srcStageMask(vk::PipelineStageFlags srcStageMask) {
        mSrcStageMask = srcStageMask;
        return *this;
    }

    PipelineBarrier &PipelineBarrier::dstStageMask(vk::PipelineStageFlags dstStageMask) {
        mDstStageMask = dstStageMask;
        return *this;
    }

    PipelineBarrier &PipelineBarrier::dependencyFlags(vk::DependencyFlags dependencyFlags) {
        mDependencyFlags = dependencyFlags;
        return *this;
    }

    PipelineBarrier &PipelineBarrier::addMemoryBarrier(const vk::MemoryBarrier &memoryBarrier) {
        mMemoryBarriers.push_back(memoryBarrier);
        return *this;
    }

    PipelineBarrier &PipelineBarrier::addBufferMemoryBarrier(const vk::BufferMemoryBarrier &bufferMemoryBarrier) {
        mBufferMemoryBarriers.push_back(bufferMemoryBarrier);
        return *this;
    }

    PipelineBarrier &PipelineBarrier::addImageMemoryBarrier(const vk::ImageMemoryBarrier &imageMemoryBarriers) {
        mImageMemoryBarriers.push_back(imageMemoryBarriers);
        return *this;
    }

    void PipelineBarrier::record(const vk::CommandBuffer &commandBuffer) {
        commandBuffer.pipelineBarrier(
                mSrcStageMask,
                mDstStageMask,
                mDependencyFlags,
                mMemoryBarriers,
                mBufferMemoryBarriers,
                mImageMemoryBarriers
        );
    }

} // vklite