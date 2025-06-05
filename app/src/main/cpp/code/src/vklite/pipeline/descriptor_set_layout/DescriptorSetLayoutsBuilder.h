//
// Created by leixing on 2025/6/5.
//

#pragma once

#include <vector>
#include <memory>

#include "vklite/pipeline/descriptor_set_layout/DescriptorSetLayouts.h"

namespace vklite {

    class DescriptorSetLayoutsBuilder {
    private:
        vk::Device mDevice;
        std::vector<std::vector<vk::DescriptorSetLayoutBinding>> mDescriptorSetLayoutBindings;

    public:
        DescriptorSetLayoutsBuilder();

        ~DescriptorSetLayoutsBuilder();

        DescriptorSetLayoutsBuilder &device(vk::Device device);

        DescriptorSetLayoutsBuilder &bindings(std::vector<std::vector<vk::DescriptorSetLayoutBinding>> &&descriptorSetLayoutBindings);

        DescriptorSetLayouts build();

        std::unique_ptr<DescriptorSetLayouts> buildUnique();
    };

} // vklite
