//
// Created by leixing on 2025/1/4.
//

#include "Test01SimpleTriangle.h"
#include "FileUtil.h"
#include "vklite/engine/configure/VkLiteEngineBuilder.h"
#include "vklite/instance/InstanceBuilder.h"
#include "vklite/platform/android/surface/AndroidSurfaceBuilder.h"
#include "vklite/physical_device/PhysicalDeviceSelector.h"
#include "vklite/util/selector/Uint32Selector.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/device/DeviceBuilder.h"

namespace test01 {

    Test01SimpleTriangle::Test01SimpleTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        LOG_D("Test01SimpleTriangle::Test01SimpleTriangle");

        std::vector<std::string> instanceExtensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,

                // old version
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                // new version
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };

        std::vector<std::string> layers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<std::string> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::vector<char> vertexShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/01_triangle.vert.spv");
        std::vector<char> fragmentShaderCode = FileUtil::loadFile(mApp.activity->assetManager, "shaders/01_triangle.frag.spv");

        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

        mInstance = vklite::InstanceBuilder()
                .extensions({}, std::move(instanceExtensions))
                .layers({}, std::move(layers))
                .build();
        mSurface = vklite::AndroidSurfaceBuilder(mApp.window).build(*mInstance);
        mPhysicalDevice = vklite::PhysicalDeviceSelector::makeDefault(*mSurface)->select(mInstance->listPhysicalDevices());

//        mPhysicalDevice->querySurfaceSupport(*mSurface, vk::QueueFlagBits::eGraphics);
        uint32_t sampleCount = vklite::MaxUint32Selector(4).select(mPhysicalDevice->querySupportedSampleCounts());
        vk::SampleCountFlagBits mMsaaSamples = vklite::VulkanUtil::uint32ToSampleCountFlagBits(sampleCount);

//        mDevice = vklite::DeviceBuilder().addDevicePlugin().build();


//        mVkLiteEngine = vklite::VkLiteEngineBuilder{}
//                .layers({}, std::move(layers))
//                .extensions({}, std::move(instanceExtensions))
//                .deviceExtensions(std::move(deviceExtensions))
//                .surfaceBuilder(std::make_unique<vklite::AndroidVulkanSurfaceBuilder>(mApp.window))
//                .enableMsaa()
//                .physicalDeviceAsDefault()
//                .graphicsPipeline([&](vklite::GraphicsPipelineConfigure &graphicsPipelineConfigure) {
//                    graphicsPipelineConfigure
//                            .vertexShaderCode(std::move(vertexShaderCode))
//                            .fragmentShaderCode(std::move(fragmentShaderCode))
//                            .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
//                                vertexBindingConfigure
//                                        .binding(0)
//                                        .stride(sizeof(Vertex))
//                                        .addAttribute(0, ShaderFormat::Vec3);
//                            });
//                })
////                .pipelineResource([&](){
////
////                })
//                .build();



    }

    void Test01SimpleTriangle::init() {
//        mVkLiteEngine->getGraphicsPipeline()
//                .createVertexBuffer(vertices.size() * sizeof(Vertex))
//                .updateVertexBuffer(mVkLiteEngine->getVulkanCommandPool(), vertices)
//                .createIndexBuffer(indices.size() * sizeof(uint32_t))
//                .updateIndexBuffer(mVkLiteEngine->getVulkanCommandPool(), indices);

//        (*mVkLiteEngine)
//                .createVertexBuffer(vertices.size() * sizeof(Vertex))
//                .updateVertexBuffer(vertices)
//                .createIndexBuffer(indices.size() * sizeof(uint32_t))
//                .updateIndexBuffer(indices);

    }

    // 检查是否准备好
    bool Test01SimpleTriangle::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test01SimpleTriangle::drawFrame() {
//        mVkLiteEngine->drawFrame();
    }

    // 清理操作
    void Test01SimpleTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
//        mVkLiteEngine.reset();
    }

} // test