//
// Created by leixing on 2025/5/29.
//

#include "Surface.h"

#include <utility>

namespace vklite {

    Surface::Surface(vk::Instance instance, vk::SurfaceKHR surface)
            : mInstance(instance), mSurface(surface) {}

    Surface::~Surface() {
        if (mInstance != nullptr && mSurface != nullptr) {
            mInstance.destroy(mSurface);
            mInstance = nullptr;
            mSurface = nullptr;
        }
    }

    Surface::Surface(Surface &&other) noexcept
            : mInstance(std::exchange(other.mInstance, nullptr)),
              mSurface(std::exchange(other.mSurface, nullptr)) {}

    Surface &Surface::operator=(Surface &&other) noexcept {
        if (this != &other) {
            mInstance = std::exchange(other.mInstance, nullptr);
            mSurface = std::exchange(other.mSurface, nullptr);
        }
        return *this;
    }

    const vk::SurfaceKHR &Surface::getVkSurface() const {
        return mSurface;
    }

} // vklite