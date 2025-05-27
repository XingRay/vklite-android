//
// Created by leixing on 2024/12/31.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"
#include "vklite/sync/Fence.h"
#include "vklite/sync/Semaphore.h"
#include "vklite/Log.h"

namespace vklite {

    class SyncObject {
    private:
        //vk::Semaphore 主要用于 GPU 之间的同步
        std::vector<Semaphore> mImageAvailableSemaphores;
        std::vector<Semaphore> mRenderFinishedSemaphores;

        // vk::Fence 主要用于 CPU 和 GPU 之间的同步，例如：
        // 等待 GPU 完成某个操作, 确保 CPU 不会过早地访问 GPU 资源。
        std::vector<Fence> mFences;

    public:
        explicit SyncObject(const Device &device, uint32_t count);

        ~SyncObject();

        [[nodiscard]]
        const vk::Semaphore &getImageAvailableSemaphore(uint32_t index) const;

        [[nodiscard]]
        const vk::Semaphore &getRenderFinishedSemaphore(uint32_t index) const;

        [[nodiscard]]
        vk::Fence getFence(uint32_t index) const;

        vk::Result resetFence(uint32_t index);

        vk::Result waitFence(uint32_t index);
    };

} // vklite
