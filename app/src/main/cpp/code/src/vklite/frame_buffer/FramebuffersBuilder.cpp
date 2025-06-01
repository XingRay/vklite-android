//
// Created by leixing on 2025/5/20.
//

#include "FramebuffersBuilder.h"

namespace vklite {

    FramebuffersBuilder::FramebuffersBuilder() : mCount(0) {}

    FramebuffersBuilder::~FramebuffersBuilder() = default;

    FramebuffersBuilder &FramebuffersBuilder::count(uint32_t count) {
        mCount = count;
        return *this;
    }

    FramebuffersBuilder &FramebuffersBuilder::framebufferBuilder(std::function<Framebuffer(uint32_t index)> &&FramebufferBuilder) {
        mFramebufferBuilder = std::move(FramebufferBuilder);
        return *this;
    }

    Framebuffers FramebuffersBuilder::build() {
        std::vector<Framebuffer> framebuffers;
        framebuffers.reserve(mCount);
        for (uint32_t index = 0; index < mCount; index++) {
            framebuffers.push_back(mFramebufferBuilder(index));
        }
        return Framebuffers(std::move(framebuffers));
    }

    std::unique_ptr<Framebuffers> FramebuffersBuilder::buildUnique() {
        return std::make_unique<Framebuffers>(build());
    }

} // vklite