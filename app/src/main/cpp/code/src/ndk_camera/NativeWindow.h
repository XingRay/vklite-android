//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <android/native_window.h>

namespace ndkcamera {

    class NativeWindow {
    private:
        ANativeWindow *mNativeWindow;

    public:
        explicit NativeWindow(ANativeWindow *nativeWindow);

        ~NativeWindow();

        NativeWindow(const NativeWindow &other) = delete;

        NativeWindow &operator=(const NativeWindow &other) = delete;

        NativeWindow(NativeWindow &&other) noexcept;

        NativeWindow &operator=(NativeWindow &&other) noexcept;


        [[nodiscard]]
        ANativeWindow *getNativeWindow() const;
    };

} // ndkcamera
