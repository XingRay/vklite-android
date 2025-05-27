//
// Created by leixing on 2025/4/11.
//

#include "SamplerBuilder.h"
#include "vklite/sampler/Sampler.h"

namespace vklite {

    SamplerBuilder::SamplerBuilder()
            : mMaxLoad(vk::LodClampNone) {};

    SamplerBuilder::~SamplerBuilder() = default;

    std::unique_ptr<SamplerInterface> SamplerBuilder::build(const PhysicalDevice &physicalDevice, const Device &device) {
        return std::make_unique<Sampler>(physicalDevice, device, mMaxLoad);
    }

    SamplerBuilder &SamplerBuilder::maxLoad(float maxLoad) {
        mMaxLoad = maxLoad;
        return *this;
    }

} // vklite