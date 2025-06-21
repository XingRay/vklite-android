//
// Created by leixing on 2025/4/29.
//

#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class MsaaSampleCountSelector {
    public:
        explicit MsaaSampleCountSelector();

        virtual ~MsaaSampleCountSelector() = 0;

        [[nodiscard]]
        virtual vk::SampleCountFlagBits select(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits) const = 0;
    };

} // vklite
