//
// Created by leixing on 2025-06-22.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    // todo: 待完成
    class BufferMemoryBarrierBuilder {
    private:
        vk::BufferMemoryBarrier mBufferMemoryBarrier;

    public:
        BufferMemoryBarrierBuilder();

        ~BufferMemoryBarrierBuilder();

        [[nodiscard]]
        vk::BufferMemoryBarrier build() const;
    };

} // vklite
