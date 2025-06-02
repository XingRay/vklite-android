//
// Created by leixing on 2025/4/11.
//

#pragma once

#include "SamplerBuilderInterface.h"
#include "vklite/physical_device/PhysicalDevice.h"

namespace vklite {

    class SamplerBuilder : public SamplerBuilderInterface {
    private:
        float mMaxLoad;

    public:

        SamplerBuilder();

        ~SamplerBuilder() override;

        SamplerBuilder &maxLoad(float maxLoad);

        std::unique_ptr<SamplerInterface> build(const PhysicalDevice &physicalDevice,const Device &device) override;

    };

} // vklite
