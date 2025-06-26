//
// Created by leixing on 2024/12/31.
//

#include "Framebuffer.h"

#include <utility>

namespace vklite {

    Framebuffer::Framebuffer(vk::Device device, vk::Framebuffer frameBuffer)
            : mDevice(device), mFramebuffer(frameBuffer) {}

    Framebuffer::~Framebuffer() {
        if (mDevice != nullptr && mFramebuffer != nullptr) {
            mDevice.destroy(mFramebuffer);
            mDevice = nullptr;
            mFramebuffer = nullptr;
        }
    }

    Framebuffer::Framebuffer(Framebuffer &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mFramebuffer(std::exchange(other.mFramebuffer, nullptr)) {}

    Framebuffer &Framebuffer::operator=(Framebuffer &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mFramebuffer = std::exchange(other.mFramebuffer, nullptr);
        }
        return *this;
    }

    const vk::Framebuffer &Framebuffer::getVkFramebuffer() const {
        return mFramebuffer;
    }

} // vklite