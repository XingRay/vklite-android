//
// Created by leixing on 2025/3/11.
//
#include <iterator>

#include "PipelineLayoutBuilder.h"

#include "vklite/device/Device.h"
#include "vklite/Log.h"

namespace vklite {

    PipelineLayoutBuilder::PipelineLayoutBuilder() {
        mPipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo{};
    };

    PipelineLayoutBuilder::~PipelineLayoutBuilder() = default;

    PipelineLayoutBuilder &PipelineLayoutBuilder::device(vk::Device device) {
        mDevice = device;
        return *this;
    }

    PipelineLayoutBuilder &PipelineLayoutBuilder::pushConstantRanges(std::vector<vk::PushConstantRange> pushConstantRanges) {
        mPushConstantRanges = std::move(pushConstantRanges);
        mPipelineLayoutCreateInfo.setPushConstantRanges(mPushConstantRanges);
        return *this;
    }

    PipelineLayoutBuilder &PipelineLayoutBuilder::addPushConstant(uint32_t offset, uint32_t size, vk::ShaderStageFlags stageFlags) {
        vk::PushConstantRange pushConstantRange(stageFlags, offset, size);
        mPushConstantRanges.push_back(pushConstantRange);
        mPipelineLayoutCreateInfo.setPushConstantRanges(mPushConstantRanges);
        return *this;
    }

    PipelineLayoutBuilder &PipelineLayoutBuilder::descriptorSetLayouts(const std::vector<vk::DescriptorSetLayout> &descriptorSetLayouts) {
        mPipelineLayoutCreateInfo.setSetLayouts(descriptorSetLayouts);
        return *this;
    }

//    PipelineLayoutBuilder &PipelineLayoutBuilder::descriptorSetLayouts(std::vector<vk::DescriptorSetLayout> &&descriptorSetLayouts) {
//        mDescriptorSetLayouts = std::move(descriptorSetLayouts);
//        return *this;
//    }

//    PipelineLayoutBuilder &PipelineLayoutBuilder::config(const ShaderConfigure &shaderConfigure) {
//        std::vector<vk::DescriptorSetLayout> descriptorSetLayouts = shaderConfigure.createDescriptorSetLayouts(mDevice);
//        (*this).descriptorSetLayouts(std::move(descriptorSetLayouts));
//        return *this;
//    }

    PipelineLayout PipelineLayoutBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("mDevice == nullptr, must invoke PipelineLayoutBuilder::device(vk::Device device)");
        }

        vk::PipelineLayout pipelineLayout = mDevice.createPipelineLayout(mPipelineLayoutCreateInfo);
        LOG_D("mDevice.createPipelineLayout => %p", (void *) pipelineLayout);

//        return PipelineLayout(mDevice, pipelineLayout);
        return PipelineLayout{mDevice, pipelineLayout};
    }

    std::unique_ptr<PipelineLayout> PipelineLayoutBuilder::buildUnique() {
        return std::make_unique<PipelineLayout>(build());
    }

} // vklite