//
// Created by leixing on 2025/5/30.
//

#pragma once

#include <vulkan/vulkan.h>

#include "vklite/image/Image.h"
#include "vklite/device_memory/DeviceMemory.h"

namespace vklite {

    class CombinedMemoryImage {
    private:
        Image mImage;
        DeviceMemory mDeviceMemory;

    public:
        explicit CombinedMemoryImage(Image &&image, DeviceMemory &&deviceMemory);

        ~CombinedMemoryImage();

        CombinedMemoryImage(const CombinedMemoryImage &other) = delete;

        CombinedMemoryImage &operator=(const CombinedMemoryImage &other) = delete;

        CombinedMemoryImage(CombinedMemoryImage &&other) noexcept;

        CombinedMemoryImage &operator=(CombinedMemoryImage &&other) noexcept;

        [[nodiscard]]
        Image &getImage();

        [[nodiscard]]
        DeviceMemory &getDeviceMemory();
    };

} // vklite
