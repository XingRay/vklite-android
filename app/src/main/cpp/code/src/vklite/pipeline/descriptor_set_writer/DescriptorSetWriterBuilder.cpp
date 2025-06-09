//
// Created by leixing on 2025/5/12.
//

#include "DescriptorSetWriterBuilder.h"

namespace vklite {

    DescriptorSetWriterBuilder::DescriptorSetWriterBuilder() = default;

    DescriptorSetWriterBuilder::~DescriptorSetWriterBuilder() = default;

    DescriptorSetWriterBuilder &DescriptorSetWriterBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    DescriptorSetWriterBuilder &DescriptorSetWriterBuilder::descriptorSetMappingConfigure(std::function<void(uint32_t frameIndex, DescriptorSetMappingConfigure &configure)> &&configure) {
        mDescriptorSetMappingConfigure = std::move(configure);
        return *this;
    }

    std::vector<DescriptorSetWriter> DescriptorSetWriterBuilder::build() const {
        std::vector<DescriptorSetWriter> descriptorSetWriters;
        descriptorSetWriters.reserve(mFrameCount);

        for (uint32_t frameIndex = 0; frameIndex < mFrameCount; frameIndex++) {
            DescriptorSetMappingConfigure configure;
            mDescriptorSetMappingConfigure(frameIndex, configure);
            descriptorSetWriters.push_back(configure.createDescriptorSetWriter());
        }

        return descriptorSetWriters;
    }

} // vklite