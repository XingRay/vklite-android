//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/pipeline/descriptor/DescriptorConfigure.h"

namespace vklite {

    class ShaderConfigure {
    private:
        std::vector<uint32_t> mVertexShaderCode;
        std::vector<uint32_t> mFragmentShaderCode;
        std::vector<uint32_t> mComputeShaderCode;

        // push constants
        std::vector<vk::PushConstantRange> mPushConstantRanges;

        // set -> DescriptorSetConfigure
        std::unordered_map<uint32_t, DescriptorSetConfigure> mDescriptorSetConfigures;

    public:
        ShaderConfigure();

        ~ShaderConfigure();

        ShaderConfigure(const ShaderConfigure &other);

        ShaderConfigure &operator=(const ShaderConfigure &other);

        ShaderConfigure(ShaderConfigure &&other) noexcept;

        ShaderConfigure &operator=(ShaderConfigure &&other) noexcept;

        /**
         *
         * shader code
         *
         */
        ShaderConfigure &vertexShaderCode(std::vector<uint32_t> &&code);

        ShaderConfigure &fragmentShaderCode(std::vector<uint32_t> &&code);

        ShaderConfigure &computeShaderCode(std::vector<uint32_t> &&code);

        /**
         *
         * push constants
         *
         */
        ShaderConfigure &pushConstantRanges(const std::vector<vk::PushConstantRange> &pushConstantRanges);

        ShaderConfigure &pushConstantRanges(std::vector<vk::PushConstantRange> &&pushConstantRanges);

        ShaderConfigure &addPushConstant(const vk::PushConstantRange &pushConstantRange);

        ShaderConfigure &addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags);


        /**
         *
         * descriptor set
         *
         */
        ShaderConfigure &addDescriptorSetConfigure(DescriptorSetConfigure &&descriptorSetConfigure);

        ShaderConfigure &addDescriptorSetConfigure(std::function<void(DescriptorSetConfigure &descriptorSet)>& configure);

        [[nodiscard]]
        uint32_t getDescriptorSetCount() const;

        [[nodiscard]]
        std::vector<vk::DescriptorPoolSize> calcDescriptorPoolSizes() const;

        [[nodiscard]]
        std::vector<vk::DescriptorSetLayout> createDescriptorSetLayouts(const Device &device) const;
    };

} // vklite
