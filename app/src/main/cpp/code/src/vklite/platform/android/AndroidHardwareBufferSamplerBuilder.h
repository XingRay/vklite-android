//
// Created by leixing on 2025/4/15.
//

#pragma once

#include <memory>

#include "vklite/sampler/SamplerBuilderInterface.h"
#include "AndroidHardwareBufferYcbcrConversion.h"

namespace vklite {

    class AndroidHardwareBufferSamplerBuilder : public SamplerBuilderInterface {
    private:
        const AndroidHardwareBufferYcbcrConversion &mAndroidSamplerYcbcrConversion;

    public:

        AndroidHardwareBufferSamplerBuilder(const AndroidHardwareBufferYcbcrConversion &vulkanAndroidSamplerYcbcrConversion);

        ~AndroidHardwareBufferSamplerBuilder() override;

        std::unique_ptr<SamplerInterface> build(const Device &device) override;

    };

} // vklite
