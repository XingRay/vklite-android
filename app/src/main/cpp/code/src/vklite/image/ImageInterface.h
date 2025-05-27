//
// Created by leixing on 2025/3/30.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include "vklite/command_buffer/CommandPool.h"

namespace vklite {

    class ImageInterface {
    private:

    public:
        ImageInterface();

        virtual ~ImageInterface() = 0;

        [[nodiscard]]
        virtual const vk::Image &getImage() const = 0;
    };

} // vklite
