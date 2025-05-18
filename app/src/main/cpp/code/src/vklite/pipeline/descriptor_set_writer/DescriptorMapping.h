//
// Created by leixing on 2025/5/12.
//

#pragma once

#include <cstdint>
#include <variant>

#include <vulkan/vulkan.hpp>

#include "vklite/buffer/BufferInterface.h"
#include "vklite/sampler/SamplerInterface.h"
#include "vklite/image_view/ImageViewInterface.h"

namespace vklite {

    class DescriptorMapping {
    private:
        vk::DescriptorSet mDescriptorSet;
        uint32_t mBinding;

        uint32_t mDescriptorIndex;
        uint32_t mDescriptorCount;

        vk::DescriptorType mDescriptorType;

        std::variant<std::vector<vk::DescriptorBufferInfo>, std::vector<vk::DescriptorImageInfo>> mTarget;

    public:

        DescriptorMapping();

        ~DescriptorMapping();

        [[nodiscard]]
        const vk::DescriptorSet &getDescriptorSet() const;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getDescriptorIndex() const;

        [[nodiscard]]
        uint32_t getDescriptorCount() const;

        [[nodiscard]]
        vk::DescriptorType getDescriptorType() const;

        [[nodiscard]]
        std::optional<std::reference_wrapper<const std::vector<vk::DescriptorBufferInfo>>> getDescriptorBufferInfos() const;

        [[nodiscard]]
        std::optional<std::reference_wrapper<const std::vector<vk::DescriptorImageInfo>>> getDescriptorImageInfos() const;


        // basic info
        DescriptorMapping &descriptorSet(vk::DescriptorSet descriptorSet);

        DescriptorMapping &binding(uint32_t binding);

        DescriptorMapping &descriptorIndex(uint32_t descriptorIndex);

        DescriptorMapping &descriptorCount(uint32_t descriptorCount);

        DescriptorMapping &descriptorType(vk::DescriptorType descriptorType);


        // add buffer info
        DescriptorMapping &addBufferInfo(vk::DescriptorBufferInfo bufferInfo);

        DescriptorMapping &addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range);

        DescriptorMapping &addBufferInfo(const BufferInterface &bufferInterface);


        // add image info
        DescriptorMapping &addImageInfo(vk::DescriptorImageInfo imageInfo);

        DescriptorMapping &addImageInfo(vk::Sampler sampler, vk::ImageView imageView, vk::ImageLayout imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal);

        DescriptorMapping &addImageInfo(const SamplerInterface &sampler, const ImageViewInterface &imageView, vk::ImageLayout imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal);
    };

} // vklite

