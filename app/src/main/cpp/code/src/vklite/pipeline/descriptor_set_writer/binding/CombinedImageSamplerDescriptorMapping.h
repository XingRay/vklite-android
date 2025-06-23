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

    class CombinedImageSamplerDescriptorMapping {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorIndex;
        std::vector<vk::DescriptorImageInfo> mDescriptorImageInfos;

    public:

        explicit CombinedImageSamplerDescriptorMapping();

        ~CombinedImageSamplerDescriptorMapping();

        CombinedImageSamplerDescriptorMapping(const CombinedImageSamplerDescriptorMapping &other) = delete;

        CombinedImageSamplerDescriptorMapping &operator=(const CombinedImageSamplerDescriptorMapping &other) = delete;

        CombinedImageSamplerDescriptorMapping(CombinedImageSamplerDescriptorMapping &&other) noexcept;

        CombinedImageSamplerDescriptorMapping &operator=(CombinedImageSamplerDescriptorMapping &&other) noexcept;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getDescriptorIndex() const;

        [[nodiscard]]
        std::vector<vk::DescriptorImageInfo> &getDescriptorImageInfos();

        CombinedImageSamplerDescriptorMapping &binding(uint32_t binding);

        CombinedImageSamplerDescriptorMapping &descriptorIndex(uint32_t descriptorIndex);

        // add image info
        CombinedImageSamplerDescriptorMapping &addImageInfo(vk::DescriptorImageInfo imageInfo);

        CombinedImageSamplerDescriptorMapping &addImageInfo(vk::Sampler sampler, vk::ImageView imageView, vk::ImageLayout imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal);

        CombinedImageSamplerDescriptorMapping &addImageInfo(const Sampler &sampler, const ImageView &imageView, vk::ImageLayout imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal);

    };

} // vklite
