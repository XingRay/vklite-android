//
// Created by leixing on 2025/3/24.
//

#pragma once

#include <cstdint>
#include <memory>

#include "vulkan/vulkan.hpp"

#include "vklite/pipeline/descriptor/DescriptorBufferInfoConfigure.h"

namespace vklite {

    class UniformConfigure {

    private:
        uint32_t mBinding;
        vk::ShaderStageFlags mShaderStageFlags;
        uint32_t mDescriptorOffset;
        uint32_t mDescriptorRange;

    public:
        UniformConfigure();

        ~UniformConfigure();

        UniformConfigure &binding(uint32_t binding);

        UniformConfigure &descriptorOffset(uint32_t offset);

        UniformConfigure &descriptorRange(uint32_t range);

        UniformConfigure &shaderStageFlags(vk::ShaderStageFlags shaderStageFlags);

//        UniformConfigure &setUniformBuffer(uint32_t capacity, const void *data, uint32_t size);
//
//        UniformConfigure &setUniformBuffer(std::unique_ptr<BufferInfo> &&bufferInfo);
//
//        template<class T>
//        UniformConfigure &setUniformBuffer(uint32_t capacity, const std::vector<T> &data) {
//            return setUniformBuffer(capacity, data.data(), data.size() * sizeof(T));
//        }
//
//        template<class T>
//        UniformConfigure &setUniformBuffer(const std::vector<T> &data) {
//            uint32_t size = data.size() * sizeof(T);
//            return setUniformBuffer(size, data.data(), size);
//        }
//
//        template<class T>
//        UniformConfigure &setUniformBuffer(const T &data) {
//            uint32_t size = sizeof(T);
//            return setUniformBuffer(size, &data, size);
//        }

    private:

//        [[nodiscard]]
//        std::unique_ptr<DescriptorBindingConfigure> createDescriptorBindingConfigure();
    };

} // vklite
