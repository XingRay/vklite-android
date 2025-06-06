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

    DeviceMemoryBuilder &DeviceMemoryBuilder::config(vk::PhysicalDeviceMemoryProperties memoryProperties,
                                                     vk::MemoryRequirements memoryRequirements,
                                                     vk::MemoryPropertyFlags memoryPropertyFlags) {
        allocationSize(memoryRequirements.size);
        memoryTypeIndex(VulkanUtil::findMemoryTypeIndex(memoryProperties, memoryRequirements, memoryPropertyFlags));
        return *this;
    }

    DeviceMemoryBuilder &DeviceMemoryBuilder::config(vk::PhysicalDeviceMemoryProperties memoryProperties,
                                                     vk::Image image,
                                                     vk::MemoryPropertyFlags memoryPropertyFlags) {
        config(memoryProperties, mDevice.getImageMemoryRequirements(image), memoryPropertyFlags);
        return *this;
    }

    DeviceMemoryBuilder &DeviceMemoryBuilder::config(vk::PhysicalDeviceMemoryProperties memoryProperties,
                                                     vk::Buffer buffer,
                                                     vk::MemoryPropertyFlags memoryPropertyFlags) {
        config(memoryProperties, mDevice.getBufferMemoryRequirements(buffer), memoryPropertyFlags);
        return *this;
    }

    DeviceMemoryBuilder &DeviceMemoryBuilder::config(vk::PhysicalDevice physicalDevice,
                                                     vk::Image image,
                                                     vk::MemoryPropertyFlags memoryPropertyFlags) {
        config(physicalDevice.getMemoryProperties(), mDevice.getImageMemoryRequirements(image), memoryPropertyFlags);
        return *this;
    }

    DeviceMemoryBuilder &DeviceMemoryBuilder::config(vk::PhysicalDevice physicalDevice,
                                                     vk::Buffer buffer,
                                                     vk::MemoryPropertyFlags memoryPropertyFlags) {
        config(physicalDevice.getMemoryProperties(), mDevice.getBufferMemoryRequirements(buffer), memoryPropertyFlags);
        return *this;
    }

    DeviceMemory DeviceMemoryBuilder::build() {
        vk::DeviceMemory deviceMemory = mDevice.allocateMemory(mMemoryAllocateInfo);
        DeviceMemoryMeta meta(mMemoryAllocateInfo.allocationSize, mMemoryAllocateInfo.memoryTypeIndex);
//        return DeviceMemory(mDevice, deviceMemory, std::move(meta));
        return {mDevice, deviceMemory, std::move(meta)};
    }

    std::unique_ptr<DeviceMemory> DeviceMemoryBuilder::buildUnique() {
        return std::make_unique<DeviceMemory>(build());
    }

} // vklite