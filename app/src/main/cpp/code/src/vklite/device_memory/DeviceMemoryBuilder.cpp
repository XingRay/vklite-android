//
// Created by leixing on 2025/5/30.
//

#include "DeviceMemoryBuilder.h"

#include "vklite/util/VulkanUtil.h"

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

    DeviceMemoryBuilder &DeviceMemoryBuilder::config(vk::PhysicalDeviceMemoryProperties memoryProperties,
                                                     vk::MemoryRequirements memoryRequirements,
                                                     vk::MemoryPropertyFlags memoryPropertyFlags) {
        vk::MemoryAllocateInfo memoryAllocateInfo = VulkanUtil::createMemoryAllocateInfo(memoryProperties, memoryRequirements, memoryPropertyFlags);

        allocationSize(memoryAllocateInfo.allocationSize);
        memoryTypeIndex(memoryAllocateInfo.memoryTypeIndex);
        return *this;
    }

    DeviceMemoryBuilder &DeviceMemoryBuilder::config(vk::PhysicalDeviceMemoryProperties memoryProperties,
                                                     vk::Image image,
                                                     vk::MemoryPropertyFlags memoryPropertyFlags) {
        config(memoryProperties, mDevice.getImageMemoryRequirements(image), memoryPropertyFlags);
        return *this;
    }

    DeviceMemoryBuilder &DeviceMemoryBuilder::config(vk::PhysicalDevice physicalDevice, vk::Image image, vk::MemoryPropertyFlags memoryPropertyFlags) {
        config(physicalDevice.getMemoryProperties(), mDevice.getImageMemoryRequirements(image), memoryPropertyFlags);
        return *this;
    }

    DeviceMemory DeviceMemoryBuilder::build() {
        vk::DeviceMemory deviceMemory = mDevice.allocateMemory(mMemoryAllocateInfo);
//        return DeviceMemory(mDevice, deviceMemory);
        return {mDevice, deviceMemory};
    }

    std::unique_ptr<DeviceMemory> DeviceMemoryBuilder::buildUnique() {
        return std::make_unique<DeviceMemory>(build());
    }

} // vklite