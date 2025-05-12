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

    DescriptorSetWriterBuilder &DescriptorSetWriterBuilder::writeDescriptorSetProvider(std::function<std::vector<DescriptorMapping>(uint32_t)> &&provider) {
        mWriteDescriptorSetProvider = std::move(provider);
        return *this;
    }

    DescriptorSetWriter DescriptorSetWriterBuilder::build() const {
        std::vector<DescriptorMapping> writeDescriptorSets;

        for (uint32_t frameIndex = 0; frameIndex < mFrameCount; frameIndex++) {
            std::vector<DescriptorMapping> writeDescriptorSetsOfFrame = mWriteDescriptorSetProvider(frameIndex);

            writeDescriptorSets.insert(
                    writeDescriptorSets.end(),
                    std::make_move_iterator(writeDescriptorSetsOfFrame.begin()),
                    std::make_move_iterator(writeDescriptorSetsOfFrame.end())
            );
        }

//        return DescriptorSetWriter(std::move(writeDescriptorSets));
        return {std::move(writeDescriptorSets)};
    }

} // vklite