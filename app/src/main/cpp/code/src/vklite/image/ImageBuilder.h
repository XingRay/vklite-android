//
// Created by leixing on 2025/4/11.
//

#pragma once

#include "ImageBuilderInterface.h"

namespace vklite {

    class ImageBuilder : public ImageBuilderInterface {
    private:

    public:
        ImageBuilder();

        ~ImageBuilder() override;

        [[nodiscard]]
        std::unique_ptr<ImageInterface> build(const Device &device, uint32_t width, uint32_t height, vk::Format format) override;
    };

} // vklite
