//
// Created by leixing on 2025/5/20.
//

#pragma once

#include <vector>
#include <cstddef>

#include "vklite/frame_buffer/Framebuffer.h"

namespace vklite {

    class Framebuffers {
    private:
        std::vector<Framebuffer> mFramebuffers;

    public:
        explicit Framebuffers();

        explicit Framebuffers(std::vector<Framebuffer> Framebuffers);

        ~Framebuffers();

        Framebuffers(const Framebuffers &other) = delete;

        Framebuffers &operator=(const Framebuffers &other) = delete;

        Framebuffers(Framebuffers &&other) noexcept;

        Framebuffers &operator=(Framebuffers &&other) noexcept;

        Framebuffer &operator[](size_t index);
    };

} // vklite
