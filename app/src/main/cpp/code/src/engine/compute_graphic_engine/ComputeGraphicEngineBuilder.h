//
// Created by leixing on 2025/6/9.
//

#pragma once

#include <memory>
#include <cstdint>
#include <functional>

#include "vklite/vklite.h"

#include "engine/compute_graphic_engine/ComputeGraphicEngine.h"

namespace vklite {

    class ComputeGraphicEngineBuilder {
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

        ShaderConfigure mComputeShaderConfigure;

    public:
        ComputeGraphicEngineBuilder();

        ~ComputeGraphicEngineBuilder();

        ComputeGraphicEngineBuilder(const ComputeGraphicEngineBuilder &other) = delete;

        ComputeGraphicEngineBuilder &operator=(ComputeGraphicEngineBuilder &other) = delete;

        ComputeGraphicEngineBuilder(ComputeGraphicEngineBuilder &&other) noexcept;

        ComputeGraphicEngineBuilder &operator=(ComputeGraphicEngineBuilder &&other) noexcept;

        ComputeGraphicEngineBuilder &frameCount(uint32_t frameCount);

        ComputeGraphicEngineBuilder &clearColor(const std::array<float, 4> &clearColor);

        ComputeGraphicEngineBuilder &clearColor(float r, float g, float b, float a);

        ComputeGraphicEngineBuilder &clearColor(float r, float g, float b);

        ComputeGraphicEngineBuilder &clearDepth(float clearDepth);

        ComputeGraphicEngineBuilder &addInstancePlugin(std::unique_ptr<PluginInterface> plugin);

        ComputeGraphicEngineBuilder &addDevicePlugin(std::unique_ptr<PluginInterface> plugin);

        ComputeGraphicEngineBuilder &surfaceBuilder(std::function<std::unique_ptr<Surface>(const Instance &Instance)> &&surfaceBuilder);

        ComputeGraphicEngineBuilder &physicalDeviceSelector(std::function<std::unique_ptr<PhysicalDevice>(const Instance &Instance, const Surface &surface)> &&physicalDeviceSelector);

        ComputeGraphicEngineBuilder &sampleCountSelector(std::function<vk::SampleCountFlagBits(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits)> &&sampleCountSelector);

        ComputeGraphicEngineBuilder &enableDepthTest();

        ComputeGraphicEngineBuilder &graphicShaderConfigure(ShaderConfigure &&shaderConfigure);

        ComputeGraphicEngineBuilder &computeShaderConfigure(ShaderConfigure &&shaderConfigure);

        [[nodiscard]]
        ComputeGraphicEngine build();

        [[nodiscard]]
        std::unique_ptr<ComputeGraphicEngine> buildUnique();


        /**
         * preset
         */
        ComputeGraphicEngineBuilder &asDefault();
    };

} // vklite
