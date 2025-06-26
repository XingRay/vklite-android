//
// Created by leixing on 2024/12/26.
//

#include "RenderPass.h"
#include "vklite/util/VulkanUtil.h"

#include <utility>

namespace vklite {

    RenderPass::RenderPass(const vk::Device &device, const vk::RenderPass &renderPass, RenderPassBeginInfo &&beginInfo)
            : mDevice(device), mRenderPass(renderPass), mBeginInfo(std::move(beginInfo)) {
        mBeginInfo.getBeginInfo().renderPass = mRenderPass;
    }

    RenderPass::~RenderPass() {
        if (mDevice != nullptr && mRenderPass != nullptr) {
            mDevice.destroy(mRenderPass);
            mDevice = nullptr;
            mRenderPass = nullptr;
        }
    }

    RenderPass::RenderPass(RenderPass &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mRenderPass(std::exchange(other.mRenderPass, nullptr)),
              mBeginInfo(std::move(other.mBeginInfo)) {

        mBeginInfo.getBeginInfo().renderPass = mRenderPass;
    }

    RenderPass &RenderPass::operator=(RenderPass &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mRenderPass = std::exchange(other.mRenderPass, nullptr);
            mBeginInfo = std::move(other.mBeginInfo);

            mBeginInfo.getBeginInfo().renderPass = mRenderPass;
        }
        return *this;
    }

    const vk::RenderPass &RenderPass::getVkRenderPass() const {
        return mRenderPass;
    }

    RenderPass &RenderPass::renderArea(vk::Rect2D renderArea) {
        mBeginInfo.getBeginInfo().setRenderArea(renderArea);
        return *this;
    }

    RenderPass &RenderPass::renderArea(int32_t x, int32_t y, uint32_t width, uint32_t height) {
        renderArea(vk::Rect2D{vk::Offset2D{x, y}, vk::Extent2D{width, height}});
        return *this;
    }

    RenderPass &RenderPass::renderAreaOffset(vk::Offset2D offset) {
        mBeginInfo.getBeginInfo().renderArea.setOffset(offset);
        return *this;
    }

    RenderPass &RenderPass::renderAreaOffset(int32_t x, int32_t y) {
        renderAreaOffset(vk::Offset2D{x, y});
        return *this;
    }

    RenderPass &RenderPass::renderAreaExtend(vk::Extent2D extent) {
        mBeginInfo.getBeginInfo().renderArea.setExtent(extent);
        return *this;
    }

    RenderPass &RenderPass::renderAreaExtend(uint32_t width, uint32_t height) {
        renderAreaExtend(vk::Extent2D{width, height});
        return *this;
    }

    RenderPass &RenderPass::subpassContents(vk::SubpassContents subpassContents) {
        mBeginInfo.subpassContents(subpassContents);
        return *this;
    }

    RenderPass &RenderPass::clearValues(std::vector<vk::ClearValue> &&clearValues) {
        mBeginInfo.clearValues(std::move(clearValues));
        return *this;
    }

    RenderPass &RenderPass::updateClearValue(size_t index, vk::ClearValue clearValue) {
        mBeginInfo.updateClearValue(index, clearValue);
        return *this;
    }

    void RenderPass::execute(const vk::CommandBuffer &commandBuffer, const vk::Framebuffer &framebuffer, std::function<void(const vk::CommandBuffer &commandBuffer)> handler) {
        mBeginInfo.getBeginInfo().setFramebuffer(framebuffer);
        commandBuffer.beginRenderPass(&(mBeginInfo.getBeginInfo()), mBeginInfo.getSubpassContents());
        handler(commandBuffer);
        commandBuffer.endRenderPass();
    }
}