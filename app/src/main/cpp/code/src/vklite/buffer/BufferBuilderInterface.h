//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "vklite/device/Device.h"
#include "BufferInterface.h"

namespace vklite {

    class BufferBuilderInterface {
    public:
        BufferBuilderInterface();

        virtual ~BufferBuilderInterface() = 0;

        virtual std::unique_ptr<BufferInterface> build(const PhysicalDevice &physicalDevice, const Device &device, vk::DeviceSize bufferSize) = 0;
    };

} // vklite
