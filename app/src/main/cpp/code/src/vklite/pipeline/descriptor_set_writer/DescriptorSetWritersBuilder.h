//
// Created by leixing on 2025/5/12.
//

#pragma once

#include <memory>
#include <cstdint>
#include <functional>

#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriter.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetMappingConfigure.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorSetWriters.h"

namespace vklite {

    class DescriptorSetWritersBuilder {
    private:
        uint32_t mFrameCount;
        std::function<void(uint32_t frameIndex, DescriptorSetMappingConfigure &configure)> mDescriptorSetMappingConfigure;

    public:
        DescriptorSetWritersBuilder();

        ~DescriptorSetWritersBuilder();

        DescriptorSetWritersBuilder &frameCount(uint32_t frameCount);

        DescriptorSetWritersBuilder &descriptorSetMappingConfigure(std::function<void(uint32_t frameIndex, DescriptorSetMappingConfigure &configure)> &&configure);

        [[nodiscard]]
        DescriptorSetWriters build() const;

        [[nodiscard]]
        std::unique_ptr<DescriptorSetWriters> buildUnique() const;
    };

} // vklite
