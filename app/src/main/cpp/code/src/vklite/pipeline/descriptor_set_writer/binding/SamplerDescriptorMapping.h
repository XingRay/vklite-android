//
// Created by leixing on 2025/6/7.
//

#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/image/Image.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorMapping.h"

namespace vklite {

    class SamplerDescriptorMapping {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorIndex;
        std::vector<vk::DescriptorImageInfo> mDescriptorImageInfos;

    public:

        explicit SamplerDescriptorMapping();

        ~SamplerDescriptorMapping();

        SamplerDescriptorMapping(const SamplerDescriptorMapping &other) = delete;

        SamplerDescriptorMapping &operator=(const SamplerDescriptorMapping &other) = delete;

        SamplerDescriptorMapping(SamplerDescriptorMapping &&other) noexcept;

        SamplerDescriptorMapping &operator=(SamplerDescriptorMapping &&other) noexcept;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getDescriptorIndex() const;

        [[nodiscard]]
        std::vector<vk::DescriptorImageInfo> &getDescriptorImageInfos();

        SamplerDescriptorMapping &binding(uint32_t binding);

        SamplerDescriptorMapping &descriptorIndex(uint32_t descriptorIndex);

        // add image info
        SamplerDescriptorMapping &addImageInfo(vk::DescriptorImageInfo imageInfo);

        SamplerDescriptorMapping &addImageInfo(vk::Sampler sampler, vk::ImageView imageView, vk::ImageLayout imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal);

        SamplerDescriptorMapping &addImageInfo(const Sampler &sampler, const ImageView &imageView, vk::ImageLayout imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal);

    };

} // vklite
