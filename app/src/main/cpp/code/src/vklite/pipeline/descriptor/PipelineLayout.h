//
// Created by leixing on 2025/4/18.
//

#pragma once

#include <cstdint>
#include <unordered_map>

#include "vulkan/vulkan.hpp"
#include "vklite/device/Device.h"
#include "vklite/pipeline/descriptor/DescriptorConfigure.h"
#include "vklite/pipeline/descriptor/DescriptorPool.h"
#include "vklite/pipeline/descriptor/Descriptor.h"

namespace vklite {

    class PipelineLayout {
    private:
        const Device &mDevice;

        // [set/binding] -> Descriptor
        std::unique_ptr<std::unordered_map<uint32_t, std::unordered_map<uint32_t, Descriptor>>> mDescriptors;

        // push constants
        std::vector<vk::PushConstantRange> mPushConstantRanges;

        vk::PipelineLayout mPipelineLayout;

    public:
        PipelineLayout(const Device &device,
                       std::unique_ptr<std::unordered_map<uint32_t, std::unordered_map<uint32_t, Descriptor>>> &&descriptors,
                       std::vector<vk::PushConstantRange> &&pushConstantRanges);

        ~PipelineLayout();

        [[nodiscard]]
        const vk::PipelineLayout &getPipelineLayout() const;

        [[nodiscard]]
        std::unique_ptr<DescriptorPool> createDescriptorPool(uint32_t frameCount) const;

        [[nodiscard]]
        std::vector<vk::DescriptorSetLayout> createDescriptorSetLayouts() const;

        [[nodiscard]]
        uint32_t calcTotalPushConstantSize() const;
    };

} // vklite
