//
// Created by leixing on 2025/6/8.
//

#pragma once

#include "vklite/image/Image.h"
#include "vklite/device_memory/DeviceMemory.h"
#include "vklite/image_view/ImageView.h"

namespace vklite {

    class CombinedHardwareBufferImageView {
    private:
        Image mImage;
        DeviceMemory mDeviceMemory;
        ImageView mImageView;

    public:
        explicit CombinedHardwareBufferImageView(Image &&image, DeviceMemory &&deviceMemory, ImageView &&imageView);

        ~CombinedHardwareBufferImageView();

        CombinedHardwareBufferImageView(const CombinedHardwareBufferImageView &other);

        CombinedHardwareBufferImageView &operator=(const CombinedHardwareBufferImageView &other);

        CombinedHardwareBufferImageView(CombinedHardwareBufferImageView &&other) noexcept;

        CombinedHardwareBufferImageView &operator=(CombinedHardwareBufferImageView &&other) noexcept;

        const ImageView &getImageView() const;
    };

} // vklite
