//
// Created by leixing on 2025-06-20.
//

#include "DescriptorSetWriters.h"

namespace vklite {

    DescriptorSetWriters::DescriptorSetWriters(std::vector<DescriptorSetWriter> &&descriptorSetWriters)
            : mDescriptorSetWriters(std::move(descriptorSetWriters)) {}

    DescriptorSetWriters::~DescriptorSetWriters() = default;

    DescriptorSetWriters::DescriptorSetWriters(DescriptorSetWriters &&other) noexcept
            : mDescriptorSetWriters(std::move(other.mDescriptorSetWriters)) {}

    DescriptorSetWriters &DescriptorSetWriters::operator=(DescriptorSetWriters &&other) noexcept {
        if (this != &other) {
            mDescriptorSetWriters = std::move(other.mDescriptorSetWriters);
        }
        return *this;
    }

    std::vector<vk::WriteDescriptorSet> DescriptorSetWriters::createWriteDescriptorSets() const {
        std::vector<vk::WriteDescriptorSet> writeDescriptorSets;
        for (const DescriptorSetWriter &descriptorSetWriter: mDescriptorSetWriters) {
            std::vector<vk::WriteDescriptorSet> descriptorSets = descriptorSetWriter.createWriteDescriptorSets();
            writeDescriptorSets.insert(writeDescriptorSets.begin(), std::move_iterator(descriptorSets.begin()), std::move_iterator(descriptorSets.end()));
        }
        return writeDescriptorSets;
    }

} // vklite