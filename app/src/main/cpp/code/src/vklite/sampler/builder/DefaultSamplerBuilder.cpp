//
// Created by leixing on 2025/4/11.
//

#include "DefaultSamplerBuilder.h"
#include "vklite/sampler/DefaultSampler.h"

namespace vklite {

    DefaultSamplerBuilder::DefaultSamplerBuilder()
            : mMaxLoad(0.0f) {};

    DefaultSamplerBuilder::~DefaultSamplerBuilder() = default;

    std::unique_ptr<SamplerInterface> DefaultSamplerBuilder::build(const Device &device) {
        return std::make_unique<DefaultSampler>(device, mMaxLoad);
    }

    DefaultSamplerBuilder &DefaultSamplerBuilder::setMaxLoad(float maxLoad) {
        mMaxLoad = maxLoad;
        return *this;
    }

} // vklite