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

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addMapping(const std::function<void(DescriptorMapping &descriptorMapping)> &configure) {
        DescriptorMapping descriptorMapping;
        configure(descriptorMapping);
        addMapping(std::move(descriptorMapping));
        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addUniform(UniformDescriptorMapping &&descriptorMapping) {
        DescriptorMapping mapping{};
        mapping.binding(descriptorMapping.getBinding())
                .descriptorType(vk::DescriptorType::eUniformBuffer)
                .descriptorIndex(descriptorMapping.getDescriptorIndex())
                .bufferInfos(std::move(descriptorMapping.getDescriptorBufferInfos()));
        addMapping(std::move(mapping));

        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addUniform(const std::function<void(UniformDescriptorMapping &descriptorMapping)> &configure) {
        UniformDescriptorMapping mapping;
        configure(mapping);
        addUniform(std::move(mapping));
        return *this;
    }


    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addSampler(SamplerDescriptorMapping &&descriptorMapping) {
        DescriptorMapping mapping{};
        mapping.binding(descriptorMapping.getBinding())
                .descriptorType(vk::DescriptorType::eCombinedImageSampler)
                .descriptorIndex(descriptorMapping.getDescriptorIndex())
                .imageInfos(std::move(descriptorMapping.getDescriptorImageInfos()));
        addMapping(std::move(mapping));

        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addSampler(const std::function<void(SamplerDescriptorMapping &descriptorMapping)> &configure) {
        SamplerDescriptorMapping mapping;
        configure(mapping);
        addSampler(std::move(mapping));
        return *this;
    }


    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addStorage(StorageDescriptorMapping &&descriptorMapping) {
        DescriptorMapping mapping{};
        mapping.binding(descriptorMapping.getBinding())
                .descriptorType(vk::DescriptorType::eStorageBuffer)
                .descriptorIndex(descriptorMapping.getDescriptorIndex())
                .bufferInfos(std::move(descriptorMapping.getDescriptorBufferInfos()));
        addMapping(std::move(mapping));

        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addStorage(const std::function<void(StorageDescriptorMapping &descriptorMapping)> &configure) {
        StorageDescriptorMapping mapping;
        configure(mapping);
        addStorage(std::move(mapping));
        return *this;
    }


    DescriptorSetWriter DescriptorSetMappingConfigure::createDescriptorSetWriter() {
        return DescriptorSetWriter(mDescriptorSet, std::move(mDescriptorMappings));
    }

} // vklite