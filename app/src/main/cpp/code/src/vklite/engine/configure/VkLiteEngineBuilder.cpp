//
// Created by leixing on 2025/1/8.
//

#include "VkLiteEngineBuilder.h"
#include "vklite/util/StringUtil.h"

#include "vklite/device/Device.h"
#include "vklite/util/selector/Uint32Selector.h"

namespace vklite {

    VkLiteEngineBuilder::VkLiteEngineBuilder() = default;

    VkLiteEngineBuilder::~VkLiteEngineBuilder() = default;

    VkLiteEngineBuilder &VkLiteEngineBuilder::extensions(std::vector<std::string> &&required, std::vector<std::string> &&optional) {
        mExtensionsSelector = std::make_unique<RequiredAndOptionalStringListSelector>(std::move(required), std::move(optional));
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::extensionsSelector(std::unique_ptr<ListSelector<std::string>> &selector) {
        mExtensionsSelector = std::move(selector);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::extensionsSelector(std::function<std::vector<std::string>(const std::vector<std::string> &)> selector) {
        mExtensionsSelector = std::make_unique<LambdaStringListSelector>(selector);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::layers(std::vector<std::string> &&required, std::vector<std::string> &&optional) {
        mLayersSelector = std::make_unique<RequiredAndOptionalStringListSelector>(std::move(required), std::move(optional));
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::layersSelector(std::unique_ptr<ListSelector<std::string>> &selector) {
        mLayersSelector = std::move(selector);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::layersSelector(std::function<std::vector<std::string>(const std::vector<std::string> &)> selector) {
        mLayersSelector = std::make_unique<LambdaStringListSelector>(selector);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::applicationName(const std::string &applicationName) {
        mApplicationName = applicationName;
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::applicationVersion(uint32_t version) {
        mApplicationVersion = version;
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::applicationVersion(const std::string &version) {
        std::vector<uint32_t> versionNumbers = StringUtil::parseVersion(version);

        if (versionNumbers.size() < 2) {
            throw std::invalid_argument("Invalid version format: at least major and minor versions are required");
        }

        if (versionNumbers.size() == 2) {
            versionNumbers.push_back(0);
        }

        mApplicationVersion = VK_MAKE_VERSION(versionNumbers[0], versionNumbers[1], versionNumbers[2]);

        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::engineName(const std::string &engineName) {
        mEngineName = engineName;
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::engineVersion(uint32_t version) {
        mEngineVersion = version;
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::engineVersion(const std::string &version) {
        std::vector<uint32_t> versionNumbers = StringUtil::parseVersion(version);

        if (versionNumbers.size() < 2) {
            throw std::invalid_argument("Invalid version format: at least major and minor versions are required");
        }

        if (versionNumbers.size() == 2) {
            versionNumbers.push_back(0);
        }

        mEngineVersion = VK_MAKE_VERSION(versionNumbers[0], versionNumbers[1], versionNumbers[2]);

        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::surfaceBuilder(std::unique_ptr<SurfaceBuilder> &&surfaceBuilder) {
        mSurfaceBuilder = std::move(surfaceBuilder);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::deviceExtensions(std::vector<std::string> &&deviceExtensions) {
        mDeviceExtensions = std::move(deviceExtensions);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

//    VkLiteEngineBuilder &VkLiteEngineBuilder::physicalDeviceAsDefault() {
//        mVulkanPhysicalDeviceProvider = std::make_unique<DefaultVulkanPhysicalDeviceProvider>();
//        return *this;
//    }
//
//    VkLiteEngineBuilder &VkLiteEngineBuilder::physicalDevice(std::unique_ptr<PhysicalDevice> &&vulkanPhysicalDevice) {
//        mVulkanPhysicalDeviceProvider = std::make_unique<SimpleVulkanPhysicalDeviceSelector>(std::move(vulkanPhysicalDevice));
//        return *this;
//    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::physicalDeviceProvider(std::unique_ptr<PhysicalDeviceSelector> &&provider) {
        mVulkanPhysicalDeviceProvider = std::move(provider);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::enableMsaa() {
        mMsaaSelector = std::make_unique<MaxUint32Selector>(4);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::enableMsaa(uint32_t msaaSamples) {
        mMsaaSelector = std::make_unique<FixUint32Selector>(msaaSamples);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::enableMsaaMax() {
        mMsaaSelector = std::make_unique<MaxUint32Selector>();
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::enableMsaaMax(uint32_t msaaSamplesMax) {
        mMsaaSelector = std::make_unique<MaxUint32Selector>(msaaSamplesMax);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::enableMsaa(const std::function<uint32_t(const std::vector<uint32_t> &)> &selector) {
        mMsaaSelector = std::make_unique<LambdaUint32Selector>(selector);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::graphicsPipeline(const std::function<void(GraphicsPipelineConfigure &)> &configure) {
        mVulkanGraphicsPipelineConfigure = std::make_unique<GraphicsPipelineConfigure>();
        configure(*mVulkanGraphicsPipelineConfigure);
        return *this;
    }

    VkLiteEngineBuilder &VkLiteEngineBuilder::computePipeline(const std::function<void(ComputePipelineConfigure &)> &configure) {
        return *this;
    }

    std::unique_ptr<VkLiteEngine> VkLiteEngineBuilder::build() {
        // instance
        std::unique_ptr<Instance> instance = std::make_unique<Instance>(mApplicationName,
                                                                        mApplicationVersion,
                                                                        mEngineName,
                                                                        mEngineVersion,
                                                                        *mExtensionsSelector,
                                                                        *mLayersSelector);

        // surface
        std::unique_ptr<Surface> surface = mSurfaceBuilder->build(*instance);

        // select physical device
//        std::unique_ptr<PhysicalDeviceCandidate> candidate = std::move(mVulkanPhysicalDeviceProvider->select(*instance, *surface, mDeviceExtensions));
//        std::unique_ptr<PhysicalDevice> &vulkanPhysicalDevice = candidate->getPhysicalDevice();

        // logical device
        uint32_t sampleCount = 0;
        if (mMsaaSelector != nullptr) {
//            sampleCount = mMsaaSelector->select(vulkanPhysicalDevice->querySupportedSampleCounts());
        }
//        std::unique_ptr<VulkanDevice> vulkanDevice = std::make_unique<VulkanDevice>(*vulkanPhysicalDevice,
//                                                                                    candidate->getSurfaceSupport().value(),
//                                                                                    mDeviceExtensions,
//                                                                                    instance->getEnabledLayers(),
//                                                                                    sampleCount);

        // swapchain
//        vk::Extent2D currentExtent = vulkanDevice->getCapabilities().currentExtent;
//        LOG_D("currentExtent:[%d x %d]", currentExtent.width, currentExtent.height);
//        std::unique_ptr<VulkanSwapchain> swapchain = std::make_unique<VulkanSwapchain>(*vulkanDevice, *surface, currentExtent.width, currentExtent.height);
//
//        // command pool
//        std::unique_ptr<VulkanCommandPool> commandPool = std::make_unique<VulkanCommandPool>(*vulkanDevice, mFrameCount);
//
//        std::unique_ptr<VulkanRenderPass> renderPass = std::make_unique<VulkanRenderPass>(*vulkanDevice, *swapchain);
//
//        std::unique_ptr<GraphicsPipeline> vulkanGraphicsPipeline = nullptr;
//        if (mVulkanGraphicsPipelineConfigure != nullptr) {
//            LOG_D("create VulkanGraphicsPipeline");
//            vulkanGraphicsPipeline = mVulkanGraphicsPipelineConfigure->build(*vulkanDevice, *swapchain, *renderPass);
//        }
//
//        std::unique_ptr<ComputePipeline> vulkanComputePipeline = nullptr;
//        if (mVulkanComputePipelineConfigure != nullptr) {
//            LOG_D("create VulkanGraphicsPipeline");
//            vulkanComputePipeline = mVulkanComputePipelineConfigure->build();//std::make_unique<VulkanGraphicsPipeline>(*vulkanDevice, *swapchain, *renderPass, *vulkanShader);
//        }
//
//        std::vector<PipelineResource> pipelineResources = mPipelineResourceConfigure->createPipelineResources(mFrameCount);
//
//        LOG_D("create VulkanFrameBuffer");
//        std::unique_ptr<VulkanFrameBuffer> frameBuffer = std::make_unique<VulkanFrameBuffer>(*vulkanDevice, *swapchain, *renderPass, *commandPool);
//        LOG_D("create VulkanSyncObject");
//        std::unique_ptr<VulkanSyncObject> syncObject = std::make_unique<VulkanSyncObject>(*vulkanDevice, mFrameCount);
//
//        return std::make_unique<VkLiteEngine>(std::move(instance),
//                                              std::move(surface),
//                                              std::move(vulkanPhysicalDevice),
//                                              std::move(vulkanDevice),
//                                              std::move(commandPool),
//                                              std::move(swapchain),
//                                              std::move(renderPass),
//                                              std::move(vulkanGraphicsPipeline),
//                                              std::move(vulkanComputePipeline),
//                                              std::move(pipelineResources),
//                                              std::move(frameBuffer),
//                                              std::move(syncObject),
//                                              mFrameCount);

        return nullptr;
    }

} // vklite