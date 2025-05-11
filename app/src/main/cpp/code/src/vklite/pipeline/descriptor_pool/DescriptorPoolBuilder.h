//
// Created by leixing on 2025/5/11.
//

#pragma once

#include <memory>
#include <cstdint>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor_pool/DescriptorPool.h"

namespace vklite {

    class DescriptorPoolBuilder {
        // member
    private:
        uint32_t mDescriptorSetCount;
        std::vector<vk::DescriptorPoolSize> mDescriptorPoolSizes;
        uint32_t mFrameCount;

    public:
        DescriptorPoolBuilder();

        ~DescriptorPoolBuilder();

        DescriptorPoolBuilder &descriptorSetCount(uint32_t descriptorSetCount);

        DescriptorPoolBuilder &descriptorPoolSizes(std::vector<vk::DescriptorPoolSize> &&descriptorPoolSizes);

        DescriptorPoolBuilder &frameCount(uint32_t frameCount);

        DescriptorPool build(const Device &device);

        std::unique_ptr<DescriptorPool> buildUnique(const Device &device);

        //static
    private:
    public:
        static std::vector<vk::DescriptorPoolSize> calcDescriptorPoolSizes(const std::vector<vk::DescriptorPoolSize> &descriptorPoolSizes,
                                                                           uint32_t framerCount);
    };

} // vklite
