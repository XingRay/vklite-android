//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "vklite/device/Device.h"
#include "ImageInterface.h"

namespace vklite {

    class ImageBuilderInterface {
    public:
        ImageBuilderInterface();

        virtual ~ImageBuilderInterface() = 0;

        virtual std::unique_ptr<ImageInterface> build(const Device &device, uint32_t width, uint32_t height, vk::Format format) = 0;
    };

} // vklite
