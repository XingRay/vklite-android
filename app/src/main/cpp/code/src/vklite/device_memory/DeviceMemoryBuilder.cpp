//
// Created by leixing on 2025/5/30.
//

#include "DeviceMemoryBuilder.h"

#include "vklite/util/VulkanUtil.h"
#include "vklite/device_memory/DeviceMemoryMeta.h"

namespace vklite {

    DeviceMemoryBuilder &DeviceMemoryBuilder::device(const vk::Device &device) {
        mDevice = device;
        return *this;
    }

    DeviceMemoryBuilder &DeviceMemoryBuilder::allocationSize(vk::DeviceSize allocationSize) {
        mMemoryAllocateInfo.allocationSize = allocationSize;
        return *this;
    }

    DeviceMemoryBuilder &DeviceMemoryBuilder::memoryTypeIndex(uint32_t memoryTypeIndex) {
        mMemoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;
        return *this;
    }

    DeviceMemoryBuilder &DeviceMemoryBuilder::next(const void *next) {
        mMemoryAllocateInfo.setPNext(next);
        return *this;
    }

    DeviceMemory DeviceMemoryBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("DeviceMemoryBuilder::build(): mDevice == nullptr");
        }
        vk::DeviceMemory deviceMemory = mDevice.allocateMemory(mMemoryAllocateInfo);
        DeviceMemoryMeta meta(mMemoryAllocateInfo.allocationSize, mMemoryAllocateInfo.memoryTypeIndex);
        return DeviceMemory{mDevice, deviceMemory, std::move(meta)};
    }

    std::unique_ptr<DeviceMemory> DeviceMemoryBuilder::buildUnique() {
        return std::make_unique<DeviceMemory>(build());
    }

} // vklite