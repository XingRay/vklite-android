//
// Created by leixing on 2025/1/4.
//

#include "Test01SimpleTriangle.h"
#include "FileUtil.h"
#include "vklite/platform/android/surface/AndroidSurfaceBuilder.h"
#include "vklite/physical_device/msaa/MaxMsaaSampleCountSelector.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/platform/android/device/AndroidDevicePlugin.h"

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

        std::vector<std::string> instanceLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::vector<const char *> deviceLayers = {
                "VK_LAYER_KHRONOS_validation"
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
                .layers({}, std::move(instanceLayers))
                .build();
        mSurface = vklite::AndroidSurfaceBuilder(mApp.window).build(*mInstance);
        mPhysicalDevice = vklite::PhysicalDeviceSelector::makeDefault(*mSurface)->select(mInstance->listPhysicalDevices());

//        mPhysicalDevice->querySurfaceSupport(*mSurface, vk::QueueFlagBits::eGraphics);
        vk::SampleCountFlagBits mMsaaSamples = mPhysicalDevice->selectMaxMsaaSampleCountFlagBits(4);
        vklite::QueueFamilyIndices queueFamilyIndices = mPhysicalDevice->queryQueueFamilies(mSurface->getSurface(), vk::QueueFlagBits::eGraphics);

        vk::SurfaceCapabilitiesKHR mSurfaceCapabilities;
        std::vector<vk::SurfaceFormatKHR> mSurfaceFormats;
        std::vector<vk::PresentModeKHR> mPresentModes;

        mDevice = vklite::DeviceBuilder()
                .extensions(std::move(deviceExtensions))
                .layers(std::move(deviceLayers))
                .addGraphicQueueIndex(queueFamilyIndices.graphicQueueFamilyIndex.value())
                .addPresentQueueIndex(queueFamilyIndices.presentQueueFamilyIndex.value())
                .addDevicePlugin(std::make_unique<vklite::AndroidDevicePlugin>())
                .build(*mPhysicalDevice);

        vk::Extent2D currentExtent = mPhysicalDevice->getCapabilities(*mSurface).currentExtent;
//        mSwapchain = std::make_unique<vklite::Swapchain>(*mDevice, *mSurface, currentExtent.width, currentExtent.height);

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
//                .updateVertexBuffer(mVkLiteEngine->getCommandPool(), vertices)
//                .createIndexBuffer(indices.size() * sizeof(uint32_t))
//                .updateIndexBuffer(mVkLiteEngine->getCommandPool(), indices);

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