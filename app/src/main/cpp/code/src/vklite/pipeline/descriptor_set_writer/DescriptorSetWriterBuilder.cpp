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

    DescriptorSetWriterBuilder &DescriptorSetWriterBuilder::descriptorSetMappingConfigure(std::function<void(uint32_t, DescriptorSetMappingConfigure &)> &&configure) {
        mDescriptorSetMappingConfigure = std::move(configure);
        return *this;
    }

    DescriptorSetWriter DescriptorSetWriterBuilder::build() const {
        std::vector<DescriptorMapping> descriptorMappings;

        for (uint32_t frameIndex = 0; frameIndex < mFrameCount; frameIndex++) {
            DescriptorSetMappingConfigure configure;
            mDescriptorSetMappingConfigure(frameIndex, configure);
            std::vector<DescriptorMapping> writeDescriptorSetsOfFrame = configure.getDescriptorMappings();

                    descriptorMappings.insert(
                            descriptorMappings.end(),
                            std::make_move_iterator(writeDescriptorSetsOfFrame.begin()),
                            std::make_move_iterator(writeDescriptorSetsOfFrame.end())
            );
        }

//        return DescriptorSetWriter(std::move(writeDescriptorSets));
        return {std::move(descriptorMappings)};
    }

} // vklite