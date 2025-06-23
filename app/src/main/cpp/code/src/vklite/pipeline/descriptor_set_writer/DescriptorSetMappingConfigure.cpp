//
// Created by leixing on 2025/5/13.
//

#include "DescriptorSetMappingConfigure.h"

namespace vklite {

    DescriptorSetMappingConfigure::DescriptorSetMappingConfigure() = default;

    DescriptorSetMappingConfigure::~DescriptorSetMappingConfigure() = default;

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::descriptorSet(vk::DescriptorSet descriptorSet) {
        mDescriptorSet = descriptorSet;
        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addMapping(DescriptorMapping &&descriptorMapping) {
        mDescriptorMappings.push_back(std::move(descriptorMapping));
        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addMapping(const std::function<void(DescriptorMapping & descriptorMapping)> &configure) {
        DescriptorMapping descriptorMapping;
        configure(descriptorMapping);
        addMapping(std::move(descriptorMapping));
        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addUniformBuffer(UniformBufferDescriptorMapping &&descriptorMapping) {
        DescriptorMapping mapping{};
        mapping.binding(descriptorMapping.getBinding())
                .descriptorType(vk::DescriptorType::eUniformBuffer)
                .descriptorIndex(descriptorMapping.getDescriptorIndex())
                .bufferInfos(std::move(descriptorMapping.getDescriptorBufferInfos()));
        addMapping(std::move(mapping));

        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addUniformBuffer(const std::function<void(UniformBufferDescriptorMapping & descriptorMapping)> &configure) {
        UniformBufferDescriptorMapping mapping;
        configure(mapping);
        addUniformBuffer(std::move(mapping));
        return *this;
    }


    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addStorageBuffer(StorageBufferDescriptorMapping &&descriptorMapping) {
        DescriptorMapping mapping{};
        mapping.binding(descriptorMapping.getBinding())
                .descriptorType(vk::DescriptorType::eStorageBuffer)
                .descriptorIndex(descriptorMapping.getDescriptorIndex())
                .bufferInfos(std::move(descriptorMapping.getDescriptorBufferInfos()));
        addMapping(std::move(mapping));

        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addStorageBuffer(const std::function<void(StorageBufferDescriptorMapping & descriptorMapping)> &configure) {
        StorageBufferDescriptorMapping mapping;
        configure(mapping);
        addStorageBuffer(std::move(mapping));
        return *this;
    }


    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addCombinedImageSampler(CombinedImageSamplerDescriptorMapping &&descriptorMapping) {
        DescriptorMapping mapping{};
        mapping.binding(descriptorMapping.getBinding())
                .descriptorType(vk::DescriptorType::eCombinedImageSampler)
                .descriptorIndex(descriptorMapping.getDescriptorIndex())
                .imageInfos(std::move(descriptorMapping.getDescriptorImageInfos()));
        addMapping(std::move(mapping));

        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addCombinedImageSampler(const std::function<void(CombinedImageSamplerDescriptorMapping & descriptorMapping)> &configure) {
        CombinedImageSamplerDescriptorMapping mapping;
        configure(mapping);
        addCombinedImageSampler(std::move(mapping));
        return *this;
    }


    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addStorageImage(StorageImageDescriptorMapping &&descriptorMapping) {
        DescriptorMapping mapping{};
        mapping.binding(descriptorMapping.getBinding())
                .descriptorType(vk::DescriptorType::eStorageImage)
                .descriptorIndex(descriptorMapping.getDescriptorIndex())
                .imageInfos(std::move(descriptorMapping.getDescriptorImageInfos()));
        addMapping(std::move(mapping));

        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addStorageImage(const std::function<void(StorageImageDescriptorMapping & descriptorMapping)> &configure) {
        StorageImageDescriptorMapping mapping;
        configure(mapping);
        addStorageImage(std::move(mapping));
        return *this;
    }


    DescriptorSetWriter DescriptorSetMappingConfigure::createDescriptorSetWriter() {
        return DescriptorSetWriter(mDescriptorSet, std::move(mDescriptorMappings));
    }

} // vklite