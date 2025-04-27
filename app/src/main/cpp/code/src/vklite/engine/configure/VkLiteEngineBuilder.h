//
// Created by leixing on 2025/1/8.
//

#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "vklite/util/selector/StringListSelector.h"

#ifndef APPLICATION_NAME
#define APPLICATION_NAME "vklite_app"
#endif

#ifndef ENGINE_NAME
#define ENGINE_NAME "vklite"
#endif

#include "vulkan/vulkan.hpp"

#include "vklite/instance/Instance.h"
#include "vklite/configure/physical_device/VulkanPhysicalDeviceProvider.h"

#include "vklite/engine/VkLiteEngine.h"
#include "vklite/surface/SurfaceBuilder.h"
#include "vklite/configure/pipeline/GraphicsPipelineConfigure.h"
#include "vklite/configure/pipeline/ComputePipelineConfigure.h"
#include "vklite/pipeline_resource/configure/PipelineResourceConfigure.h"
#include "vklite/util/selector/Selector.h"

namespace vklite {

    class VkLiteEngineBuilder {

    private:
        std::unique_ptr<ListSelector<std::string>> mExtensionsSelector;
        std::unique_ptr<ListSelector<std::string>> mLayersSelector;

        std::string mApplicationName = APPLICATION_NAME;
        uint32_t mApplicationVersion = VK_MAKE_VERSION(1, 0, 0);

        std::string mEngineName = ENGINE_NAME;
        uint32_t mEngineVersion = VK_MAKE_VERSION(1, 0, 0);

        std::unique_ptr<SurfaceBuilder> mSurfaceBuilder;
        std::vector<std::string> mDeviceExtensions;

        uint32_t mFrameCount = 2;

        std::unique_ptr<VulkanPhysicalDeviceProvider> mVulkanPhysicalDeviceProvider;
        std::unique_ptr<ValueSelector<uint32_t>> mMsaaSelector;

        std::unique_ptr<GraphicsPipelineConfigure> mVulkanGraphicsPipelineConfigure;
        std::unique_ptr<ComputePipelineConfigure> mVulkanComputePipelineConfigure;

        std::unique_ptr<PipelineResourceConfigure> mPipelineResourceConfigure;

//        // shader
//        std::vector<char> mComputeShaderCode;
//        std::vector<char> mVertexShaderCode;
//        std::vector<char> mFragmentShaderCode;
//
//        std::vector<VulkanVertex> mVertices;
//        std::vector<VulkanDescriptorSet> mDescriptorSets;
//        std::vector<VulkanPushConstant> mPushConstants;

    public:

        VkLiteEngineBuilder();

        ~VkLiteEngineBuilder();

        // instance extensions
        VkLiteEngineBuilder &extensions(std::vector<std::string> &&required, std::vector<std::string> &&optional = {});

        VkLiteEngineBuilder &extensionsSelector(std::unique_ptr<ListSelector<std::string>> &selector);

        VkLiteEngineBuilder &extensionsSelector(std::function<std::vector<std::string>(const std::vector<std::string> &)> selector);

        // instance layers
        VkLiteEngineBuilder &layers(std::vector<std::string> &&required, std::vector<std::string> &&optional = {});

        VkLiteEngineBuilder &layersSelector(std::unique_ptr<ListSelector<std::string>> &selector);

        VkLiteEngineBuilder &layersSelector(std::function<std::vector<std::string>(const std::vector<std::string> &)> selector);

        // application
        VkLiteEngineBuilder &applicationName(const std::string &applicationName);

        VkLiteEngineBuilder &applicationVersion(uint32_t version);

        // version: "1.0" or "1.0.0"
        VkLiteEngineBuilder &applicationVersion(const std::string &version);


        // engine
        VkLiteEngineBuilder &engineName(const std::string &engineName);

        VkLiteEngineBuilder &engineVersion(uint32_t version);

        // version: "1.0" or "1.0.0"
        VkLiteEngineBuilder &engineVersion(const std::string &version);

        // surface
        VkLiteEngineBuilder &surfaceBuilder(std::unique_ptr<SurfaceBuilder> &&surfaceBuilder);

        // device extensions
        VkLiteEngineBuilder &deviceExtensions(std::vector<std::string> &&deviceExtensions);

        // frame count
        VkLiteEngineBuilder &frameCount(uint32_t frameCount);

        // physical device
        VkLiteEngineBuilder &physicalDeviceAsDefault();

        VkLiteEngineBuilder &physicalDevice(std::unique_ptr<VulkanPhysicalDevice> &&vulkanPhysicalDevice);

        VkLiteEngineBuilder &physicalDeviceProvider(std::unique_ptr<VulkanPhysicalDeviceProvider> &&provider);

        // msaa
        VkLiteEngineBuilder &enableMsaa();

        VkLiteEngineBuilder &enableMsaa(uint32_t msaaSamples);

        VkLiteEngineBuilder &enableMsaaMax();

        VkLiteEngineBuilder &enableMsaaMax(uint32_t msaaSamplesMax);

        VkLiteEngineBuilder &enableMsaa(const std::function<uint32_t(const std::vector<uint32_t> &)> &selector);

        // graphics pipeline
        VkLiteEngineBuilder &graphicsPipeline(const std::function<void(GraphicsPipelineConfigure &)> &configure);

        // compute pipeline
        VkLiteEngineBuilder &computePipeline(const std::function<void(ComputePipelineConfigure &)> &configure);

        std::unique_ptr<VkLiteEngine> build();

    private:

    };

} // vklite
