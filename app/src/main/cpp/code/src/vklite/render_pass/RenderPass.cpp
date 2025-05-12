//
// Created by leixing on 2024/12/26.
//

#include "RenderPass.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    RenderPass::RenderPass(const Device &device,
                           vk::Format displayFormat,
                           bool enableMsaa,
                           vk::SampleCountFlagBits sampleCountFlagBits,
                           bool enableDepth)
            : mDevice(device) {
        LOG_D("RenderPass::RenderPass");

        std::vector<vk::AttachmentDescription> attachmentDescriptions;
        uint32_t index = 0;

        std::vector<vk::AttachmentReference> msaaColorAttachmentReferences;
        std::vector<vk::AttachmentReference> depthAttachmentReferences;
        std::vector<vk::AttachmentReference> presentColorAttachmentReferences;

        if (enableMsaa) {
            vk::AttachmentDescription msaaColorAttachmentDescription{};
            msaaColorAttachmentDescription
                    .setFormat(displayFormat)
                    .setSamples(sampleCountFlagBits)
                            //载入图像前将帧缓冲清0
                    .setLoadOp(vk::AttachmentLoadOp::eClear)
                            // 渲染图像之后将图像数据保存
                    .setStoreOp(vk::AttachmentStoreOp::eStore)
                            // 模版缓冲, 这里不关注
                    .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                    .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                            // 常见的布局
                            //
                            //VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL：用作彩色附件的图像
                            //VK_IMAGE_LAYOUT_PRESENT_SRC_KHR：要在交换链中呈现的图像
                            //VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL：用作内存复制操作目标的图像
                            //
                            // initialLayout 渲染通道开始之前图像将具有的布局
                            // finalLayout 渲染通道完成时自动转换到的布局
                            //
                            // 使用 VK_IMAGE_LAYOUT_UNDEFINED 意味着我们不关心图像以前的布局
                            // 这个特殊值的警告是图像的内容不能保证被保留，但这并不重要，因为我们无论如何要清除
                    .setInitialLayout(vk::ImageLayout::eUndefined)
                    .setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);
            attachmentDescriptions.push_back(msaaColorAttachmentDescription);

            vk::AttachmentReference msaaColorAttachmentReference{};
            msaaColorAttachmentReference
                    .setAttachment(index)
                    .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

            msaaColorAttachmentReferences.push_back(msaaColorAttachmentReference);

            index++;
        }

        if (enableDepth) {
            vk::AttachmentDescription depthAttachmentDescription{};
            depthAttachmentDescription
                    .setFormat(device.getPhysicalDevice().findDepthFormat())
                    .setSamples(sampleCountFlagBits)
                    .setLoadOp(vk::AttachmentLoadOp::eClear)
                    .setStoreOp(vk::AttachmentStoreOp::eDontCare)
                    .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                    .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                    .setInitialLayout(vk::ImageLayout::eUndefined)
                    .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
            attachmentDescriptions.push_back(depthAttachmentDescription);

            vk::AttachmentReference depthAttachmentReference{};
            depthAttachmentReference
                    .setAttachment(index)
                    .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

            depthAttachmentReferences.push_back(depthAttachmentReference);

            index++;
        }

        vk::AttachmentDescription presentColorAttachmentDescription{};
        presentColorAttachmentDescription
                .setFormat(displayFormat)
                .setSamples(vk::SampleCountFlagBits::e1)
                .setLoadOp(vk::AttachmentLoadOp::eDontCare)
                .setStoreOp(vk::AttachmentStoreOp::eStore)
                .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                .setInitialLayout(vk::ImageLayout::eUndefined)
                .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);
        attachmentDescriptions.push_back(presentColorAttachmentDescription);

        vk::AttachmentReference presentColorAttachmentReference{};
        presentColorAttachmentReference
                .setAttachment(index)
                .setLayout(vk::ImageLayout::eColorAttachmentOptimal);
        presentColorAttachmentReferences.push_back(presentColorAttachmentReference);

        index++;


        vk::SubpassDescription subPassDescription{};
        subPassDescription.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
        if (enableMsaa) {
            subPassDescription
                    .setColorAttachments(msaaColorAttachmentReferences)
                    .setResolveAttachments(presentColorAttachmentReferences);
        } else {
            subPassDescription
                    .setColorAttachments(presentColorAttachmentReferences);

        }
        if (enableDepth) {
            // Vulkan规定每个Subpass最多只能关联一个深度/模板附件
            subPassDescription
                    .setPDepthStencilAttachment(&depthAttachmentReferences[0]);
        }

        std::array<vk::SubpassDescription, 1> subpassDescriptions{subPassDescription};

        vk::SubpassDependency subPassDependency = vk::SubpassDependency{}
                .setSrcSubpass(vk::SubpassExternal)
                .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
                .setSrcAccessMask(vk::AccessFlagBits::eNone)
                .setDstSubpass(0)
                .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
                .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);
        std::array<vk::SubpassDependency, 1> subpassDependencies = {subPassDependency};

        vk::RenderPassCreateInfo renderPassCreateInfo{};
        renderPassCreateInfo
                .setFlags(vk::RenderPassCreateFlags{})
                .setAttachments(attachmentDescriptions)
                .setSubpasses(subpassDescriptions)
                .setDependencies(subpassDependencies);

        mRenderPass = device.getDevice().createRenderPass(renderPassCreateInfo);
    }

    RenderPass::~RenderPass() {
        LOG_D("RenderPass::~RenderPass");
        mDevice.getDevice().destroy(mRenderPass);
    }

    const vk::RenderPass &RenderPass::getRenderPass() const {
        return mRenderPass;
    }

    vk::Format RenderPass::findDepthFormat(const vk::PhysicalDevice &physicalDevice) {
        return findSupportedFormat(
                physicalDevice,
                {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
                vk::ImageTiling::eOptimal,
                vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );
    }

    vk::Format RenderPass::findSupportedFormat(const vk::PhysicalDevice &physicalDevice, const std::vector<vk::Format> &candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
        for (const auto &format: candidates) {
            vk::FormatProperties properties = physicalDevice.getFormatProperties(format);
            if (tiling == vk::ImageTiling::eLinear && (properties.linearTilingFeatures & features) == features) {
                return format;
            } else if (tiling == vk::ImageTiling::eOptimal && (properties.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format !");
    }
}