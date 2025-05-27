//
// Created by leixing on 2024/12/31.
//

#include "SyncObject.h"
#include "vklite/Log.h"

namespace vklite {
    SyncObject::SyncObject(const Device &device, uint32_t count) {
        vk::Device vkDevice = device.getDevice();

        vk::SemaphoreCreateInfo semaphoreCreateInfo{};
        vk::FenceCreateInfo fenceCreateInfo{};
        // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
        fenceCreateInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

        for (int i = 0; i < count; i++) {
            mImageAvailableSemaphores.push_back(Semaphore(vkDevice, vkDevice.createSemaphore(semaphoreCreateInfo)));
            mRenderFinishedSemaphores.push_back(Semaphore(vkDevice, vkDevice.createSemaphore(semaphoreCreateInfo)));
            mFences.push_back(Fence(vkDevice, vkDevice.createFence(fenceCreateInfo)));
        }
    }

    SyncObject::~SyncObject() = default;

    const vk::Semaphore &SyncObject::getImageAvailableSemaphore(uint32_t index) const {
        return mImageAvailableSemaphores[index].getSemaphore();
    }

    const vk::Semaphore &SyncObject::getRenderFinishedSemaphore(uint32_t index) const {
        return mRenderFinishedSemaphores[index].getSemaphore();
    }

    vk::Fence SyncObject::getFence(uint32_t index) const {
        return mFences[index].getFence();
    }

    vk::Result SyncObject::resetFence(uint32_t index) {
        return mFences[index].reset();
    }

    vk::Result SyncObject::waitFence(uint32_t index) {
        return mFences[index].wait();
    }
} // vklite