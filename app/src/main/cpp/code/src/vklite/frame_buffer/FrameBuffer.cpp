//
// Created by leixing on 2024/12/31.
//

#include "FrameBuffer.h"

#include <utility>

namespace vklite {

    FrameBuffer::FrameBuffer(vk::Device device, vk::Framebuffer frameBuffer)
            : mDevice(device), mFrameBuffer(frameBuffer) {}

    FrameBuffer::~FrameBuffer() {
        if (mDevice != nullptr && mFrameBuffer != nullptr) {
            mDevice.destroy(mFrameBuffer);
            mDevice = nullptr;
            mFrameBuffer = nullptr;
        }
    }

    FrameBuffer::FrameBuffer(FrameBuffer &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mFrameBuffer(std::exchange(other.mFrameBuffer, nullptr)) {}

    FrameBuffer &FrameBuffer::operator=(FrameBuffer &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mFrameBuffer = std::exchange(other.mFrameBuffer, nullptr);
        }
        return *this;
    }

    const vk::Framebuffer &FrameBuffer::getFrameBuffer() const {
        return mFrameBuffer;
    }

} // vklite