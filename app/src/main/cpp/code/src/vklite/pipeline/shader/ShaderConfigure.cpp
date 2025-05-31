//
// Created by leixing on 2025/5/31.
//

#include "ShaderConfigure.h"

namespace vklite {

    ShaderConfigure::ShaderConfigure() = default;

    ShaderConfigure::~ShaderConfigure() = default;

    ShaderConfigure::ShaderConfigure(const ShaderConfigure &other) = default;

    ShaderConfigure &ShaderConfigure::operator=(const ShaderConfigure &other) {
        if (this != &other) {
            mVertexShaderCode = other.mVertexShaderCode;
            mFragmentShaderCode = other.mFragmentShaderCode;
            mComputeShaderCode = other.mComputeShaderCode;
            mPushConstantRanges = other.mPushConstantRanges;
            mDescriptorSetConfigures = other.mDescriptorSetConfigures;
        }
        return *this;
    };

    ShaderConfigure::ShaderConfigure(ShaderConfigure &&other) noexcept
            : mVertexShaderCode(std::move(other.mVertexShaderCode)),
              mFragmentShaderCode(std::move(other.mFragmentShaderCode)),
              mComputeShaderCode(std::move(other.mComputeShaderCode)),
              mPushConstantRanges(std::move(other.mPushConstantRanges)),
              mDescriptorSetConfigures(std::move(other.mDescriptorSetConfigures)) {}

    ShaderConfigure &ShaderConfigure::operator=(ShaderConfigure &&other) noexcept {
        if (this != &other) {
            mVertexShaderCode = std::move(other.mVertexShaderCode);
            mFragmentShaderCode = std::move(other.mFragmentShaderCode);
            mComputeShaderCode = std::move(other.mComputeShaderCode);
            mPushConstantRanges = std::move(other.mPushConstantRanges);
            mDescriptorSetConfigures = std::move(other.mDescriptorSetConfigures);
        }
        return *this;
    }

    /**
     *
     * shader code
     *
     */
    ShaderConfigure &ShaderConfigure::vertexShaderCode(std::vector<uint32_t> &&code) {
        mVertexShaderCode = std::move(code);
        return *this;
    }

    ShaderConfigure &ShaderConfigure::fragmentShaderCode(std::vector<uint32_t> &&code) {
        mFragmentShaderCode = std::move(code);
        return *this;
    }

    ShaderConfigure &ShaderConfigure::computeShaderCode(std::vector<uint32_t> &&code) {
        mComputeShaderCode = std::move(code);
        return *this;
    }

    /**
     *
     * push constants
     *
     */
    ShaderConfigure &ShaderConfigure::pushConstantRanges(const std::vector<vk::PushConstantRange> &pushConstantRanges) {
        mPushConstantRanges = pushConstantRanges;
        return *this;
    }

    ShaderConfigure &ShaderConfigure::pushConstantRanges(std::vector<vk::PushConstantRange> &&pushConstantRanges) {
        mPushConstantRanges = std::move(pushConstantRanges);
        return *this;
    }

    ShaderConfigure &ShaderConfigure::addPushConstant(const vk::PushConstantRange &pushConstantRange) {
        mPushConstantRanges.push_back(pushConstantRange);
        return *this;
    }

    ShaderConfigure &ShaderConfigure::addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags) {
//        addPushConstant(vk::PushConstantRange{stageFlags, offset, size});
        mPushConstantRanges.emplace_back(stageFlags, offset, size);
        return *this;
    }


    ShaderConfigure &ShaderConfigure::addDescriptorSetConfigure(DescriptorSetConfigure &&descriptorSetConfigure) {
        mDescriptorSetConfigures.emplace(descriptorSetConfigure.getSet(), std::move(descriptorSetConfigure));
        return *this;
    }

    ShaderConfigure &ShaderConfigure::addDescriptorSetConfigure(std::function<void(DescriptorSetConfigure &descriptorSet)> &configure) {
        DescriptorSetConfigure descriptorSetConfigure;
        configure(descriptorSetConfigure);
        addDescriptorSetConfigure(std::move(descriptorSetConfigure));
        return *this;
    }

    uint32_t ShaderConfigure::getDescriptorSetCount() const {
        return mDescriptorSetConfigures.size();
    }

    std::vector<vk::DescriptorPoolSize> ShaderConfigure::calcDescriptorPoolSizes() const {
        std::vector<vk::DescriptorPoolSize> descriptorPoolSizes;
        std::unordered_map<vk::DescriptorType, size_t> descriptorTypeToIndexMap;

        for (const auto &setEntry: mDescriptorSetConfigures) {
            uint32_t set = setEntry.first;
            const DescriptorSetConfigure &descriptorSet = setEntry.second;
            const std::unordered_map<uint32_t, DescriptorBindingConfigure> &descriptorBindingConfigures = descriptorSet.getDescriptorBindingConfigures();

            for (const auto &bindingEntry: descriptorBindingConfigures) {
                uint32_t binding = bindingEntry.first;
                const DescriptorBindingConfigure &descriptor = bindingEntry.second;

                const vk::DescriptorType type = descriptor.getDescriptorType();
                const uint32_t count = descriptor.getDescriptorCount();
                if (descriptorTypeToIndexMap.contains(type)) {
                    descriptorPoolSizes[descriptorTypeToIndexMap[type]].descriptorCount += count;
                } else {
                    descriptorTypeToIndexMap[type] = descriptorPoolSizes.size();
                    descriptorPoolSizes.emplace_back(type, count);
                }
            }
        }

        return descriptorPoolSizes;
    }

    std::vector<vk::DescriptorSetLayout> ShaderConfigure::createDescriptorSetLayouts(const Device &device) const {

        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;

        for (const auto &setEntry: mDescriptorSetConfigures) {
            uint32_t set = setEntry.first;
            const DescriptorSetConfigure &descriptorSetConfigure = setEntry.second;

            std::vector<vk::DescriptorSetLayoutBinding> descriptorSetLayoutBindings = descriptorSetConfigure.createDescriptorSetLayoutBindings();

            vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
            descriptorSetLayoutCreateInfo
                    .setBindings(descriptorSetLayoutBindings);

            vk::DescriptorSetLayout descriptorSetLayout = device.getDevice().createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
            descriptorSetLayouts.push_back(descriptorSetLayout);
        }

        return descriptorSetLayouts;
    }

} // vklite