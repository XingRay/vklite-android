//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor_set_writer/DescriptorMapping.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriter.h"

#include "vklite/pipeline/descriptor_set_writer/binding/CombinedImageSamplerDescriptorMapping.h"
#include "vklite/pipeline/descriptor_set_writer/binding/StorageBufferDescriptorMapping.h"
#include "vklite/pipeline/descriptor_set_writer/binding/UniformBufferDescriptorMapping.h"
#include "vklite/pipeline/descriptor_set_writer/binding/StorageImageDescriptorMapping.h"

namespace vklite {

    class DescriptorSetMappingConfigure {
    private:
        std::vector<DescriptorMapping> mDescriptorMappings;
        vk::DescriptorSet mDescriptorSet;

    public:
        explicit DescriptorSetMappingConfigure();

        ~DescriptorSetMappingConfigure();

        DescriptorSetMappingConfigure &descriptorSet(vk::DescriptorSet descriptorSet);

        DescriptorSetMappingConfigure &addMapping(DescriptorMapping &&descriptorMapping);

        DescriptorSetMappingConfigure &addMapping(const std::function<void(DescriptorMapping &descriptorMapping)> &configure);


        DescriptorSetMappingConfigure &addUniformBuffer(UniformBufferDescriptorMapping &&descriptorMapping);

        DescriptorSetMappingConfigure &addUniformBuffer(const std::function<void(UniformBufferDescriptorMapping &descriptorMapping)> &configure);


        DescriptorSetMappingConfigure &addStorageBuffer(StorageBufferDescriptorMapping &&descriptorMapping);

        DescriptorSetMappingConfigure &addStorageBuffer(const std::function<void(StorageBufferDescriptorMapping &descriptorMapping)> &configure);


        DescriptorSetMappingConfigure &addCombinedImageSampler(CombinedImageSamplerDescriptorMapping &&descriptorMapping);

        DescriptorSetMappingConfigure &addCombinedImageSampler(const std::function<void(CombinedImageSamplerDescriptorMapping &descriptorMapping)> &configure);


        DescriptorSetMappingConfigure &addStorageImage(StorageImageDescriptorMapping &&descriptorMapping);

        DescriptorSetMappingConfigure &addStorageImage(const std::function<void(StorageImageDescriptorMapping &descriptorMapping)> &configure);

        [[nodiscard]]
        DescriptorSetWriter createDescriptorSetWriter();

    };

} // vklite
