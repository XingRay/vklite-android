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

namespace vklite {

    class DescriptorSetWriterBuilder {
    private:
        uint32_t mFrameCount;
        std::function<std::vector<DescriptorMapping>(uint32_t)> mWriteDescriptorSetProvider;

    public:
        DescriptorSetWriterBuilder();

        ~DescriptorSetWriterBuilder();

        DescriptorSetWriterBuilder &frameCount(uint32_t frameCount);

        DescriptorSetWriterBuilder &writeDescriptorSetProvider(std::function<std::vector<DescriptorMapping>(uint32_t)> &&provider);

        [[nodiscard]]
        DescriptorSetWriter build() const;
    };

} // vklite
