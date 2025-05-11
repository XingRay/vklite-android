//
// Created by leixing on 2025/4/18.
//

#pragma once

#include <cstdint>
#include <unordered_map>

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/pipeline/descriptor_pool/DescriptorPool.h"
#include "vklite/pipeline/descriptor/old/Descriptor.h"

namespace vklite {

    class PipelineLayout {
    private:
        const Device &mDevice;

        vk::PipelineLayout mPipelineLayout;

    public:
        PipelineLayout(const Device &device,
                       const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts,
                       const std::vector<vk::PushConstantRange> &pushConstantRanges);

        ~PipelineLayout();

        [[nodiscard]]
        const vk::PipelineLayout &getPipelineLayout() const;
    };

} // vklite
