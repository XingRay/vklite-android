//
// Created by leixing on 2025/6/9.
//

#include "ComputeGraphicEngine.h"

namespace vklite {

    ComputeGraphicEngine::ComputeGraphicEngine(
            uint32_t frameCount,
            vk::SampleCountFlagBits sampleCount,
            std::unique_ptr<Instance> instance,
            std::unique_ptr<Surface> surface,
            std::unique_ptr<PhysicalDevice> physicalDevice,
            std::unique_ptr<Device> device,
            std::unique_ptr<Queue> graphicQueue,
            std::unique_ptr<Queue> presentQueue,
            std::unique_ptr<Swapchain> swapchain,
            std::vector<ImageView> &&displayImageViews,
            std::vector<vk::Viewport> &&viewports,
            std::vector<vk::Rect2D> &&scissors,
            std::unique_ptr<CommandPool> commandPool,
            std::unique_ptr<CommandBuffers> commandBuffers,
            std::unique_ptr<RenderPass> renderPass,
            std::unique_ptr<CombinedImageView> colorImageView,
            std::unique_ptr<CombinedImageView> depthImageView,
            Framebuffers &&framebuffers,
            std::vector<Semaphore> &&imageAvailableSemaphores,
            std::vector<Semaphore> &&renderFinishedSemaphores,
            std::vector<Fence> &&fences,

            std::unique_ptr<DescriptorPool> descriptorPool,
            std::unique_ptr<CombinedPipeline> graphicPipeline,

            std::unique_ptr<Queue> computeQueue,
            std::unique_ptr<CommandBuffers> computeCommandBuffers,
            std::vector<Fence> &&computeFences,
            std::vector<Semaphore> &&computeFinishSemaphores,

            std::unique_ptr<CombinedPipeline> computePipeline)
            : mFrameCount(frameCount),
              mSampleCount(sampleCount),
              mIndexCount(0),

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
              mGraphicCommandBuffers(std::move(commandBuffers)),
              mRenderPass(std::move(renderPass)),
              mColorImageView(std::move(colorImageView)),
              mDepthImageView(std::move(depthImageView)),
              mFramebuffers(std::move(framebuffers)),
              mGraphicImageAvailableSemaphores(std::move(imageAvailableSemaphores)),
              mGraphicRenderFinishedSemaphores(std::move(renderFinishedSemaphores)),
              mGraphicFences(std::move(fences)),
              mDescriptorPool(std::move(descriptorPool)),
              mGraphicPipeline(std::move(graphicPipeline)),

              mComputeQueue(std::move(computeQueue)),
              mComputeCommandBuffers(std::move(computeCommandBuffers)),
              mComputeFences(std::move(computeFences)),
              mComputeFinishSemaphores(std::move(computeFinishSemaphores)),

              mComputePipeline(std::move(computePipeline)) {}

    ComputeGraphicEngine::~ComputeGraphicEngine() = default;

    ComputeGraphicEngine::ComputeGraphicEngine(ComputeGraphicEngine &&other) noexcept
            : mFrameCount(other.mFrameCount),
              mSampleCount(other.mSampleCount),
              mIndexCount(other.mIndexCount),

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
              mGraphicCommandBuffers(std::move(other.mGraphicCommandBuffers)),
              mRenderPass(std::move(other.mRenderPass)),
              mColorImageView(std::move(other.mColorImageView)),
              mDepthImageView(std::move(other.mDepthImageView)),
              mFramebuffers(std::move(other.mFramebuffers)),
              mGraphicImageAvailableSemaphores(std::move(other.mGraphicImageAvailableSemaphores)),
              mGraphicRenderFinishedSemaphores(std::move(other.mGraphicRenderFinishedSemaphores)),
              mGraphicFences(std::move(other.mGraphicFences)),
              mDescriptorPool(std::move(other.mDescriptorPool)),
              mGraphicPipeline(std::move(other.mGraphicPipeline)),

              mComputeQueue(std::move(other.mComputeQueue)),
              mComputeCommandBuffers(std::move(other.mComputeCommandBuffers)),
              mComputeFences(std::move(other.mComputeFences)),
              mComputeFinishSemaphores(std::move(other.mComputeFinishSemaphores)),

              mComputePipeline(std::move(other.mComputePipeline)) {}

    ComputeGraphicEngine &ComputeGraphicEngine::operator=(ComputeGraphicEngine &&other) noexcept {
        if (this != &other) {
            mFrameCount = other.mFrameCount;
            mSampleCount = other.mSampleCount;
            mIndexCount = other.mIndexCount;

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
            mGraphicCommandBuffers = std::move(other.mGraphicCommandBuffers);
            mRenderPass = std::move(other.mRenderPass);
            mColorImageView = std::move(other.mColorImageView);
            mDepthImageView = std::move(other.mDepthImageView);
            mFramebuffers = std::move(other.mFramebuffers);
            mGraphicImageAvailableSemaphores = std::move(other.mGraphicImageAvailableSemaphores);
            mGraphicRenderFinishedSemaphores = std::move(other.mGraphicRenderFinishedSemaphores);
            mGraphicFences = std::move(other.mGraphicFences);
            mDescriptorPool = std::move(other.mDescriptorPool);
            mGraphicPipeline = std::move(other.mGraphicPipeline);

            mComputeQueue = std::move(other.mComputeQueue);
            mComputeCommandBuffers = std::move(other.mComputeCommandBuffers);
            mComputeFences = std::move(other.mComputeFences);
            mComputeFinishSemaphores = std::move(other.mComputeFinishSemaphores);

            mComputePipeline = std::move(other.mComputePipeline);
        }
        return *this;
    }

    Device &ComputeGraphicEngine::getDevice() const {
        return *mDevice;
    }

    PhysicalDevice &ComputeGraphicEngine::getPhysicalDevice() const {
        return *mPhysicalDevice;
    }

    CommandPool &ComputeGraphicEngine::getCommandPool() const {
        return *mCommandPool;
    }

    const std::vector<std::vector<vk::DescriptorSet>> &ComputeGraphicEngine::getDescriptorSets() const {
        return mGraphicPipeline->getDescriptorSets();
    }

    const vk::DescriptorSet &ComputeGraphicEngine::getDescriptorSets(uint32_t frameIndex, uint32_t set) const {
        return mGraphicPipeline->getDescriptorSet(frameIndex, set);
    }

    const std::vector<std::vector<vk::DescriptorSet>> &ComputeGraphicEngine::getComputeDescriptorSets() const {
        return mComputePipeline->getDescriptorSets();
    }

    const vk::DescriptorSet &ComputeGraphicEngine::getComputeDescriptorSets(uint32_t frameIndex, uint32_t set) const {
        return mComputePipeline->getDescriptorSet(frameIndex, set);
    }

    uint32_t ComputeGraphicEngine::getFrameIndex() const {
        return mCurrentFrameIndex;
    }

    uint32_t ComputeGraphicEngine::getFrameCount() const {
        return mFrameCount;
    }

    ComputeGraphicEngine &ComputeGraphicEngine::updateDescriptorSets(std::function<void(uint32_t frameIndex, DescriptorSetMappingConfigure &configure)> &&configure) {
        DescriptorSetWriters descriptorSetWriters = DescriptorSetWritersBuilder()
                .frameCount(mFrameCount)
                .descriptorSetMappingConfigure(std::move(configure))
                .build();

        std::vector<vk::WriteDescriptorSet> writeDescriptorSets = descriptorSetWriters.createWriteDescriptorSets();

        bool debugLog = false;
        if (debugLog) {
            LOG_D("mDevice->getVkDevice().updateDescriptorSets: %ld", writeDescriptorSets.size());
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
        }

        mDevice->getVkDevice().updateDescriptorSets(writeDescriptorSets, nullptr);

        return *this;
    }

    VertexBufferBuilder ComputeGraphicEngine::vertexBufferBuilder() {
        return VertexBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

    ComputeGraphicEngine &ComputeGraphicEngine::addVertexBuffers(const std::function<void(uint32_t frameIndex, std::vector<vk::Buffer> &buffers, std::vector<vk::DeviceSize> &offsets)> &configure) {
        mVertexBuffers.reserve(mFrameCount);
        mVertexBufferOffsets.reserve(mFrameCount);
        for (uint32_t i = 0; i < mFrameCount; i++) {
            mVertexBuffers.emplace_back();
            mVertexBufferOffsets.emplace_back();
            configure(i, mVertexBuffers[i], mVertexBufferOffsets[i]);
        }

        return *this;
    }


    IndexBufferBuilder ComputeGraphicEngine::indexBufferBuilder() {
        return IndexBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

    ComputeGraphicEngine &ComputeGraphicEngine::indexBuffer(const vk::Buffer &buffer, uint32_t indexCount) {
        mIndexBuffer = buffer;
        mIndexCount = indexCount;
        return *this;
    }

    ComputeGraphicEngine &ComputeGraphicEngine::indexBuffer(const IndexBuffer &buffer, uint32_t indexCount) {
        mIndexBuffer = buffer.getVkBuffer();
        mIndexCount = indexCount;
        return *this;
    }

    ComputeGraphicEngine &ComputeGraphicEngine::updatePushConstant(uint32_t index, const void *data, uint32_t size) {
        mGraphicPipeline->getPushConstants()[index].update(data, size);
        return *this;
    }

    void ComputeGraphicEngine::drawIndexed(uint32_t particleCount) {

        // compute pipeline
        vk::Result result = mComputeFences[mCurrentFrameIndex].wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        result = mComputeFences[mCurrentFrameIndex].reset();
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("mComputeFences[mCurrentFrameIndex] resetFences failed");
        }

        const vklite::PooledCommandBuffer &computeCommandBuffer = (*mComputeCommandBuffers)[mCurrentFrameIndex];
        computeCommandBuffer.record([&](const vk::CommandBuffer &commandBuffer) {
            commandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, mComputePipeline->getVkPipeline());
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, mComputePipeline->getVkPipelineLayout(), 0, mComputePipeline->getDescriptorSets(mCurrentFrameIndex), nullptr);

            commandBuffer.dispatch(particleCount / 256, 1, 1);
        });

        mComputeQueue->submit(computeCommandBuffer.getVkCommandBuffer(),
                              mComputeFinishSemaphores[mCurrentFrameIndex].getVkSemaphore(),
                              mComputeFences[mCurrentFrameIndex].getVkFence());

        result = mComputeFences[mCurrentFrameIndex].wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }


        Semaphore &imageAvailableSemaphore = mGraphicImageAvailableSemaphores[mCurrentFrameIndex];
        Semaphore &renderFinishedSemaphore = mGraphicRenderFinishedSemaphores[mCurrentFrameIndex];
        Fence &fence = mGraphicFences[mCurrentFrameIndex];

        result = mGraphicFences[mCurrentFrameIndex].wait();
        if (result != vk::Result::eSuccess) {
            LOG_E("waitForFences failed");
            throw std::runtime_error("waitForFences failed");
        }

        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
        auto [acquireResult, imageIndex] = mSwapchain->acquireNextImage(imageAvailableSemaphore.getVkSemaphore());
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

        const PooledCommandBuffer &commandBuffer = (*mGraphicCommandBuffers)[mCurrentFrameIndex];
        commandBuffer.record([&](const vk::CommandBuffer &vkCommandBuffer) {
            mRenderPass->execute(vkCommandBuffer, mFramebuffers[imageIndex].getVkFramebuffer(), [&](const vk::CommandBuffer &vkCommandBuffer) {
                vkCommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mGraphicPipeline->getVkPipeline());
                vkCommandBuffer.setViewport(0, mViewports);
                vkCommandBuffer.setScissor(0, mScissors);

                if (!mGraphicPipeline->getDescriptorSets().empty()) {
                    vkCommandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, mGraphicPipeline->getVkPipelineLayout(), 0, mGraphicPipeline->getDescriptorSets(mCurrentFrameIndex), nullptr);
                }

                for (const PushConstant &pushConstant: mGraphicPipeline->getPushConstants()) {
                    vkCommandBuffer.pushConstants(mGraphicPipeline->getVkPipelineLayout(),
                                                  pushConstant.getStageFlags(),
                                                  pushConstant.getOffset(),
                                                  pushConstant.getSize(),
                                                  pushConstant.getData());
                }

                vkCommandBuffer.bindVertexBuffers(0, mVertexBuffers[mCurrentFrameIndex], mVertexBufferOffsets[mCurrentFrameIndex]);
                vkCommandBuffer.draw(particleCount, 1, 0, 0);
            });
        });

        result = mGraphicFences[mCurrentFrameIndex].reset();
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("resetFences failed");
        }

        mGraphicQueue->submit(commandBuffer.getVkCommandBuffer(),
                              vk::PipelineStageFlagBits::eColorAttachmentOutput,
                              imageAvailableSemaphore.getVkSemaphore(),
                              renderFinishedSemaphore.getVkSemaphore(),
                              fence.getVkFence());

        result = mPresentQueue->present(mSwapchain->getVkSwapChain(), imageIndex, renderFinishedSemaphore.getVkSemaphore());
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

    UniformBufferBuilder ComputeGraphicEngine::uniformBufferBuilder() {
        return UniformBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

    StorageBufferBuilder ComputeGraphicEngine::storageBufferBuilder() {
        return StorageBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

    StagingBufferBuilder ComputeGraphicEngine::stagingBufferBuilder() {
        return StagingBufferBuilder()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

    CombinedImageSamplerBuilder ComputeGraphicEngine::samplerBuilder() {
        return CombinedImageSamplerBuilder().asDefault()
                .device(mDevice->getVkDevice())
                .physicalDeviceMemoryProperties(mPhysicalDevice->getMemoryProperties());
    }

} // vklite