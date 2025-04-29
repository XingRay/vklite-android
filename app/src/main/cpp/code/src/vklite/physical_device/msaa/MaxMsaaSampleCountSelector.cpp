//
// Created by leixing on 2025/4/29.
//

#include "MaxMsaaSampleCountSelector.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    MaxMsaaSampleCountSelector::MaxMsaaSampleCountSelector(uint32_t maxLimit) : mUint32Selector(maxLimit) {}

    MaxMsaaSampleCountSelector::MaxMsaaSampleCountSelector() : mUint32Selector(std::numeric_limits<uint32_t>::max()) {}

    MaxMsaaSampleCountSelector::~MaxMsaaSampleCountSelector() = default;

    vk::SampleCountFlagBits MaxMsaaSampleCountSelector::select(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits) const {
        std::vector<uint32_t> countValues;
        countValues.reserve(sampleCountFlagBits.size());
        for (const vk::SampleCountFlagBits &bits: sampleCountFlagBits) {
            countValues.push_back(VulkanUtil::sampleCountFlagBitsToUint32(bits));
        }

        uint32_t value = mUint32Selector.select(countValues);
        return VulkanUtil::uint32ToSampleCountFlagBits(value);
    }

} // vklite