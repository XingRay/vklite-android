//
// Created by leixing on 2025-06-24.
//

#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/image/Image.h"
#include "vklite/pipeline/descriptor_set_writer/DescriptorMapping.h"

namespace vklite {

    class StorageImageDescriptorMapping {
    private:
        uint32_t mBinding;
        uint32_t mDescriptorIndex;
        std::vector<vk::DescriptorImageInfo> mDescriptorImageInfos;

    public:

        explicit StorageImageDescriptorMapping();

        ~StorageImageDescriptorMapping();

        StorageImageDescriptorMapping(const StorageImageDescriptorMapping &other) = delete;

        StorageImageDescriptorMapping &operator=(const StorageImageDescriptorMapping &other) = delete;

        StorageImageDescriptorMapping(StorageImageDescriptorMapping &&other) noexcept;

        StorageImageDescriptorMapping &operator=(StorageImageDescriptorMapping &&other) noexcept;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getDescriptorIndex() const;

        [[nodiscard]]
        std::vector<vk::DescriptorImageInfo> &getDescriptorImageInfos();

        StorageImageDescriptorMapping &binding(uint32_t binding);

        StorageImageDescriptorMapping &descriptorIndex(uint32_t descriptorIndex);

        // add image info
        StorageImageDescriptorMapping &addImageInfo(vk::DescriptorImageInfo imageInfo);

        StorageImageDescriptorMapping &addImageInfo(vk::ImageView imageView, vk::ImageLayout imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal);

        StorageImageDescriptorMapping &addImageInfo(const ImageView &imageView, vk::ImageLayout imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal);

    };

} // vklite
