//
// Created by leixing on 2025/5/12.
//

#pragma once

#include <cstdint>
#include <variant>

#include <vulkan/vulkan.hpp>

#include "vklite/buffer/Buffer.h"
#include "vklite/sampler/Sampler.h"
#include "vklite/image_view/ImageView.h"

namespace vklite {

    class DescriptorMapping {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorIndex;
        vk::DescriptorType mDescriptorType;

        std::vector<vk::DescriptorBufferInfo> mBufferInfos;
        std::vector<vk::DescriptorImageInfo> mImageInfos;

    public:

        explicit DescriptorMapping();

        ~DescriptorMapping();

        DescriptorMapping(const DescriptorMapping &other) = delete;

        DescriptorMapping &operator=(const DescriptorMapping &other) = delete;

        DescriptorMapping(DescriptorMapping &&other) noexcept;

        DescriptorMapping &operator=(DescriptorMapping &&other) noexcept;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getDescriptorIndex() const;

        [[nodiscard]]
        uint32_t getDescriptorCount() const;

        [[nodiscard]]
        vk::DescriptorType getVkDescriptorType() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorBufferInfo> &getDescriptorBufferInfos() const;

        [[nodiscard]]
        const std::vector<vk::DescriptorImageInfo> &getDescriptorImageInfos() const;


        // basic info
//        DescriptorMapping &descriptorSet(vk::DescriptorSet descriptorSet);

        DescriptorMapping &binding(uint32_t binding);

        DescriptorMapping &descriptorIndex(uint32_t descriptorIndex);

        DescriptorMapping &descriptorType(vk::DescriptorType descriptorType);


        // add buffer info
        DescriptorMapping &addBufferInfo(vk::DescriptorBufferInfo bufferInfo);

        DescriptorMapping &addBufferInfo(vk::Buffer buffer, vk::DeviceSize offset, vk::DeviceSize range);

        DescriptorMapping &addBufferInfo(const Buffer &buffer);

        DescriptorMapping &bufferInfos(std::vector<vk::DescriptorBufferInfo> &&bufferInfos);


        // add image info
        DescriptorMapping &addImageInfo(vk::DescriptorImageInfo imageInfo);

        DescriptorMapping &addImageInfo(vk::Sampler sampler, vk::ImageView imageView, vk::ImageLayout imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal);

        DescriptorMapping &addImageInfo(const Sampler &sampler, const ImageView &imageView, vk::ImageLayout imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal);

        DescriptorMapping &imageInfos(std::vector<vk::DescriptorImageInfo> &&imageInfos);

    };

} // vklite

