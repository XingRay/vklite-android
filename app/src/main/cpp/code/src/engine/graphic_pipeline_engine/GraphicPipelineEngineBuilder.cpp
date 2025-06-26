//
// Created by leixing on 2025/6/4.
//

#include "GraphicPipelineEngineBuilder.h"
#include <utility>

#include "vklite/pipeline/descriptor_set_layout/DescriptorSetLayoutsBuilder.h"

namespace vklite {

    GraphicPipelineEngineBuilder::GraphicPipelineEngineBuilder()
            : mInstanceBuilder{}, mDeviceBuilder{} {}

    GraphicPipelineEngineBuilder::~GraphicPipelineEngineBuilder() = default;

    GraphicPipelineEngineBuilder::GraphicPipelineEngineBuilder(GraphicPipelineEngineBuilder &&other) noexcept
            : mFrameCount(std::exchange(other.mFrameCount, 0)),
              mClearColor(other.mClearColor),
              mClearDepth(std::exchange(other.mClearDepth, 0.0f)),
              mMsaaEnable(std::exchange(other.mMsaaEnable, false)),
              mDepthTestEnable(std::exchange(other.mDepthTestEnable, false)),

              mGraphicShaderConfigure(std::move(other.mGraphicShaderConfigure)),
              mInstanceBuilder(std::move(other.mInstanceBuilder)),
              mSurfaceBuilder(std::move(other.mSurfaceBuilder)),
              mPhysicalDeviceSelector(std::move(other.mPhysicalDeviceSelector)),
              mSampleCountSelector(std::move(other.mSampleCountSelector)),
              mDeviceBuilder(std::move(other.mDeviceBuilder)) {}

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::operator=(GraphicPipelineEngineBuilder &&other) noexcept {
        if (this != &other) {
            // 基本类型：直接赋值（移动等同于拷贝）
            mFrameCount = other.mFrameCount;
            mClearColor = other.mClearColor;
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

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::clearColor(const std::array<float, 4> &clearColor) {
        mClearColor = clearColor;
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::clearColor(float r, float g, float b, float a) {
        clearColor(std::array<float, 4>{r, g, b, a});
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::clearColor(float r, float g, float b) {
        clearColor(r, g, b, 1.0f);
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::clearDepth(float clearDepth) {
        mClearDepth = clearDepth;
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::addInstancePlugin(std::unique_ptr<PluginInterface> plugin) {
        mInstanceBuilder.addPlugin(std::move(plugin));
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::addDevicePlugin(std::unique_ptr<PluginInterface> plugin) {
        mDeviceBuilder.addPlugin(std::move(plugin));
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::surfaceBuilder(std::function<std::unique_ptr<Surface>(const Instance &Instance)> &&surfaceBuilder) {
        mSurfaceBuilder = std::move(surfaceBuilder);
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::physicalDeviceSelector(
            std::function<std::unique_ptr<PhysicalDevice>(const Instance &Instance, const Surface &surface)> &&physicalDeviceSelector) {
        mPhysicalDeviceSelector = std::move(physicalDeviceSelector);
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::sampleCountSelector(
            std::function<vk::SampleCountFlagBits(const std::vector<vk::SampleCountFlagBits> &sampleCountFlagBits)> &&sampleCountSelector) {
        mSampleCountSelector = std::move(sampleCountSelector);
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::enableDepthTest() {
        mDepthTestEnable = true;
        return *this;
    }

    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::shaderConfigure(ShaderConfigure &&shaderConfigure) {
        mGraphicShaderConfigure = std::move(shaderConfigure);
        return *this;
    }

    GraphicPipelineEngine GraphicPipelineEngineBuilder::build() {

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

        uint32_t presentQueueFamilyIndex = physicalDevice->queryQueueFamilyIndicesBySurface(surface->getVkSurface())[0];
        uint32_t graphicQueueFamilyIndex = physicalDevice->queryQueueFamilyIndicesByFlags(vk::QueueFlagBits::eGraphics)[0];

        std::unique_ptr<Device> device = mDeviceBuilder
                .physicalDevice(physicalDevice->getVkPhysicalDevice())
                .addQueueFamily(graphicQueueFamilyIndex)
                .addQueueFamily(presentQueueFamilyIndex)
                .buildUnique();
        LOG_D("device => %p", (void *) device->getVkDevice());

        std::unique_ptr<Queue> graphicQueue = std::make_unique<Queue>(device->getQueue(graphicQueueFamilyIndex));
        std::unique_ptr<Queue> presentQueue = std::make_unique<Queue>(device->getQueue(presentQueueFamilyIndex));

        std::unique_ptr<Swapchain> swapchain = SwapchainBuilder()
                .device(device->getVkDevice())
                .surface(surface->getVkSurface())
                .queueFamilyIndices({presentQueueFamilyIndex})
                .config(physicalDevice->getVkPhysicalDevice(), surface->getVkSurface())
                .buildUnique();

//        std::vector<vk::Viewport> viewports = swapchain->fullScreenViewports();
//        std::vector<vk::Rect2D> scissors = swapchain->fullScreenScissors();
        std::vector<vk::Viewport> viewports = swapchain->centerSquareViewports();
        std::vector<vk::Rect2D> scissors = swapchain->centerSquareScissors();

        std::unique_ptr<CommandPool> commandPool = CommandPoolBuilder()
                .device(device->getVkDevice())
                .queueFamilyIndex(graphicQueueFamilyIndex)
                .buildUnique();
        std::unique_ptr<CommandBuffers> commandBuffers = commandPool->allocateUnique(mFrameCount);

        // 创建附件
        std::vector<ImageView> displayImageViews = swapchain->createDisplayImageViews();

        std::unique_ptr<CombinedImageView> colorImageView = nullptr;
        if (mMsaaEnable) {
            colorImageView = CombinedImageViewBuilder().asColorAttachment()
                    .device(device->getVkDevice())
                    .format(swapchain->getVkFormat())
                    .size(swapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .physicalDeviceMemoryProperties(physicalDevice->getMemoryProperties())
                    .buildUnique();
        }

        std::unique_ptr<CombinedImageView> depthImageView = nullptr;
        if (mDepthTestEnable) {
            depthImageView = CombinedImageViewBuilder().asDepthAttachment()
                    .device(device->getVkDevice())
                    .format(physicalDevice->findDepthFormat())
                    .size(swapchain->getDisplaySize())
                    .sampleCount(sampleCount)
                    .physicalDeviceMemoryProperties(physicalDevice->getMemoryProperties())
                    .buildUnique();
        }

        Subpass externalSubpass = Subpass::externalSubpass();
        std::unique_ptr<RenderPass> renderPass = RenderPassBuilder()
                .device(device->getVkDevice())
                .renderAreaExtend(swapchain->getDisplaySize())
                .addSubpass([&](Subpass &subpass, const std::vector<Subpass> &subpasses) {
                    subpass
                            .pipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                            .addDependency(externalSubpass,
                                           vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                           vk::AccessFlagBits::eNone,
                                           vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                           vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);
                })
                .addAttachmentIf(mMsaaEnable, [&](Attachment &attachment, std::vector<Subpass> &subpasses) {
                    Attachment::msaaColorAttachment(attachment)
                            .sampleCount(sampleCount)
                            .format(swapchain->getVkFormat())
                            .clearColorValue(mClearColor)
                            .asColorAttachmentUsedIn(subpasses[0], vk::ImageLayout::eColorAttachmentOptimal);
                })
                .addAttachment([&](Attachment &attachment, std::vector<Subpass> &subpasses) {
                    Attachment::presentColorAttachment(attachment)
                            .format(swapchain->getVkFormat())
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
                            .device(device->getVkDevice())
                            .renderPass(renderPass->getVkRenderPass())
                            .width(swapchain->getDisplaySize().width)
                            .height(swapchain->getDisplaySize().height)
                                    // 下面添加附件的顺序不能乱, 附件的顺序由 RenderPass 的附件定义顺序决定，必须严格一致。
                            .addAttachmentIf(mMsaaEnable, [&]() { return colorImageView->getVkImageView(); })
                            .addAttachment(displayImageViews[index].getVkImageView())
                            .addAttachmentIf(mDepthTestEnable, [&]() { return depthImageView->getVkImageView(); })
                            .build();
                })
                .build();

        std::vector<Semaphore> imageAvailableSemaphores = SemaphoreBuilder().device(device->getVkDevice()).build(mFrameCount);
        std::vector<Semaphore> renderFinishedSemaphores = SemaphoreBuilder().device(device->getVkDevice()).build(mFrameCount);
        std::vector<Fence> fences = FenceBuilder()
                .device(device->getVkDevice())
                        // 已发出信号的状态下创建栅栏，以便第一次调用 vkWaitForFences()立即返回
                .fenceCreateFlags(vk::FenceCreateFlagBits::eSignaled)
                .build(mFrameCount);

        std::vector<vk::PushConstantRange> pushConstantRanges = mGraphicShaderConfigure.getPushConstantRanges();
        std::vector<PushConstant> pushConstants;
        pushConstants.reserve(pushConstantRanges.size());
        for (const vk::PushConstantRange &pushConstantRange: pushConstantRanges) {
            pushConstants.emplace_back(pushConstantRange.size, pushConstantRange.offset, pushConstantRange.stageFlags);
        }

        std::unique_ptr<DescriptorPool> descriptorPool = vklite::DescriptorPoolBuilder()
                .device(device->getVkDevice())
                .frameCount(mFrameCount)
                .descriptorPoolSizes(mGraphicShaderConfigure.calcDescriptorPoolSizes())
                .descriptorSetCount(mGraphicShaderConfigure.getDescriptorSetCount())
                .buildUnique();

        DescriptorSetLayouts descriptorSetLayouts = DescriptorSetLayoutsBuilder()
                .device(device->getVkDevice())
                .bindings(mGraphicShaderConfigure.createDescriptorSetLayoutBindings())
                .build();

        std::vector<std::vector<vk::DescriptorSet>> descriptorSets;
        descriptorSets.reserve(mFrameCount);
        for (uint32_t i = 0; i < mFrameCount; i++) {
            std::vector<vk::DescriptorSet> sets = descriptorPool->allocateDescriptorSets(descriptorSetLayouts.getDescriptorSetLayouts());
            LOG_D("descriptorPool->allocateDescriptorSets:");
            for (const vk::DescriptorSet &set: sets) {
                LOG_D("\tset:%p", (void *) set);
            }
            descriptorSets.push_back(std::move(sets));
        }

        std::unique_ptr<PipelineLayout> pipelineLayout = PipelineLayoutBuilder()
                .device(device->getVkDevice())
                .descriptorSetLayouts(descriptorSetLayouts.getDescriptorSetLayouts())
                .pushConstantRanges(std::move(pushConstantRanges))
                .buildUnique();

        std::unique_ptr<ShaderModule> vertexShader = ShaderModuleBuilder()
                .device(device->getVkDevice())
                .code(std::move(mGraphicShaderConfigure.getVertexShaderCode()))
                .buildUnique();

        std::unique_ptr<ShaderModule> fragmentShader = ShaderModuleBuilder()
                .device(device->getVkDevice())
                .code(std::move(mGraphicShaderConfigure.getFragmentShaderCode()))
                .buildUnique();

        std::unique_ptr<Pipeline> pipeline = GraphicsPipelineBuilder()
                .device(device->getVkDevice())
                .renderPass(renderPass->getVkRenderPass())
                .pipelineLayout(pipelineLayout->getVkPipelineLayout())
                .viewports(viewports)
                .scissors(scissors)
                .vertexShader(std::move(vertexShader))
                .vertexBindingDescriptions(mGraphicShaderConfigure.createVertexBindingDescriptions())
                .vertexAttributeDescriptions(mGraphicShaderConfigure.createVertexAttributeDescriptions())
                .fragmentShader(std::move(fragmentShader))
                .sampleCount(sampleCount)
                .depthTestEnable(mDepthTestEnable)
                .buildUnique();

        return {mFrameCount,
                sampleCount,
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
                std::move(pipelineLayout),
                std::move(descriptorPool),
                std::move(descriptorSetLayouts),
                std::move(descriptorSets),
                std::move(pushConstants),
                std::move(pipeline)
        };
    }

    std::unique_ptr<GraphicPipelineEngine> GraphicPipelineEngineBuilder::buildUnique() {
        return std::make_unique<GraphicPipelineEngine>(build());
    }


    GraphicPipelineEngineBuilder &GraphicPipelineEngineBuilder::asDefault() {
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