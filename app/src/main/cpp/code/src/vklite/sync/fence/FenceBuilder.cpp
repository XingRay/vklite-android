//
// Created by leixing on 2025/5/28.
//

#include "FenceBuilder.h"

namespace vklite {

    FenceBuilder::FenceBuilder()
    // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
            : mFenceCreateFlags(vk::FenceCreateFlagBits::eSignaled) {}

    FenceBuilder::~FenceBuilder() = default;

    FenceBuilder &FenceBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    FenceBuilder &FenceBuilder::fenceCreateFlags(vk::FenceCreateFlags fenceCreateFlags) {
        mFenceCreateFlags = fenceCreateFlags;
        return *this;
    }

    Fence FenceBuilder::build() {
        vk::FenceCreateInfo fenceCreateInfo{};
        fenceCreateInfo.setFlags(mFenceCreateFlags);

        return Fence{mDevice, mDevice.createFence(fenceCreateInfo)};
    }

    std::unique_ptr<Fence> FenceBuilder::buildUnique() {
        return std::make_unique<Fence>(build());
    }

    std::vector<Fence> FenceBuilder::build(uint32_t count) {
        vk::FenceCreateInfo fenceCreateInfo{};
        fenceCreateInfo.setFlags(mFenceCreateFlags);

        std::vector<Fence> fences;
        fences.reserve(count);
        for (uint32_t i = 0; i < count; i++) {
            fences.emplace_back(mDevice, mDevice.createFence(fenceCreateInfo));
        }
        return fences;
    }

} // vklite