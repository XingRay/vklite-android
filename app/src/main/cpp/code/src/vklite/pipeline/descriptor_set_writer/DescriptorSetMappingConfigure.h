//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor_set_writer/DescriptorMapping.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriter.h"

#include "vklite/pipeline/descriptor_set_writer/binding/SamplerDescriptorMapping.h"
#include "vklite/pipeline/descriptor_set_writer/binding/StorageDescriptorMapping.h"
#include "vklite/pipeline/descriptor_set_writer/binding/UniformDescriptorMapping.h"

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


        DescriptorSetMappingConfigure &addUniform(UniformDescriptorMapping &&descriptorMapping);

        DescriptorSetMappingConfigure &addUniform(const std::function<void(UniformDescriptorMapping &descriptorMapping)> &configure);


        DescriptorSetMappingConfigure &addSampler(SamplerDescriptorMapping &&descriptorMapping);

        DescriptorSetMappingConfigure &addSampler(const std::function<void(SamplerDescriptorMapping &descriptorMapping)> &configure);


        DescriptorSetMappingConfigure &addStorage(StorageDescriptorMapping &&descriptorMapping);

        DescriptorSetMappingConfigure &addStorage(const std::function<void(StorageDescriptorMapping &descriptorMapping)> &configure);


        [[nodiscard]]
        DescriptorSetWriter createDescriptorSetWriter();

    };

} // vklite
