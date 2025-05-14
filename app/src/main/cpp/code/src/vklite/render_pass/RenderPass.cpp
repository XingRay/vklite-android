//
// Created by leixing on 2024/12/26.
//

#include "RenderPass.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    RenderPass::RenderPass(const Device &device,
                           const std::vector<vk::AttachmentDescription> &attachmentDescriptions,
                           const std::vector<vk::SubpassDescription> &subpassDescriptions,
                           const std::vector<vk::SubpassDependency> &subpassDependencies)
            : mDevice(device) {
        LOG_D("RenderPass::RenderPass");

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
}