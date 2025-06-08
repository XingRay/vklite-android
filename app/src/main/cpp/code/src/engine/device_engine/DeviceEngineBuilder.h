//
// Created by leixing on 2025/6/8.
//

#pragma once

#include <memory>
#include <cstdint>
#include <functional>

#include "vklite/vklite.h"
#include "engine/device_engine/DeviceEngine.h"

namespace vklite {

    class DeviceEngineBuilder {
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
        DeviceEngineBuilder();

        ~DeviceEngineBuilder();

        DeviceEngineBuilder(const DeviceEngineBuilder &other) = delete;

        DeviceEngineBuilder &operator=(DeviceEngineBuilder &other) = delete;

        DeviceEngineBuilder(DeviceEngineBuilder &&other) noexcept;

        DeviceEngineBuilder &operator=(DeviceEngineBuilder &&other) noexcept;

        DeviceEngineBuilder &frameCount(uint32_t frameCount);

        DeviceEngineBuilder &clearColor(const std::array<float, 4> &clearColor);

        DeviceEngineBuilder &clearColor(float r, float g, float b, float a);

        DeviceEngineBuilder &clearColor(float r, float g, float b);

        DeviceEngineBuilder &clearDepth(float clearDepth);

        DeviceEngineBuilder &addInstancePlugin(std::unique_ptr<PluginInterface> plugin);

        DeviceEngineBuilder &addDevicePlugin(std::unique_ptr<PluginInterface> plugin);

        DeviceEngineBuilder &surfaceBuilder(std::function<std::unique_ptr<Surface>(const Instance &Instance)> &&surfaceBuilder);

        DeviceEngineBuilder &physicalDeviceSelector(std::function<std::unique_ptr<PhysicalDevice>(const Instance &Instance, const Surface &surface)> &&physicalDeviceSelector);

        DeviceEngineBuilder &sampleCountSelector(std::function<vk::SampleCountFlagBits(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits)> &&sampleCountSelector);

        DeviceEngineBuilder &enableDepthTest();

        DeviceEngineBuilder &shaderConfigure(ShaderConfigure &&shaderConfigure);

        [[nodiscard]]
        DeviceEngine build();

        [[nodiscard]]
        std::unique_ptr<DeviceEngine> buildUnique();


        /**
         * preset
         */
        DeviceEngineBuilder &asDefault();

    };

} // vklite
