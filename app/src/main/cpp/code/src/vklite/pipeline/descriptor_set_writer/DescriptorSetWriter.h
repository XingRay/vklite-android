//
// Created by leixing on 2025/5/12.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor_set_writer/DescriptorMapping.h"

namespace vklite {

    class DescriptorSetWriter {
    private:
        std::vector<DescriptorMapping> mDescriptorMappings;

    public:
        DescriptorSetWriter(std::vector<DescriptorMapping> &&descriptorMappings);

        ~DescriptorSetWriter();

        std::vector<vk::WriteDescriptorSet> createWriteDescriptorSets() const;
    };

} // vklite
