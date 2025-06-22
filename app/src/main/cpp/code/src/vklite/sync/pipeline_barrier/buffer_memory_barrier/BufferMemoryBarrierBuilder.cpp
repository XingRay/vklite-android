//
// Created by leixing on 2025-06-22.
//

#include "BufferMemoryBarrierBuilder.h"

namespace vklite {

    BufferMemoryBarrierBuilder::BufferMemoryBarrierBuilder()
            : mBufferMemoryBarrier{} {}

    BufferMemoryBarrierBuilder::~BufferMemoryBarrierBuilder() = default;

    vk::BufferMemoryBarrier BufferMemoryBarrierBuilder::build() const {
        return mBufferMemoryBarrier;
    }

} // vklite