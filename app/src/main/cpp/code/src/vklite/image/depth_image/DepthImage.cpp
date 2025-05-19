//
// Created by leixing on 2025/5/19.
//

#include "DepthImage.h"

namespace vklite {

    DepthImage::DepthImage(Image &&image)
            : mImage(std::move(image)) {}

    DepthImage::~DepthImage() = default;

    const vk::Image &DepthImage::getImage() const {
        return mImage.getImage();
    }

} // vklite