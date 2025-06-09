//
// Created by leixing on 2025/5/14.
//

#pragma once

#include <vector>
#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class SubpassDescription {
    private:

        vk::SubpassDescriptionFlags mFlags;
        vk::PipelineBindPoint mPipelineBindPoint;

        std::vector<vk::AttachmentReference> mInputAttachments;
        std::vector<vk::AttachmentReference> mColorAttachments;
        std::vector<vk::AttachmentReference> mResolveAttachments;
        std::optional<vk::AttachmentReference> mDepthStencilAttachment;
        std::vector<uint32_t> mPreserveAttachments;

    public:
        explicit SubpassDescription(
                vk::SubpassDescriptionFlags flags,
                vk::PipelineBindPoint pipelineBindPoint,
                std::vector<vk::AttachmentReference> inputAttachments,
                std::vector<vk::AttachmentReference> colorAttachments,
                std::vector<vk::AttachmentReference> resolveAttachments,
                std::optional<vk::AttachmentReference> depthStencilAttachment,
                std::vector<uint32_t> preserveAttachments);

        ~SubpassDescription();

        [[nodiscard]]
        vk::SubpassDescription createSubpassDescription() const;
    };

} // vklite
