//
// Created by leixing on 2025/5/19.
//

#include "ColorImage.h"

namespace vklite {

    ColorImage::ColorImage(Image &&image)
            : mImage(std::move(image)) {}

    ColorImage::~ColorImage() = default;

    const vk::Image &ColorImage::getImage() const {
        return mImage.getImage();
    }

} // vklite