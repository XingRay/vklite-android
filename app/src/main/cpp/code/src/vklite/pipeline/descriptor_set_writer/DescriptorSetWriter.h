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
        vk::DescriptorSet mDescriptorSet;
        std::vector<DescriptorMapping> mDescriptorMappings;

    public:
        explicit DescriptorSetWriter(vk::DescriptorSet descriptorSet, std::vector<DescriptorMapping> &&descriptorMappings);

        ~DescriptorSetWriter();

        DescriptorSetWriter(const DescriptorSetWriter &other) = delete;

        DescriptorSetWriter &operator=(const DescriptorSetWriter &other) = delete;

        DescriptorSetWriter(DescriptorSetWriter &&other) noexcept;

        DescriptorSetWriter &operator=(DescriptorSetWriter &&other) noexcept;

        [[nodiscard]]
        std::vector<vk::WriteDescriptorSet> createWriteDescriptorSets() const;
    };

} // vklite
