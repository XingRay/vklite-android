//
// Created by leixing on 2025/5/19.
//

#pragma once

#include "vklite/device/Device.h"
#include "vklite/image/ImageInterface.h"
#include "vklite/image/Image.h"

namespace vklite {

    class ColorImage : public ImageInterface {
    private:
        Image mImage;

    public:
        explicit ColorImage(Image &&image);

        ~ColorImage() override;

        [[nodiscard]]
        const vk::Image &getImage() const override;

    };

} // vklite
