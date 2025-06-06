//
// Created by leixing on 2025/5/30.
//

#pragma once

#include <functional>
#include <memory>
#include <optional>

#include "CombinedMemoryImage.h"
#include "vklite/image/ImageBuilder.h"
#include "vklite/device_memory/DeviceMemoryBuilder.h"

namespace vklite {

    class CombinedMemoryImageBuilder {
    private:
        ImageBuilder mImageBuilder;

        DeviceMemoryBuilder mDeviceMemoryBuilder;

        std::optional<vk::PhysicalDeviceMemoryProperties> mPhysicalDeviceMemoryProperties;
        vk::MemoryPropertyFlags mMemoryPropertyFlags = vk::MemoryPropertyFlagBits::eDeviceLocal;

        vk::DeviceSize mMemoryOffset;

        std::function<void(ImageBuilder &builder)> mImageBuilderConfigure;
        std::function<void(Image &image, DeviceMemoryBuilder &builder)> mDeviceMemoryBuilderConfigure;

    public:

        CombinedMemoryImageBuilder();

        ~CombinedMemoryImageBuilder();

        CombinedMemoryImageBuilder &device(vk::Device device);

        CombinedMemoryImageBuilder &memoryOffset(vk::DeviceSize memoryOffset);

        CombinedMemoryImageBuilder &imageBuilder(std::function<void(ImageBuilder &builder)> &&configure);

        CombinedMemoryImageBuilder &deviceMemoryBuilder(std::function<void(Image &image, DeviceMemoryBuilder &builder)> &&configure);

        CombinedMemoryImage build();

        std::unique_ptr<CombinedMemoryImage> buildUnique();
    };

} // vklite
