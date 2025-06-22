//
// Created by leixing on 2025-06-22.
//

#include "MemoryBarrierBuilder.h"

namespace vklite {

    MemoryBarrierBuilder::MemoryBarrierBuilder()
            : mMemoryBarrier{} {}

    MemoryBarrierBuilder::~MemoryBarrierBuilder() = default;

    vk::MemoryBarrier MemoryBarrierBuilder::build() const {
        return mMemoryBarrier;
    }

} // vklite