//
// Created by leixing on 2025/5/28.
//

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "Fence.h"

namespace vklite {

    class FenceBuilder {
    private:
        vk::Device mDevice;
        vk::FenceCreateFlags mFenceCreateFlags;

    public:
        FenceBuilder();

        ~FenceBuilder();

        FenceBuilder &device(vk::Device device);

        FenceBuilder &fenceCreateFlags(vk::FenceCreateFlags fenceCreateFlags);

        Fence build();

        std::unique_ptr<Fence> buildUnique();

        std::vector<Fence> build(uint32_t count);
    };

} // vklite
