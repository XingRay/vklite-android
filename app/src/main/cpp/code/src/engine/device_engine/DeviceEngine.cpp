//
// Created by leixing on 2025/6/8.
//

#include "DeviceEngine.h"

namespace vklite {

    DeviceEngine::DeviceEngine(
            uint32_t frameCount,
            bool depthTestEnable,
            vk::SampleCountFlagBits sampleCount,
            std::unique_ptr <Instance> instance,
            std::unique_ptr <Surface> surface,
            std::unique_ptr <PhysicalDevice> physicalDevice,
            std::unique_ptr <Device> device,
            std::unique_ptr <Queue> graphicQueue,
            std::unique_ptr <Queue> presentQueue,
            std::unique_ptr <Swapchain> swapchain,
            std::vector <ImageView> &&displayImageViews,
            std::vector <vk::Viewport> &&viewports,
            std::vector <vk::Rect2D> &&scissors,
            std::unique_ptr <CommandPool> commandPool,
            std::unique_ptr <CommandBuffers> commandBuffers,
            std::unique_ptr <RenderPass> renderPass,
            std::unique_ptr <CombinedImageView> colorImageView,
            std::unique_ptr <CombinedImageView> depthImageView,
            Framebuffers &&framebuffers,
            std::vector <Semaphore> &&imageAvailableSemaphores,
            std::vector <Semaphore> &&renderFinishedSemaphores,
            std::vector <Fence> &&fences)
            : mFrameCount(frameCount),
              mDepthTestEnable(depthTestEnable),
              mSampleCount(sampleCount),
              mInstance(std::move(instance)),
              mSurface(std::move(surface)),
              mPhysicalDevice(std::move(physicalDevice)),
              mDevice(std::move(device)),
              mGraphicQueue(std::move(graphicQueue)),
              mPresentQueue(std::move(presentQueue)),
              mSwapchain(std::move(swapchain)),
              mDisplayImageViews(std::move(displayImageViews)),
              mViewports(std::move(viewports)),
              mScissors(std::move(scissors)),
              mCommandPool(std::move(commandPool)),
              mCommandBuffers(std::move(commandBuffers)),
              mRenderPass(std::move(renderPass)),
              mColorImageView(std::move(colorImageView)),
              mDepthImageView(std::move(depthImageView)),
              mFramebuffers(std::move(framebuffers)),
              mImageAvailableSemaphores(std::move(imageAvailableSemaphores)),
              mRenderFinishedSemaphores(std::move(renderFinishedSemaphores)),
              mFences(std::move(fences)),
              mIndexCount(0) {}

    DeviceEngine::~DeviceEngine() = default;

    DeviceEngine::DeviceEngine(DeviceEngine &&other) noexcept
            : mFrameCount(other.mFrameCount),
              mDepthTestEnable(other.mDepthTestEnable),
              mSampleCount(other.mSampleCount),
              mInstance(std::move(other.mInstance)),
              mSurface(std::move(other.mSurface)),
              mPhysicalDevice(std::move(other.mPhysicalDevice)),
              mDevice(std::move(other.mDevice)),
              mGraphicQueue(std::move(other.mGraphicQueue)),
              mPresentQueue(std::move(other.mPresentQueue)),
              mSwapchain(std::move(other.mSwapchain)),
              mDisplayImageViews(std::move(other.mDisplayImageViews)),
              mViewports(std::move(other.mViewports)),
              mScissors(std::move(other.mScissors)),
              mCommandPool(std::move(other.mCommandPool)),
              mCommandBuffers(std::move(other.mCommandBuffers)),
              mRenderPass(std::move(other.mRenderPass)),
              mColorImageView(std::move(other.mColorImageView)),
              mDepthImageView(std::move(other.mDepthImageView)),
              mFramebuffers(std::move(other.mFramebuffers)),
              mImageAvailableSemaphores(std::move(other.mImageAvailableSemaphores)),
              mRenderFinishedSemaphores(std::move(other.mRenderFinishedSemaphores)),
              mFences(std::move(other.mFences)),
              mPipelineLayout(std::move(other.mPipelineLayout)),
              mDescriptorPool(std::move(other.mDescriptorPool)),
              mDescriptorSetLayouts(std::move(other.mDescriptorSetLayouts)),
              mDescriptorSets(std::move(other.mDescriptorSets)),
              mPushConstants(std::move(other.mPushConstants)),
              mPipeline(std::move(other.mPipeline)),
              mIndexCount(other.mIndexCount) {}

    DeviceEngine &DeviceEngine::operator=(DeviceEngine &&other) noexcept {
        if (this != &other) {
            mFrameCount = other.mFrameCount;
            mDepthTestEnable = other.mDepthTestEnable;
            mSampleCount = other.mSampleCount;
            // 移动赋值：逐个成员进行移动赋值
            mInstance = std::move(other.mInstance);
            mSurface = std::move(other.mSurface);
            mPhysicalDevice = std::move(other.mPhysicalDevice);
            mDevice = std::move(other.mDevice);
            mGraphicQueue = std::move(other.mGraphicQueue);
            mPresentQueue = std::move(other.mPresentQueue);
            mSwapchain = std::move(other.mSwapchain);
            mDisplayImageViews = std::move(other.mDisplayImageViews);
            mViewports = std::move(other.mViewports);
            mScissors = std::move(other.mScissors);
            mCommandPool = std::move(other.mCommandPool);
            mCommandBuffers = std::move(other.mCommandBuffers);
            mRenderPass = std::move(other.mRenderPass);
            mColorImageView = std::move(other.mColorImageView);
            mDepthImageView = std::move(other.mDepthImageView);
            mFramebuffers = std::move(other.mFramebuffers);
            mImageAvailableSemaphores = std::move(other.mImageAvailableSemaphores);
            mRenderFinishedSemaphores = std::move(other.mRenderFinishedSemaphores);
            mFences = std::move(other.mFences);
            mPipelineLayout = std::move(other.mPipelineLayout);
            mDescriptorPool = std::move(other.mDescriptorPool);
            mDescriptorSetLayouts = std::move(other.mDescriptorSetLayouts);
            mDescriptorSets = std::move(other.mDescriptorSets);
            mPushConstants = std::move(other.mPushConstants);
            mPipeline = std::move(other.mPipeline);
            mIndexCount = other.mIndexCount;
        }
        return *this;
    }

    Device &DeviceEngine::getDevice() const {
        return *mDevice;
    }

    vk::Device DeviceEngine::getVkDevice() const {
        return (*mDevice).getDevice();
    }

    PhysicalDevice &DeviceEngine::getPhysicalDevice() const {
        return *mPhysicalDevice;
    }

    vk::PhysicalDeviceMemoryProperties DeviceEngine::getMemoryProperties() const {
        return mPhysicalDevice->getPhysicalDevice().getMemoryProperties();
    }

    CommandPool &DeviceEngine::getCommandPool() const {
        return *mCommandPool;
    }

    [[nodiscard]]
    const std::vector <std::vector<vk::DescriptorSet>> &DeviceEngine::getDescriptorSets() const {
        return mDescriptorSets;
    }

    [[nodiscard]]
    const vk::DescriptorSet &DeviceEngine::getDescriptorSets(uint32_t frameIndex, uint32_t set) const {
        return mDescriptorSets[frameIndex][set];
    }

    uint32_t DeviceEngine::getFrameIndex() const {
        return mCurrentFrameIndex;
    }

    uint32_t DeviceEngine::getFrameCount() const {
        return mFrameCount;
    }


    DeviceEngine &DeviceEngine::shaderConfigure(ShaderConfigure &shaderConfigure) {
        std::vector <vk::PushConstantRange> pushConstantRanges = shaderConfigure.getPushConstantRanges();
        mPushConstants.reserve(pushConstantRanges.size());
        for (const vk::PushConstantRange &pushConstantRange: pushConstantRanges) {
            mPushConstants.emplace_back(pushConstantRange.size, pushConstantRange.offset, pushConstantRange.stageFlags);
        }

        mDescriptorPool = vklite::DescriptorPoolBuilder()
                .device(mDevice->getDevice())
                .frameCount(mFrameCount)
                .descriptorPoolSizes(shaderConfigure.calcDescriptorPoolSizes())
                .descriptorSetCount(shaderConfigure.getDescriptorSetCount())
                .buildUnique();

        mDescriptorSetLayouts = DescriptorSetLayoutsBuilder()
                .device(mDevice->getDevice())
                .bindings(shaderConfigure.createDescriptorSetLayoutBindings())
                .buildUnique();

        mDescriptorSets.reserve(mFrameCount);
        for (uint32_t i = 0; i < mFrameCount; i++) {
            std::vector <vk::DescriptorSet> sets = mDescriptorPool->allocateDescriptorSets(mDescriptorSetLayouts->getDescriptorSetLayouts());
            LOG_D("descriptorPool->allocateDescriptorSets:");
            for (const vk::DescriptorSet &set: sets) {
                LOG_D("\tset:%p", (void *) set);
            }
            mDescriptorSets.push_back(std::move(sets));
        }

        mPipelineLayout = PipelineLayoutBuilder()
                .device(mDevice->getDevice())
                .descriptorSetLayouts(mDescriptorSetLayouts->getDescriptorSetLayouts())
                .pushConstantRanges(std::move(pushConstantRanges))
                .buildUnique();

        std::unique_ptr <ShaderModule> vertexShader = ShaderModuleBuilder()
                .device(mDevice->getDevice())
                .code(std::move(shaderConfigure.getVertexShaderCode()))
                .buildUnique();

        std::unique_ptr <ShaderModule> fragmentShader = ShaderModuleBuilder()
                .device(mDevice->getDevice())
                .code(std::move(shaderConfigure.getFragmentShaderCode()))
                .buildUnique();

        mPipeline = GraphicsPipelineBuilder()
                .device(mDevice->getDevice())
                .renderPass(mRenderPass->getRenderPass())
                .pipelineLayout(mPipelineLayout->getPipelineLayout())
                .viewports(mViewports)
                .scissors(mScissors)
                .vertexShader(std::move(vertexShader))
                .vertexBindingDescriptions(shaderConfigure.createVertexBindingDescriptions())
                .vertexAttributeDescriptions(shaderConfigure.createVertexAttributeDescriptions())
                .fragmentShader(std::move(fragmentShader))
                .sampleCount(mSampleCount)
                .depthTestEnable(mDepthTestEnable)
                .buildUnique();

        return *this;
    }


    DeviceEngine &DeviceEngine::updateDescriptorSets(std::function<void(uint32_t frameIndex, DescriptorSetMappingConfigure & mappingConfigure)> &&configure) {
        std::vector <DescriptorSetWriter> descriptorSetWriters = DescriptorSetWriterBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure(std::move(configure))
                .build();

        std::vector <vk::WriteDescriptorSet> writeDescriptorSets;
        for (DescriptorSetWriter &descriptorSetWriter: descriptorSetWriters) {
            std::vector <vk::WriteDescriptorSet> descriptorSets = descriptorSetWriter.createWriteDescriptorSets();
            writeDescriptorSets.insert(writeDescriptorSets.begin(), std::move_iterator(descriptorSets.begin()), std::move_iterator(descriptorSets.end()));
        }

        LOG_D("mDevice->getDevice().updateDescriptorSets: %ld", writeDescriptorSets.size());
        for (const vk::WriteDescriptorSet &writeDescriptorSet: writeDescriptorSets) {
            LOG_D("\twriteDescriptorSet:");
            LOG_D("\t\tdstSet:%p", (void *) writeDescriptorSet.dstSet);
            LOG_D("\t\tdstBinding:%d", writeDescriptorSet.dstBinding);
            LOG_D("\t\tdstArrayElement:%d", writeDescriptorSet.dstArrayElement);
            LOG_D("\t\tdescriptorType:%d", writeDescriptorSet.descriptorType);
            LOG_D("\t\tdescriptorCount:%d", writeDescriptorSet.descriptorCount);
            if (writeDescriptorSet.pBufferInfo != nullptr) {
                for (uint32_t i = 0; i < writeDescriptorSet.descriptorCount; i++) {
                    const vk::DescriptorBufferInfo &bufferInfo = writeDescriptorSet.pBufferInfo[i];
                    LOG_D("\t\tbufferInfo:");
                    LOG_D("\t\t\tbuffer:%p", (void *) bufferInfo.buffer);
                    LOG_D("\t\t\toffset:%ld", bufferInfo.offset);
                    LOG_D("\t\t\trange:%ld", bufferInfo.range);
                }
            }
            if (writeDescriptorSet.pImageInfo != nullptr) {
                for (uint32_t i = 0; i < writeDescriptorSet.descriptorCount; i++) {
                    const vk::DescriptorImageInfo &imageInfo = writeDescriptorSet.pImageInfo[i];
                    LOG_D("\t\timageInfo:");
                    LOG_D("\t\t\timageView:%p", (void *) imageInfo.imageView);
                    LOG_D("\t\t\tsampler:%p", (void *) imageInfo.sampler);
                    LOG_D("\t\t\timageLayout:%p", (void *) imageInfo.imageLayout);
                }
            }
        }

        LOG_D("mDevice->getDevice().updateDescriptorSets");
        mDevice->getDevice().updateDescriptorSets(writeDescriptorSets, nullptr);
        LOG_D("mDevice->getDevice().updateDescriptorSets ok");

        return *this;
    }

    VertexBufferBuilder DeviceEngine::vertexBufferBuilder() {
        return VertexBufferBuilder()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties());
    }

    DeviceEngine &DeviceEngine::addVertexBuffer(const vk::Buffer &buffer, vk::DeviceSize offset) {
        mVertexBuffers.push_back(buffer);
        mVertexBufferOffsets.push_back(offset);
        return *this;
    }

    DeviceEngine &DeviceEngine::addVertexBuffer(const VertexBuffer &buffer, vk::DeviceSize offset) {
        addVertexBuffer(buffer.getVkBuffer(), offset);
        return *this;
    }


    IndexBufferBuilder DeviceEngine::indexBufferBuilder() {
        return IndexBufferBuilder()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties());
    }

    DeviceEngine &DeviceEngine::indexBuffer(const vk::Buffer &buffer, uint32_t indexCount) {
        mIndexBuffer = buffer;
        mIndexCount = indexCount;
        return *this;
    }

    DeviceEngine &DeviceEngine::indexBuffer(const IndexBuffer &buffer, uint32_t indexCount) {
        mIndexBuffer = buffer.getVkBuffer();
        mIndexCount = indexCount;
        return *this;
    }

    DeviceEngine &DeviceEngine::updatePushConstant(uint32_t index, const void *data, uint32_t size) {
        mPushConstants[index].update(data, size);
        return *this;
    }

    void DeviceEngine::drawIndexed() {
        Semaphore &imageAvailableSemaphore = mImageAvailableSemaphores[mCurrentFrameIndex];
        Semaphore &renderFinishedSemaphore = mRenderFinishedSemaphores[mCurrentFrameIndex];
        Fence &fence = mFences[mCurrentFrameIndex];

        vk::Result result = mFences[mCurrentFrameIndex].wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
        auto [acquireResult, imageIndex] = mSwapchain->acquireNextImage(imageAvailableSemaphore.getSemaphore());
        if (acquireResult != vk::Result::eSuccess) {
            if (acquireResult == vk::Result::eErrorOutOfDateKHR) {
                // 交换链已与表面不兼容，不能再用于渲染。通常在窗口大小调整后发生。
                LOG_E("acquireNextImageKHR: eErrorOutOfDateKHR, recreateSwapChain");
//                recreateSwapChain();
                return;
            } else if (acquireResult == vk::Result::eSuboptimalKHR) {
                //vk::Result::eSuboptimalKHR 交换链仍然可以成功显示到表面，但表面属性不再完全匹配。
                LOG_D("acquireNextImageKHR: eSuboptimalKHR");
            } else {
                LOG_E("acquireNextImageKHR: failed: %d", acquireResult);
                throw std::runtime_error("acquireNextImageKHR failed");
            }
        }

        const PooledCommandBuffer &commandBuffer = (*mCommandBuffers)[mCurrentFrameIndex];
        commandBuffer.record([&](const vk::CommandBuffer &vkCommandBuffer) {
            mRenderPass->execute(vkCommandBuffer, mFramebuffers[imageIndex].getFramebuffer(), [&](const vk::CommandBuffer &vkCommandBuffer) {
                vkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline->getPipeline());
                vkCommandBuffer.setViewport(0, mViewports);
                vkCommandBuffer.setScissor(0, mScissors);

                if (!mDescriptorSets.empty()) {
                    vkCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mPipelineLayout->getPipelineLayout(), 0, mDescriptorSets[mCurrentFrameIndex], nullptr);
                }

                for (const PushConstant &pushConstant: mPushConstants) {
                    vkCommandBuffer.pushConstants(mPipelineLayout->getPipelineLayout(),
                                                  pushConstant.getStageFlags(),
                                                  pushConstant.getOffset(),
                                                  pushConstant.getSize(),
                                                  pushConstant.getData());
                }

                vkCommandBuffer.bindVertexBuffers(0, mVertexBuffers, mVertexBufferOffsets);
                vkCommandBuffer.bindIndexBuffer(mIndexBuffer, 0, vk::IndexType::eUint32);
                vkCommandBuffer.drawIndexed(mIndexCount, 1, 0, 0, 0);
            });
        });

        result = mFences[mCurrentFrameIndex].reset();
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("resetFences failed");
        }

        mGraphicQueue->submit(commandBuffer.getCommandBuffer(),
                              vk::PipelineStageFlagBits::eColorAttachmentOutput,
                              imageAvailableSemaphore.getSemaphore(),
                              renderFinishedSemaphore.getSemaphore(),
                              fence.getFence());

        result = mPresentQueue->present(mSwapchain->getSwapChain(), imageIndex, renderFinishedSemaphore.getSemaphore());
        if (result != vk::Result::eSuccess) {
            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mFramebufferResized) {
                mFramebufferResized = false;
                LOG_E("presentKHR: eErrorOutOfDateKHR or eSuboptimalKHR or mFramebufferResized, recreateSwapChain");
                // todo: recreateSwapChain
//                recreateSwapChain();
                return;
            } else {
                throw std::runtime_error("presentKHR failed");
            }
        }

        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mFrameCount;
    }

    UniformBufferBuilder DeviceEngine::uniformBufferBuilder() {
        return UniformBufferBuilder()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties());
    }

    StorageBufferBuilder DeviceEngine::storageBufferBuilder() {
        return StorageBufferBuilder()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties());
    }

    StagingBufferBuilder DeviceEngine::stagingBufferBuilder() {
        return StagingBufferBuilder()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties());
    }

    CombinedImageSamplerBuilder DeviceEngine::samplerBuilder() {
        return CombinedImageSamplerBuilder().asDefault()
                .device(mDevice->getDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getPhysicalDevice().getMemoryProperties());
    }

} // vklite