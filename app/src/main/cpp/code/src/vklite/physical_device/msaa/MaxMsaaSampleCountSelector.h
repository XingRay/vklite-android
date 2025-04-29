//
// Created by leixing on 2025/4/29.
//

#pragma once

#include "vklite/physical_device/msaa/MsaaSampleCountSelector.h"
#include "vklite/util/selector/MaxUint32Selector.h"

namespace vklite {

    class MaxMsaaSampleCountSelector : public MsaaSampleCountSelector {
    private:
        MaxUint32Selector mUint32Selector;

    public:

        explicit MaxMsaaSampleCountSelector(uint32_t maxLimit);

        MaxMsaaSampleCountSelector();

        ~MaxMsaaSampleCountSelector() override;

        [[nodiscard]]
        vk::SampleCountFlagBits select(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits) const override;
    };

} // vklite
