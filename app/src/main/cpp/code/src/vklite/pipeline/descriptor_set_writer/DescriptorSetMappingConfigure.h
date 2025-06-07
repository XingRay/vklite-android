//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor_set_writer/DescriptorMapping.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriter.h"

namespace vklite {

    class DescriptorSetMappingConfigure {
    private:
        std::vector<DescriptorMapping> mDescriptorMappings;
        vk::DescriptorSet mDescriptorSet;

    public:
        DescriptorSetMappingConfigure();

        ~DescriptorSetMappingConfigure();

        DescriptorSetMappingConfigure &descriptorSet(vk::DescriptorSet descriptorSet);

        DescriptorSetMappingConfigure &addMapping(DescriptorMapping &&descriptorMapping);

        DescriptorSetMappingConfigure &addMapping(const std::function<void(DescriptorMapping &descriptorMapping)> &configure);

        [[nodiscard]]
        DescriptorSetWriter createDescriptorSetWriter();

    };

} // vklite
