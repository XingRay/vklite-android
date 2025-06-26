//
// Created by leixing on 2025/5/12.
//

#include "DescriptorSetWriter.h"

#include <utility>

namespace vklite {

    DescriptorSetWriter::DescriptorSetWriter(vk::DescriptorSet descriptorSet, std::vector<DescriptorMapping> &&descriptorMappings)
            : mDescriptorSet(descriptorSet),
              mDescriptorMappings(std::move(descriptorMappings)) {}

    DescriptorSetWriter::~DescriptorSetWriter() = default;

    DescriptorSetWriter::DescriptorSetWriter(DescriptorSetWriter &&other) noexcept
            : mDescriptorSet(std::exchange(other.mDescriptorSet, nullptr)),
              mDescriptorMappings(std::move(other.mDescriptorMappings)) {}

    DescriptorSetWriter &DescriptorSetWriter::operator=(DescriptorSetWriter &&other) noexcept {
        if (this != &other) {
            mDescriptorSet = std::exchange(other.mDescriptorSet, nullptr);
            mDescriptorMappings = std::move(other.mDescriptorMappings);
        }
        return *this;
    }

    std::vector<vk::WriteDescriptorSet> DescriptorSetWriter::createWriteDescriptorSets() const {
        std::vector<vk::WriteDescriptorSet> writeDescriptorSets;

        for (const DescriptorMapping &mapping: mDescriptorMappings) {
            vk::WriteDescriptorSet writeDescriptorSet{};

            writeDescriptorSet
                    .setDstSet(mDescriptorSet)
                    .setDstBinding(mapping.getBinding())
                    .setDstArrayElement(mapping.getDescriptorIndex())
//                    .setDescriptorCount(mapping.getDescriptorCount())
                    .setDescriptorType(mapping.getVkDescriptorType());
            if (!mapping.getDescriptorBufferInfos().empty()) {
                writeDescriptorSet.setBufferInfo(mapping.getDescriptorBufferInfos());
            }
            if (!mapping.getDescriptorImageInfos().empty()) {
                writeDescriptorSet.setImageInfo(mapping.getDescriptorImageInfos());
            }

            writeDescriptorSets.push_back(writeDescriptorSet);
        }

        return writeDescriptorSets;
    }

} // vklite