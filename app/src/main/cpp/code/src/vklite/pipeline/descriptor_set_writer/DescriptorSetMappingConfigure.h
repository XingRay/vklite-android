//
// Created by leixing on 2025/5/13.
//

#pragma once

#include <functional>

#include "vklite/pipeline/descriptor_set_writer/DescriptorMapping.h"

namespace vklite {

    class DescriptorSetMappingConfigure {
    private:
        std::vector<DescriptorMapping> mDescriptorMappings;

    public:
        DescriptorSetMappingConfigure();

        ~DescriptorSetMappingConfigure();

        DescriptorSetMappingConfigure &addMapping(DescriptorMapping &&descriptorMapping);

        DescriptorSetMappingConfigure &addMapping(const std::function<void(DescriptorMapping &descriptorMapping)>& configure);

        [[nodiscard]]
        std::vector<DescriptorMapping> getDescriptorMappings();
    };

} // vklite
