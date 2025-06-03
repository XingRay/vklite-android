//
// Created by leixing on 2025/1/4.
//

#include "Test02SingleColorTriangle.h"

#include "util/FileUtil.h"
#include "vklite/vklite_android.h"

namespace test02 {

//    Test02SingleColorTriangle::Test02SingleColorTriangle(const android_app &app, const std::string &name)
//            : TestBase(name), mApp(app) {
//
//        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/02_triangle_color.vert.spv");
//        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/02_triangle_color.frag.spv");
//
//        vklite::ShaderConfigure graphicShaderConfigure = vklite::ShaderConfigure()
//                .vertexShaderCode(std::move(vertexShaderCode))
//                .fragmentShaderCode(std::move(fragmentShaderCode))
//                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
//                    vertexBindingConfigure
//                            .binding(0)
//                            .stride(sizeof(Vertex))
//                            .addAttribute(0, ShaderFormat::Vec3);
//                })
//                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
//                    descriptorSetConfigure
//                            .set(0)
//                            .addUniform([&](vklite::UniformConfigure &uniformConfigure) {
//                                uniformConfigure
//                                        .binding(0)
//                                        .shaderStageFlags(vk::ShaderStageFlagBits::eVertex);
//                            });
//                });
//
//        LOG_D("test created ");
//    }
//
//    void Test02SingleColorTriangle::init() {
//        std::vector<Vertex> vertices = {
//                {{1.0f,  -1.0f, 0.0f}},
//                {{-1.0f, -1.0f, 0.0f}},
//                {{0.0f,  1.0f,  0.0f}},
//        };
//
//        std::vector<uint32_t> indices = {0, 1, 2};
//
//        ColorUniformBufferObject colorUniformBufferObject{{0.8f, 0.2f, 0.4f}};
//
//
//        mIndexBuffer = vklite::IndexBufferBuilder()
//                .bufferSize(indices.size() * sizeof(uint32_t))
//                .build(*mPhysicalDevice, *mDevice);
//        mIndexBuffer->update(*mCommandPool, indices);
//
//        mVertexBuffer = vklite::VertexBufferBuilder()
//                .bufferSize(vertices.size() * sizeof(Vertex))
//                .build(*mPhysicalDevice, *mDevice);
//        mVertexBuffer->update(*mCommandPool, vertices);
//
//        mPipelineResources = vklite::PipelineResourcesBuilder()
//                .frameCount(mFrameCount)
//                .pipelineResourceBuilder([&](uint32_t frameIndex) {
//                    return vklite::PipelineResourceBuilder()
//                            .addVertexBuffer(*mVertexBuffer)
//                            .indexBuffer(*mIndexBuffer)
//                            .indicesCount(indices.size())
//                            .descriptorSets(mDescriptorPool->allocateDescriptorSets(descriptorSetLayouts))
//                            .build();
//                })
//                .build();
//
//        for (int i = 0; i < mFrameCount; i++) {
//            mUniformBuffers.push_back(vklite::UniformBufferBuilder().build(*mPhysicalDevice, *mDevice, sizeof(ColorUniformBufferObject)));
//            mUniformBuffers.back()->update(*mCommandPool, &colorUniformBufferObject, sizeof(ColorUniformBufferObject));
//        }
//
//        vklite::DescriptorSetWriter descriptorSetWriter = vklite::DescriptorSetWriterBuilder()
//                .frameCount(mFrameCount)
//                .descriptorSetMappingConfigure([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &descriptorSetMappingConfigure) {
//                    descriptorSetMappingConfigure
//                            .addMapping([&](vklite::DescriptorMapping &mapping) {
//                                mapping
//                                        .descriptorSet(mPipelineResources[frameIndex].getDescriptorSets()[0])
////                                        .binding(0)
//                                        .descriptorType(vk::DescriptorType::eUniformBuffer)
////                                        .descriptorIndex(0)
////                                        .descriptorCount(1)
//                                        .addBufferInfo(*mUniformBuffers[frameIndex]);
//                            });
//                })
//                .build();
//        mDevice->getDevice().updateDescriptorSets(descriptorSetWriter.createWriteDescriptorSets(), nullptr);
//    }
//
//    // 检查是否准备好
//    bool Test02SingleColorTriangle::isReady() {
//        return true;
//    }
//
//    // 绘制三角形帧
//    void Test02SingleColorTriangle::drawFrame() {
//        const vk::Device vkDevice = mDevice->getDevice();
//        vk::Semaphore imageAvailableSemaphore = mSyncObject->getImageAvailableSemaphore(mCurrentFrameIndex);
//        vk::Semaphore renderFinishedSemaphore = mSyncObject->getRenderFinishedSemaphore(mCurrentFrameIndex);
//        vk::Fence fence = mSyncObject->getFence(mCurrentFrameIndex);
//        std::array<vk::Fence, 1> waitFences = {fence};
//
//        vk::Result result = vkDevice.waitForFences(waitFences, vk::True, std::numeric_limits<uint64_t>::max());
//        if (result != vk::Result::eSuccess) {
//            LOG_E("waitForFences failed");
//            throw std::runtime_error("waitForFences failed");
//        }
//
//        // 当 acquireNextImageKHR 成功返回时，imageAvailableSemaphore 会被触发，表示图像已经准备好，可以用于渲染。
//        auto [acquireResult, imageIndex] = vkDevice.acquireNextImageKHR(mSwapchain->getSwapChain(), std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore);
//        if (acquireResult != vk::Result::eSuccess) {
//            if (acquireResult == vk::Result::eErrorOutOfDateKHR) {
//                // 交换链已与表面不兼容，不能再用于渲染。通常在窗口大小调整后发生。
//                LOG_E("acquireNextImageKHR: eErrorOutOfDateKHR, recreateSwapChain");
////                recreateSwapChain();
//                return;
//            } else if (acquireResult == vk::Result::eSuboptimalKHR) {
//                //vk::Result::eSuboptimalKHR 交换链仍然可以成功显示到表面，但表面属性不再完全匹配。
//                LOG_D("acquireNextImageKHR: eSuboptimalKHR");
//            } else {
//                LOG_E("acquireNextImageKHR: failed: %d", acquireResult);
//                throw std::runtime_error("acquireNextImageKHR failed");
//            }
//        }
//
//        const vklite::PooledCommandBuffer &commandBuffer = (*mCommandBuffers)[mCurrentFrameIndex];
//        commandBuffer.record([&](const vk::CommandBuffer &vkCommandBuffer) {
//            mRenderPass->execute(vkCommandBuffer, mFramebuffers[imageIndex].getFrameBuffer(), [&](const vk::CommandBuffer &vkCommandBuffer) {
//                mGraphicsPipeline->drawFrame(vkCommandBuffer, *mPipelineLayout, mPipelineResources[mCurrentFrameIndex], mViewports, mScissors);
//            });
//        });
//
//        result = mSyncObject->resetFence(mCurrentFrameIndex);
//        if (result != vk::Result::eSuccess) {
//            throw std::runtime_error("resetFences failed");
//        }
//
//        std::array<vk::Semaphore, 1> waitSemaphores = {imageAvailableSemaphore};
//        std::array<vk::PipelineStageFlags, 1> waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
//        std::array<vk::CommandBuffer, 1> commandBuffers = {commandBuffer.getCommandBuffer()};
//        std::array<vk::Semaphore, 1> signalSemaphores = {renderFinishedSemaphore};
//
//        vk::SubmitInfo submitInfo{};
//        submitInfo
//                .setWaitSemaphores(waitSemaphores)
//                .setWaitDstStageMask(waitStages)
//                .setCommandBuffers(commandBuffers)
//                .setSignalSemaphores(signalSemaphores);
//
//        std::array<vk::SubmitInfo, 1> submitInfos = {submitInfo};
////        mDevice->getGraphicsQueue().submit(submitInfos, fence);
//
//        std::array<vk::SwapchainKHR, 1> swapChains = {mSwapchain->getSwapChain()};
//        std::array<uint32_t, 1> imageIndices = {imageIndex};
//        vk::PresentInfoKHR presentInfo{};
//        presentInfo
//                .setWaitSemaphores(signalSemaphores)
//                .setSwapchains(swapChains)
//                .setImageIndices(imageIndices);
//
//        // https://github.com/KhronosGroup/Vulkan-Hpp/issues/599
//        // 当出现图片不匹配时， cpp风格的 presentKHR 会抛出异常， 而不是返回 result， 而C风格的 presentKHR 接口会返回 result
//        try {
////            result = mDevice->getPresentQueue().presentKHR(presentInfo);
//        } catch (const vk::OutOfDateKHRError &e) {
//            LOG_E("mPresentQueue.presentKHR => OutOfDateKHRError");
//            result = vk::Result::eErrorOutOfDateKHR;
//        }
//
//        if (result != vk::Result::eSuccess) {
//            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || mFramebufferResized) {
//                mFramebufferResized = false;
//                LOG_E("presentKHR: eErrorOutOfDateKHR or eSuboptimalKHR or mFramebufferResized, recreateSwapChain");
//                // todo: recreateSwapChain
////                recreateSwapChain();
//                return;
//            } else {
//                throw std::runtime_error("presentKHR failed");
//            }
//        }
//
//        mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mFrameCount;
//    }
//
//    // 清理操作
//    void Test02SingleColorTriangle::cleanup() {
//        LOG_I("Cleaning up %s", getName().c_str());
//    }

} // test