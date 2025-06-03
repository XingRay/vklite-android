//
// Created by leixing on 2025/6/4.
//

#include "SimpleGraphicEngineBuilder.h"
#include <utility>

namespace vklite {

    SimpleGraphicEngineBuilder::SimpleGraphicEngineBuilder()
            : mInstanceBuilder{}, mDeviceBuilder{} {}

    SimpleGraphicEngineBuilder::~SimpleGraphicEngineBuilder() = default;

    SimpleGraphicEngineBuilder::SimpleGraphicEngineBuilder(SimpleGraphicEngineBuilder &&other) noexcept
            : mFrameCount(std::exchange(other.mFrameCount, 0)), // 基本类型使用 exchange 重置
              mClearColor(std::move(other.mClearColor)),        // std::array 移动等同于拷贝
              mClearDepth(std::exchange(other.mClearDepth, 0.0f)),
              mMsaaEnable(std::exchange(other.mMsaaEnable, false)),
              mDepthTestEnable(std::exchange(other.mDepthTestEnable, false)),

              mGraphicShaderConfigure(std::move(other.mGraphicShaderConfigure)),
              mInstanceBuilder(std::move(other.mInstanceBuilder)),
              mSurfaceBuilder(std::move(other.mSurfaceBuilder)),
              mPhysicalDeviceSelector(std::move(other.mPhysicalDeviceSelector)),
              mSampleCountSelector(std::move(other.mSampleCountSelector)),
              mDeviceBuilder(std::move(other.mDeviceBuilder)) {}

    SimpleGraphicEngineBuilder &SimpleGraphicEngineBuilder::operator=(SimpleGraphicEngineBuilder &&other) noexcept {
        if (this != &other) {
            // 基本类型：直接赋值（移动等同于拷贝）
            mFrameCount = other.mFrameCount;
            mClearColor = other.mClearColor; // std::array 移动赋值等同于拷贝
            mClearDepth = other.mClearDepth;
            mMsaaEnable = other.mMsaaEnable;
            mDepthTestEnable = other.mDepthTestEnable;

            // 自定义类型：使用移动赋值
            mGraphicShaderConfigure = std::move(other.mGraphicShaderConfigure);
            mInstanceBuilder = std::move(other.mInstanceBuilder);
            mSurfaceBuilder = std::move(other.mSurfaceBuilder);
            mPhysicalDeviceSelector = std::move(other.mPhysicalDeviceSelector);
            mSampleCountSelector = std::move(other.mSampleCountSelector);
            mDeviceBuilder = std::move(other.mDeviceBuilder);
        }
        return *this;
    }

    SimpleGraphicEngineBuilder &SimpleGraphicEngineBuilder::addInstancePlugin(std::unique_ptr<PluginInterface> plugin) {
        mInstanceBuilder.addPlugin(std::move(plugin));
        return *this;
    }

    SimpleGraphicEngineBuilder &SimpleGraphicEngineBuilder::addDevicePlugin(std::unique_ptr<PluginInterface> plugin) {
        mDeviceBuilder.addPlugin(std::move(plugin));
        return *this;
    }

    SimpleGraphicEngineBuilder &SimpleGraphicEngineBuilder::surfaceBuilder(std::function<std::unique_ptr<Surface>(const Instance &Instance)> &&surfaceBuilder) {
        mSurfaceBuilder = std::move(surfaceBuilder);
        return *this;
    }

    SimpleGraphicEngineBuilder &SimpleGraphicEngineBuilder::physicalDeviceSelector(
            std::function<std::unique_ptr<PhysicalDevice>(const Instance &Instance, const Surface &surface)> &&physicalDeviceSelector) {
        mPhysicalDeviceSelector = std::move(physicalDeviceSelector);
        return *this;
    }

    SimpleGraphicEngineBuilder &SimpleGraphicEngineBuilder::sampleCountSelector(
            std::function<vk::SampleCountFlagBits(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits)> &&sampleCountSelector) {
        mSampleCountSelector = std::move(sampleCountSelector);
        return *this;
    }

    SimpleGraphicEngineBuilder &SimpleGraphicEngineBuilder::enableDepthTest() {
        mDepthTestEnable = true;
        return *this;
    }

    SimpleGraphicEngineBuilder &SimpleGraphicEngineBuilder::shaderConfigure(ShaderConfigure &&shaderConfigure) {
        mGraphicShaderConfigure = std::move(shaderConfigure);
        return *this;
    }

    SimpleGraphicEngine SimpleGraphicEngineBuilder::build() {

        std::unique_ptr<Instance> instance = mInstanceBuilder.buildUnique();
        std::unique_ptr<Surface> surface = mSurfaceBuilder(*instance);
        std::unique_ptr<PhysicalDevice> physicalDevice = mPhysicalDeviceSelector(*instance, *surface);

        vk::SampleCountFlagBits sampleCount = vk::SampleCountFlagBits::e1;
        if (mSampleCountSelector != nullptr) {
            sampleCount = mSampleCountSelector(physicalDevice->querySampleCountFlagBits());
        }
        if (sampleCount != vk::SampleCountFlagBits::e1) {
            mMsaaEnable = true;
        }

        uint32_t presentQueueFamilyIndex = physicalDevice->queryQueueFamilyIndicesBySurface(surface->getSurface())[0];
        uint32_t graphicQueueFamilyIndex = physicalDevice->queryQueueFamilyIndicesByFlags(vk::QueueFlagBits::eGraphics)[0];

        std::unique_ptr<Device> device = mDeviceBuilder
                .physicalDevice(physicalDevice->getPhysicalDevice())
                .addQueueFamily(graphicQueueFamilyIndex)
                .addQueueFamily(presentQueueFamilyIndex)
                .buildUnique();

        std::unique_ptr<Queue> graphicQueue = std::make_unique<Queue>(device->getQueue(graphicQueueFamilyIndex));
        std::unique_ptr<Queue> presentQueue = std::make_unique<Queue>(device->getQueue(presentQueueFamilyIndex));

        std::unique_ptr<Swapchain> swapchain = SwapchainBuilder()
                .device(device->getDevice())
                .surface(surface->getSurface())
                .queueFamilyIndices({presentQueueFamilyIndex})
                .config(physicalDevice->getPhysicalDevice(), surface->getSurface())
                .buildUnique();

        std::vector<vk::Viewport> viewports = swapchain->fullScreenViewports();
        std::vector<vk::Rect2D> scissors = swapchain->fullScreenScissors();

        std::unique_ptr<CommandPool> commandPool = CommandPoolBuilder()
                .device(device->getDevice())
                .queueFamilyIndex(graphicQueueFamilyIndex)
                .buildUnique();
        std::unique_ptr<CommandBuffers> commandBuffers = commandPool->allocateUnique(mFrameCount);

        // 创建附件
        std::vector<ImageView> displayImageViews = swapchain->createDisplayImageViews();

        std::unique_ptr<CombinedImageView> colorImageView = nullptr;
        if (mMsaaEnable) {
            colorImageView = CombinedImageViewBuilder().asColor()
                    .device(device->getDevice())
                    .format(swapchain->getDisplayFormat())
                    .size(swapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .configDeviceMemory(physicalDevice->getPhysicalDevice().getMemoryProperties())
                    .buildUnique();
        }

        std::unique_ptr<CombinedImageView> depthImageView = nullptr;
        if (mDepthTestEnable) {
            depthImageView = CombinedImageViewBuilder().asDepth()
                    .device(device->getDevice())
                    .format(physicalDevice->findDepthFormat())
                    .size(swapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .configDeviceMemory(physicalDevice->getPhysicalDevice().getMemoryProperties())
                    .buildUnique();
        }

        Subpass externalSubpass = Subpass::externalSubpass();
        std::unique_ptr<RenderPass> renderPass = RenderPassBuilder()
                .device(device->getDevice())
                .renderAreaExtend(swapchain->getDisplaySize())
                .addSubpass([&](Subpass &subpass, const std::vector<Subpass> &subpasses) {
                    subpass
                            .pipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                            .stageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
                            .accessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite)
                            .addDependency(externalSubpass);
                })
                .addAttachmentIf(mMsaaEnable, [&](Attachment &attachment, std::vector<Subpass> &subpasses) {
                    Attachment::msaaColorAttachment(attachment)
                            .sampleCount(sampleCount)
                            .format(swapchain->getDisplayFormat())
                            .clearColorValue(mClearColor)
                            .asColorAttachmentUsedIn(subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                })
                .addAttachment([&](Attachment &attachment, std::vector<Subpass> &subpasses) {
                    Attachment::presentColorAttachment(attachment)
                            .format(swapchain->getDisplayFormat())
                            .clearColorValue(mClearColor)
                            .applyIf(mMsaaEnable, [&](Attachment &thiz) {
                                thiz
                                        .loadOp(vk::AttachmentLoadOp::eDontCare)
                                        .asResolveAttachmentUsedIn(subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                            })
                            .applyIf(!mMsaaEnable, [&](Attachment &thiz) {
                                thiz.asColorAttachmentUsedIn(subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                            });
                })
                .addAttachmentIf(mDepthTestEnable, [&](Attachment &attachment, std::vector<Subpass> &subpasses) {
                    Attachment::depthStencilAttachment(attachment)
                            .sampleCount(sampleCount)
                            .clearDepthValue(mClearDepth)
                            .format(physicalDevice->findDepthFormat())
                            .asDepthStencilAttachmentUsedIn(subpasses[0], vk::ImageLayout::eDepthStencilAttachmentOptimal);
                })
                .buildUnique();

        Framebuffers framebuffers = FramebuffersBuilder()
                .count(displayImageViews.size())
                .framebufferBuilder([&](uint32_t index) {
                    return FramebufferBuilder()
                            .device(device->getDevice())
                            .renderPass(renderPass->getRenderPass())
                            .width(swapchain->getDisplaySize().width)
                            .height(swapchain->getDisplaySize().height)
                                    // 下面添加附件的顺序不能乱, 附件的顺序由 RenderPass 的附件定义顺序决定，必须严格一致。
                            .addAttachmentIf(mMsaaEnable, [&]() { return colorImageView->getImageView().getImageView(); })
                            .addAttachment(displayImageViews[index].getImageView())
                            .addAttachmentIf(mDepthTestEnable, [&]() { return depthImageView->getImageView().getImageView(); })
                            .build();
                })
                .build();

        std::vector<Semaphore> imageAvailableSemaphores = SemaphoreBuilder().device(device->getDevice()).build(mFrameCount);
        std::vector<Semaphore> renderFinishedSemaphores = SemaphoreBuilder().device(device->getDevice()).build(mFrameCount);
        std::vector<Fence> fences = FenceBuilder()
                .device(device->getDevice())
                        // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
                .fenceCreateFlags(vk::FenceCreateFlagBits::eSignaled)
                .build(mFrameCount);

        std::unique_ptr<DescriptorPool> graphicsDescriptorPool = DescriptorPoolBuilder()
                .device(device->getDevice())
                .frameCount(mFrameCount)
                .buildUnique();

        std::unique_ptr<PipelineLayout> graphicsPipelineLayout = PipelineLayoutBuilder()
                .device(device->getDevice())
                .buildUnique();

        std::unique_ptr<Pipeline> graphicsPipeline = GraphicsPipelineBuilder()
                .device(device->getDevice())
                .renderPass(renderPass->getRenderPass())
                .pipelineLayout(graphicsPipelineLayout->getPipelineLayout())
                .viewports(viewports)
                .scissors(scissors)
                .configShader(mGraphicShaderConfigure)
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .buildUnique();

        return {
                mFrameCount,
                std::move(instance),
                std::move(surface),
                std::move(physicalDevice),
                std::move(device),
                std::move(graphicQueue),
                std::move(presentQueue),
                std::move(swapchain),
                std::move(displayImageViews),
                std::move(viewports),
                std::move(scissors),
                std::move(commandPool),
                std::move(commandBuffers),
                std::move(renderPass),
                std::move(colorImageView),
                std::move(depthImageView),
                std::move(framebuffers),
                std::move(imageAvailableSemaphores),
                std::move(renderFinishedSemaphores),
                std::move(fences),
                std::move(graphicsDescriptorPool),
                std::move(graphicsPipelineLayout),
                std::move(graphicsPipeline)
        };
    }

    std::unique_ptr<SimpleGraphicEngine> SimpleGraphicEngineBuilder::buildUnique() {
        return std::make_unique<SimpleGraphicEngine>(build());
    }


    SimpleGraphicEngineBuilder &SimpleGraphicEngineBuilder::asDefault() {
        (*this).physicalDeviceSelector([](const vklite::Instance &instance, const vklite::Surface &surface) {
                    return vklite::PhysicalDeviceSelector::makeDefault(surface)
                            .selectUnique(instance.enumeratePhysicalDevices());
                })
                .sampleCountSelector([](const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits) {
                    return vklite::MaxMsaaSampleCountSelector(4).select(sampleCountFlagBits);
                });
        return *this;
    }

} // vklite