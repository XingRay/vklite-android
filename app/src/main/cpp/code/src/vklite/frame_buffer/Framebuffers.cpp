//
// Created by leixing on 2025/5/20.
//

#include "Framebuffers.h"

namespace vklite {

    Framebuffers::Framebuffers()
            : mFramebuffers{} {}

    Framebuffers::Framebuffers(std::vector<Framebuffer> Framebuffers)
            : mFramebuffers(std::move(Framebuffers)) {}

    Framebuffers::~Framebuffers() = default;

    Framebuffers::Framebuffers(Framebuffers &&other) noexcept
            : mFramebuffers(std::move(other.mFramebuffers)) {}

    Framebuffers &Framebuffers::operator=(Framebuffers &&other) noexcept {
        if (this != &other) {
            mFramebuffers = std::move(other.mFramebuffers);
        }
        return *this;
    }

    Framebuffer &Framebuffers::operator[](size_t index) {
        return mFramebuffers[index];
    }

} // vklite