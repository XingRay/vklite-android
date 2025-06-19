//
// Created by leixing on 2025-06-19.
//

#include "DescriptorSetLayoutBuilder.h"

namespace vklite {

    DescriptorSetLayoutBuilder::DescriptorSetLayoutBuilder() = default;

    DescriptorSetLayoutBuilder::~DescriptorSetLayoutBuilder() = default;

    DescriptorSetLayoutBuilder &DescriptorSetLayoutBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    DescriptorSetLayoutBuilder &DescriptorSetLayoutBuilder::bindings(std::vector<vk::DescriptorSetLayoutBinding> &&descriptorSetLayoutBindings) {
        mDescriptorSetLayoutBindings = std::move(descriptorSetLayoutBindings);
        mDescriptorSetLayoutCreateInfo.setBindings(mDescriptorSetLayoutBindings);
        return *this;
    }

    DescriptorSetLayout DescriptorSetLayoutBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("DescriptorSetLayoutsBuilder::build(): mDevice == nullptr");
        }

        vk::DescriptorSetLayout descriptorSetLayout = mDevice.createDescriptorSetLayout(mDescriptorSetLayoutCreateInfo);

        return DescriptorSetLayout{mDevice, descriptorSetLayout};
    }

    std::unique_ptr<DescriptorSetLayout> DescriptorSetLayoutBuilder::buildUnique() {
        return std::make_unique<DescriptorSetLayout>(build());
    }

} // vklite