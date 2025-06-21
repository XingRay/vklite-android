//
// Created by leixing on 2025/6/5.
//

#include "DescriptorSetLayoutsBuilder.h"

#include "vklite/Log.h"


namespace vklite {

    DescriptorSetLayoutsBuilder::DescriptorSetLayoutsBuilder() = default;

    DescriptorSetLayoutsBuilder::~DescriptorSetLayoutsBuilder() = default;

    DescriptorSetLayoutsBuilder &DescriptorSetLayoutsBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    DescriptorSetLayoutsBuilder &DescriptorSetLayoutsBuilder::bindings(std::vector<std::vector<vk::DescriptorSetLayoutBinding>> &&descriptorSetLayoutBindings) {
        mDescriptorSetLayoutBindings = std::move(descriptorSetLayoutBindings);
        return *this;
    }

    DescriptorSetLayouts DescriptorSetLayoutsBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("DescriptorSetLayoutsBuilder::build(): mDevice == nullptr");
        }

        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts;
        descriptorSetLayouts.reserve(mDescriptorSetLayoutBindings.size());

        for (std::vector<vk::DescriptorSetLayoutBinding> &bindings: mDescriptorSetLayoutBindings) {
            vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{};
            descriptorSetLayoutCreateInfo
                    .setBindings(bindings);

            vk::DescriptorSetLayout descriptorSetLayout = mDevice.createDescriptorSetLayout(descriptorSetLayoutCreateInfo);
            LOG_D("mDevice.createDescriptorSetLayout => %p", (void *) descriptorSetLayout);
            descriptorSetLayouts.push_back(descriptorSetLayout);
        }

        return DescriptorSetLayouts{mDevice, std::move(descriptorSetLayouts)};
    }

    std::unique_ptr<DescriptorSetLayouts> DescriptorSetLayoutsBuilder::buildUnique() {
        return std::make_unique<DescriptorSetLayouts>(build());
    }

} // vklite