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

namespace vklite {

    class DescriptorSetWriterBuilder {
    private:
        uint32_t mFrameCount;
        std::function<void(uint32_t, DescriptorSetMappingConfigure &)> mDescriptorSetMappingConfigure;

    public:
        DescriptorSetWriterBuilder();

        ~DescriptorSetWriterBuilder();

        DescriptorSetWriterBuilder &frameCount(uint32_t frameCount);

        DescriptorSetWriterBuilder &descriptorSetMappingConfigure(std::function<void(uint32_t, DescriptorSetMappingConfigure &)> &&configure);

        [[nodiscard]]
        std::vector<DescriptorSetWriter> build() const;
    };

} // vklite
