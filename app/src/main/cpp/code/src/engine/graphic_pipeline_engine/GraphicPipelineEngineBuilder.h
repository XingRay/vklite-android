//
// Created by leixing on 2025/6/4.
//

#pragma once

#include <memory>
#include <cstdint>
#include <functional>

#include "vklite/vklite.h"

#include "GraphicPipelineEngine.h"

namespace vklite {

    class GraphicPipelineEngineBuilder {
    private:
        uint32_t mFrameCount = 2;
        std::array<float, 4> mClearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        float mClearDepth = 1.0f;
        bool mMsaaEnable = false;
        bool mDepthTestEnable = false;
        ShaderConfigure mGraphicShaderConfigure;

        InstanceBuilder mInstanceBuilder;
        std::function<std::unique_ptr<Surface>(const Instance &Instance)> mSurfaceBuilder;
        std::function<std::unique_ptr<PhysicalDevice>(const Instance &Instance, const Surface &surface)> mPhysicalDeviceSelector;
        std::function<vk::SampleCountFlagBits(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits)> mSampleCountSelector;
        DeviceBuilder mDeviceBuilder;

    public:
        GraphicPipelineEngineBuilder();

        ~GraphicPipelineEngineBuilder();

        GraphicPipelineEngineBuilder(const GraphicPipelineEngineBuilder &other) = delete;

        GraphicPipelineEngineBuilder &operator=(GraphicPipelineEngineBuilder &other) = delete;

        GraphicPipelineEngineBuilder(GraphicPipelineEngineBuilder &&other) noexcept;

        GraphicPipelineEngineBuilder &operator=(GraphicPipelineEngineBuilder &&other) noexcept;

        GraphicPipelineEngineBuilder &frameCount(uint32_t frameCount);

        GraphicPipelineEngineBuilder &clearColor(const std::array<float, 4> &clearColor);

        GraphicPipelineEngineBuilder &clearColor(float r, float g, float b, float a);

        GraphicPipelineEngineBuilder &clearColor(float r, float g, float b);

        GraphicPipelineEngineBuilder &clearDepth(float clearDepth);

        GraphicPipelineEngineBuilder &addInstancePlugin(std::unique_ptr<PluginInterface> plugin);

        GraphicPipelineEngineBuilder &addDevicePlugin(std::unique_ptr<PluginInterface> plugin);

        GraphicPipelineEngineBuilder &surfaceBuilder(std::function<std::unique_ptr<Surface>(const Instance &Instance)> &&surfaceBuilder);

        GraphicPipelineEngineBuilder &physicalDeviceSelector(std::function<std::unique_ptr<PhysicalDevice>(const Instance &Instance, const Surface &surface)> &&physicalDeviceSelector);

        GraphicPipelineEngineBuilder &sampleCountSelector(std::function<vk::SampleCountFlagBits(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits)> &&sampleCountSelector);

        GraphicPipelineEngineBuilder &enableDepthTest();

        GraphicPipelineEngineBuilder &shaderConfigure(ShaderConfigure &&shaderConfigure);

        [[nodiscard]]
        GraphicPipelineEngine build();

        [[nodiscard]]
        std::unique_ptr<GraphicPipelineEngine> buildUnique();


        /**
         * preset
         */
        GraphicPipelineEngineBuilder &asDefault();
    };

} // vklite
