//
// Created by leixing on 2025/5/11.
//

#include "DescriptorPoolBuilder.h"

#include "vklite/Log.h"

namespace vklite {

    DescriptorPoolBuilder::DescriptorPoolBuilder()
            : mFrameCount(1), mDescriptorSetCount(0) {}

    DescriptorPoolBuilder::~DescriptorPoolBuilder() = default;

    DescriptorPoolBuilder &DescriptorPoolBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    DescriptorPoolBuilder &DescriptorPoolBuilder::descriptorSetCount(uint32_t descriptorSetCount) {
        mDescriptorSetCount = descriptorSetCount;
        return *this;
    }

    DescriptorPoolBuilder &DescriptorPoolBuilder::addDescriptorSetCount(uint32_t descriptorSetCount) {
        mDescriptorSetCount += descriptorSetCount;
        return *this;
    }

    DescriptorPoolBuilder &DescriptorPoolBuilder::descriptorPoolSizes(std::vector<vk::DescriptorPoolSize> &&descriptorPoolSizes) {
        mDescriptorPoolSizes = std::move(descriptorPoolSizes);
        return *this;
    }

    DescriptorPoolBuilder &DescriptorPoolBuilder::addDescriptorPoolSizes(std::vector<vk::DescriptorPoolSize> &&descriptorPoolSizes) {
        mDescriptorPoolSizes.insert(mDescriptorPoolSizes.begin(),
                                    std::move_iterator(descriptorPoolSizes.begin()),
                                    std::move_iterator(descriptorPoolSizes.end()));
        return *this;
    }

    DescriptorPoolBuilder &DescriptorPoolBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    DescriptorPool DescriptorPoolBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("DescriptorPoolBuilder::build() mDevice == nullptr");
        }
        std::vector<vk::DescriptorPoolSize> descriptorPoolSizes = mergeDescriptorPoolSizes(mDescriptorPoolSizes, mFrameCount);
        uint32_t maxSets = mDescriptorSetCount * mFrameCount;

        vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo;
        descriptorPoolCreateInfo
                .setPoolSizes(descriptorPoolSizes)
                .setMaxSets(maxSets)
                .setFlags(vk::DescriptorPoolCreateFlags{});

        vk::DescriptorPool descriptorPool = mDevice.createDescriptorPool(descriptorPoolCreateInfo);
        LOG_D("mDevice.createDescriptorPool => %p", (void *) descriptorPool);

        return DescriptorPool{mDevice, descriptorPool};
    }

    std::unique_ptr<DescriptorPool> DescriptorPoolBuilder::buildUnique() {
        return std::make_unique<DescriptorPool>(build());
    }

    std::vector<vk::DescriptorPoolSize> DescriptorPoolBuilder::mergeDescriptorPoolSizes(const std::vector<vk::DescriptorPoolSize> &descriptorPoolSizes, uint32_t framerCount) {
        std::vector<vk::DescriptorPoolSize> mergedDescriptorPoolSizes;
        // type -> index in mergedDescriptorPoolSizes
        std::unordered_map<vk::DescriptorType, size_t> descriptorTypeToIndexMap;

        for (const vk::DescriptorPoolSize &poolSize: descriptorPoolSizes) {
            const vk::DescriptorType type = poolSize.type;
            const uint32_t count = poolSize.descriptorCount * framerCount;
            if (descriptorTypeToIndexMap.contains(type)) {
                mergedDescriptorPoolSizes[descriptorTypeToIndexMap[type]].descriptorCount += count;
            } else {
                descriptorTypeToIndexMap[type] = mergedDescriptorPoolSizes.size();
                mergedDescriptorPoolSizes.emplace_back(type, count);
            }
        }

        return mergedDescriptorPoolSizes;
    }

} // vklite