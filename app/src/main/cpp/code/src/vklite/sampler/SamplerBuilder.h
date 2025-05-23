//
// Created by leixing on 2025/4/11.
//

#pragma once

#include "SamplerBuilderInterface.h"

namespace vklite {

    class SamplerBuilder : public SamplerBuilderInterface {
    private:
        float mMaxLoad;

    public:

        SamplerBuilder();

        ~SamplerBuilder() override;

        SamplerBuilder &maxLoad(float maxLoad);

        std::unique_ptr<SamplerInterface> build(const Device &device) override;

    };

} // vklite
