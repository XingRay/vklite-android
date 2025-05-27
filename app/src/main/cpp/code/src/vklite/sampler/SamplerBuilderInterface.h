//
// Created by leixing on 2025/4/4.
//

#pragma once

#include <memory>

#include "vklite/device/Device.h"
#include "vklite/sampler/SamplerInterface.h"

namespace vklite {

    class SamplerBuilderInterface {
    public:
        SamplerBuilderInterface();

        virtual ~SamplerBuilderInterface() = 0;

        virtual std::unique_ptr<SamplerInterface> build(const PhysicalDevice &physicalDevice, const Device &device) = 0;
    };

} // vklite
