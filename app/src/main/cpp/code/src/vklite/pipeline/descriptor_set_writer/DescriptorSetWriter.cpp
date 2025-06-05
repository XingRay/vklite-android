//
// Created by leixing on 2025/5/12.
//

#include "DescriptorSetWriter.h"

namespace vklite {

    DescriptorSetWriter::DescriptorSetWriter(std::vector<DescriptorMapping> &&descriptorMappings)
            : mDescriptorMappings(std::move(descriptorMappings)) {}

    DescriptorSetWriter::~DescriptorSetWriter() = default;

    std::vector<vk::WriteDescriptorSet> DescriptorSetWriter::createWriteDescriptorSets() const {
        std::vector<vk::WriteDescriptorSet> writeDescriptorSets;

        for (const DescriptorMapping &mapping: mDescriptorMappings) {
            vk::WriteDescriptorSet writeDescriptorSet{};

            writeDescriptorSet
                    .setDstSet(mapping.getDescriptorSet())
                    .setDstBinding(mapping.getBinding())
                    .setDstArrayElement(mapping.getDescriptorIndex())
                    .setDescriptorCount(mapping.getDescriptorCount())
                    .setDescriptorType(mapping.getDescriptorType());
            if (mapping.getDescriptorBufferInfos().has_value()) {
                writeDescriptorSet.setBufferInfo(mapping.getDescriptorBufferInfos().value().get());
            }
            if (mapping.getDescriptorImageInfos().has_value()) {
                writeDescriptorSet.setImageInfo(mapping.getDescriptorImageInfos().value().get());
            }

            writeDescriptorSets.push_back(writeDescriptorSet);
        }

        return writeDescriptorSets;
    }

} // vklite