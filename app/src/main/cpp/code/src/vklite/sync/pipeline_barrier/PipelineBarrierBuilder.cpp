//
// Created by leixing on 2025-06-22.
//

#include "PipelineBarrierBuilder.h"

namespace vklite {

    PipelineBarrierBuilder::PipelineBarrierBuilder()
            : mSrcStage(vk::PipelineStageFlagBits::eNone),
              mDstStage(vk::PipelineStageFlagBits::eNone),
              mDependency(vk::DependencyFlags{}) {}

    PipelineBarrierBuilder::~PipelineBarrierBuilder() = default;

    PipelineBarrierBuilder &PipelineBarrierBuilder::srcStage(vk::PipelineStageFlags srcStage) {
        mSrcStage = srcStage;
        return *this;
    }

    PipelineBarrierBuilder &PipelineBarrierBuilder::dstStage(vk::PipelineStageFlags dstStage) {
        mDstStage = dstStage;
        return *this;
    }

    PipelineBarrierBuilder &PipelineBarrierBuilder::dependency(vk::DependencyFlags dependency) {
        mDependency = dependency;
        return *this;
    }

    PipelineBarrierBuilder &PipelineBarrierBuilder::addMemoryBarrier(const vk::MemoryBarrier &barrier) {
        mMemoryBarriers.push_back(barrier);
        return *this;
    }

    PipelineBarrierBuilder &PipelineBarrierBuilder::addMemoryBarrier(const std::function<void(MemoryBarrierBuilder &builder)> &builderConfig) {
        MemoryBarrierBuilder builder;
        builderConfig(builder);
        addMemoryBarrier(builder.build());
        return *this;
    }


    PipelineBarrierBuilder &PipelineBarrierBuilder::addBufferMemoryBarrier(const vk::BufferMemoryBarrier &barrier) {
        mBufferMemoryBarriers.push_back(barrier);
        return *this;
    }

    PipelineBarrierBuilder &PipelineBarrierBuilder::addBufferMemoryBarrier(const std::function<void(BufferMemoryBarrierBuilder &builder)> &builderConfig) {
        BufferMemoryBarrierBuilder builder;
        builderConfig(builder);
        addBufferMemoryBarrier(builder.build());
        return *this;
    }


    PipelineBarrierBuilder &PipelineBarrierBuilder::addImageMemoryBarrier(const vk::ImageMemoryBarrier &barrier) {
        mImageMemoryBarriers.push_back(barrier);
        return *this;
    }

    PipelineBarrierBuilder &PipelineBarrierBuilder::addImageMemoryBarrier(const std::function<void(ImageMemoryBarrierBuilder &builder)> &builderConfig) {
        ImageMemoryBarrierBuilder builder;
        builderConfig(builder);
        addImageMemoryBarrier(builder.build());
        return *this;
    }

    PipelineBarrier PipelineBarrierBuilder::build() {
        return PipelineBarrier{
                mSrcStage,
                mDstStage,
                mDependency,
                std::move(mMemoryBarriers),
                std::move(mBufferMemoryBarriers),
                std::move(mImageMemoryBarriers),
        };
    }

    PipelineBarrierBuilder &PipelineBarrierBuilder::asDefault() {
        (*this)
                .dependency(vk::DependencyFlags{});

        return *this;
    }

} // vklite