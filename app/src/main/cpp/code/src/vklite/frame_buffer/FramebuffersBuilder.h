//
// Created by leixing on 2025/5/20.
//

#pragma once

#include <memory>
#include <cstdint>

#include "vklite/frame_buffer/Framebuffers.h"

namespace vklite {

    class FramebuffersBuilder {
    private:
        uint32_t mCount;
        std::function<Framebuffer(uint32_t index)> mFramebufferBuilder;

    public:
        FramebuffersBuilder();

        ~FramebuffersBuilder();

        FramebuffersBuilder &count(uint32_t count);

        FramebuffersBuilder &framebufferBuilder(std::function<Framebuffer(uint32_t index)> &&framebufferBuilder);

        Framebuffers build();

        std::unique_ptr<Framebuffers> buildUnique();
    };

} // vklite
