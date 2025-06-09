//
// Created by leixing on 2025/6/8.
//

#pragma once

#include "vklite/platform/android/image/HardwareBufferImage.h"
#include "vklite/device_memory/DeviceMemory.h"
#include "vklite/image_view/ImageView.h"

namespace vklite {

    class CombinedHardwareBufferImageView {
    private:
        HardwareBufferImage mImage;
        DeviceMemory mDeviceMemory;
        ImageView mImageView;

    public:
        explicit CombinedHardwareBufferImageView(HardwareBufferImage &&image, DeviceMemory &&deviceMemory, ImageView &&imageView);

        ~CombinedHardwareBufferImageView();

        CombinedHardwareBufferImageView(const CombinedHardwareBufferImageView &other) = delete;

        CombinedHardwareBufferImageView &operator=(const CombinedHardwareBufferImageView &other) = delete;

        CombinedHardwareBufferImageView(CombinedHardwareBufferImageView &&other) noexcept;

        CombinedHardwareBufferImageView &operator=(CombinedHardwareBufferImageView &&other) noexcept;

        const ImageView &getImageView() const;
    };

} // vklite
