//
// Created by leixing on 2024/12/31.
//

#include "SyncObject.h"
#include "vklite/Log.h"

namespace vklite {
    SyncObject::SyncObject(const Device &device, uint32_t count) : mDevice(device) {
        vk::Device vkDevice = device.getDevice();

        vk::SemaphoreCreateInfo semaphoreCreateInfo{};
        vk::FenceCreateInfo fenceCreateInfo{};
        // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
        fenceCreateInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

        for (int i = 0; i < count; i++) {
            mImageAvailableSemaphores.push_back(vkDevice.createSemaphore(semaphoreCreateInfo));
            mRenderFinishedSemaphores.push_back(vkDevice.createSemaphore(semaphoreCreateInfo));
            mFences.push_back(vkDevice.createFence(fenceCreateInfo));
        }
    }

    SyncObject::~SyncObject() {
        LOG_D("VulkanSyncObject::~VulkanSyncObject");
        const vk::Device& vkDevice = mDevice.getDevice();
        for (const auto &semaphore: mImageAvailableSemaphores) {
            vkDevice.destroy(semaphore);
        }
        for (const auto &semaphore: mRenderFinishedSemaphores) {
            vkDevice.destroy(semaphore);
        }
        for (const auto &fence: mFences) {
            vkDevice.destroy(fence);
        }
    }

    const vk::Semaphore &SyncObject::getImageAvailableSemaphore(uint32_t index) const {
        return mImageAvailableSemaphores[index];
    }

    const vk::Semaphore &SyncObject::getRenderFinishedSemaphore(uint32_t index) const {
        return mRenderFinishedSemaphores[index];
    }

    vk::Fence SyncObject::getFence(uint32_t index) const {
        return mFences[index];
    }

    vk::Result SyncObject::resetFence(uint32_t index) {
        const vk::Device& vkDevice = mDevice.getDevice();
        return vkDevice.resetFences(1, &mFences[index]);
    }

    vk::Result SyncObject::waitFence(uint32_t index) {
        const vk::Device& vkDevice = mDevice.getDevice();
        return vkDevice.waitForFences(1, &mFences[index], vk::True, std::numeric_limits<uint64_t>::max());
    }
} // vklite