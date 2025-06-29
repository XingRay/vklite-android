//
// Created by leixing on 2025/5/12.
//

#include "DescriptorSetWritersBuilder.h"

namespace vklite {

    DescriptorSetWritersBuilder::DescriptorSetWritersBuilder() = default;

    DescriptorSetWritersBuilder::~DescriptorSetWritersBuilder() = default;

    DescriptorSetWritersBuilder &DescriptorSetWritersBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    // todo: addDescriptorSetMappingConfigure
    DescriptorSetWritersBuilder &DescriptorSetWritersBuilder::descriptorSetMappingConfigure(std::function<void(uint32_t frameIndex, DescriptorSetMappingConfigure &configure)> &&configure) {
        mDescriptorSetMappingConfigure = std::move(configure);
        return *this;
    }

    DescriptorSetWriters DescriptorSetWritersBuilder::build() const {
        std::vector<DescriptorSetWriter> descriptorSetWriters;
        descriptorSetWriters.reserve(mFrameCount);

        for (uint32_t frameIndex = 0; frameIndex < mFrameCount; frameIndex++) {
            DescriptorSetMappingConfigure configure;
            mDescriptorSetMappingConfigure(frameIndex, configure);
            descriptorSetWriters.push_back(configure.createDescriptorSetWriter());
        }

        return DescriptorSetWriters(std::move(descriptorSetWriters));
    }

    std::unique_ptr<DescriptorSetWriters> DescriptorSetWritersBuilder::buildUnique() const {
        return std::make_unique<DescriptorSetWriters>(build());
    }

} // vklite