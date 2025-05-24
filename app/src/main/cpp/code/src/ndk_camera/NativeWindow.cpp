//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/NativeWindow.h"

#include <utility>

namespace ndkcamera {

    NativeWindow::NativeWindow(ANativeWindow *nativeWindow)
            : mNativeWindow(nativeWindow) {}

    NativeWindow::~NativeWindow() = default;

    NativeWindow::NativeWindow(NativeWindow &&other) noexcept
            : mNativeWindow(std::exchange(other.mNativeWindow, nullptr)) {}

    NativeWindow &NativeWindow::operator=(NativeWindow &&other) noexcept {
        if (this != &other) {
            mNativeWindow = std::exchange(other.mNativeWindow, nullptr);
        }
        return *this;
    }

    ANativeWindow *NativeWindow::getNativeWindow() const {
        return mNativeWindow;
    }

} // ndkcamera