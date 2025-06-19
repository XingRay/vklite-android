//
// Created by leixing on 2025-06-20.
//

#pragma once

#include <vector>

#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriter.h"

namespace vklite {

    class DescriptorSetWriters {
    private:
        std::vector<DescriptorSetWriter> mDescriptorSetWriters;

    public:
        explicit DescriptorSetWriters(std::vector<DescriptorSetWriter> &&descriptorSetWriters);

        ~DescriptorSetWriters();

        DescriptorSetWriters(const DescriptorSetWriters &other) = delete;

        DescriptorSetWriters &operator=(const DescriptorSetWriters &other) = delete;

        DescriptorSetWriters(DescriptorSetWriters &&other) noexcept;

        DescriptorSetWriters &operator=(DescriptorSetWriters &&other) noexcept;

        [[nodiscard]]
        std::vector<vk::WriteDescriptorSet> createWriteDescriptorSets() const;
    };

} // vklite
