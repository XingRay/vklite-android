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

    DescriptorSetWriter DescriptorSetMappingConfigure::createDescriptorSetWriter() {
        return DescriptorSetWriter(mDescriptorSet, std::move(mDescriptorMappings));
    }

} // vklite