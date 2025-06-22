//
// Created by leixing on 2025-06-22.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    // todo: 待完成
    class MemoryBarrierBuilder {
    private:
        vk::MemoryBarrier mMemoryBarrier;

    public:
        MemoryBarrierBuilder();

        ~MemoryBarrierBuilder();

        [[nodiscard]]
        vk::MemoryBarrier build() const;
    };

} // vklite
