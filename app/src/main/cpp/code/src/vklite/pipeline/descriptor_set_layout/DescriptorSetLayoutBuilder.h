//
// Created by leixing on 2025-06-19.
//

#pragma once

#include <vector>
#include <memory>

#include "vklite/pipeline/descriptor_set_layout/DescriptorSetLayout.h"

namespace vklite {

    class DescriptorSetLayoutBuilder {
    private:
        vk::Device mDevice;

        vk::DescriptorSetLayoutCreateInfo mDescriptorSetLayoutCreateInfo;
        std::vector<vk::DescriptorSetLayoutBinding> mDescriptorSetLayoutBindings;

    public:

        DescriptorSetLayoutBuilder();

        ~DescriptorSetLayoutBuilder();

        DescriptorSetLayoutBuilder &device(vk::Device device);

        DescriptorSetLayoutBuilder &bindings(std::vector<vk::DescriptorSetLayoutBinding> &&descriptorSetLayoutBindings);

        DescriptorSetLayout build();

        std::unique_ptr<DescriptorSetLayout> buildUnique();
    };

} // vklite
