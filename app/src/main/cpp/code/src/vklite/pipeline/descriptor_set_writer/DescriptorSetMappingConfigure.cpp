//
// Created by leixing on 2025/5/13.
//

#include "DescriptorSetMappingConfigure.h"

namespace vklite {

    DescriptorSetMappingConfigure::DescriptorSetMappingConfigure() = default;

    DescriptorSetMappingConfigure::~DescriptorSetMappingConfigure() = default;

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addMapping(DescriptorMapping &&descriptorMapping) {
        mDescriptorMappings.push_back(std::move(descriptorMapping));
        return *this;
    }

    DescriptorSetMappingConfigure &DescriptorSetMappingConfigure::addMapping(std::function<void(DescriptorMapping &descriptorMapping)> configure) {
        DescriptorMapping descriptorMapping;
        configure(descriptorMapping);
        addMapping(std::move(descriptorMapping));
        return *this;
    }

    std::vector<DescriptorMapping> DescriptorSetMappingConfigure::getDescriptorMappings() {
        return mDescriptorMappings;
    }

} // vklite