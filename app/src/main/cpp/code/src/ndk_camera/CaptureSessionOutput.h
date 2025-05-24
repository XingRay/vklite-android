//
// Created by leixing on 2025/1/20.
//

#pragma once

#include "camera/NdkCameraCaptureSession.h"
#include "ndk_camera/NativeWindow.h"

namespace ndkcamera {

    class CaptureSessionOutput {
    private:
        ACaptureSessionOutput *mCaptureSessionOutput;

    public:
        explicit CaptureSessionOutput(ACaptureSessionOutput *captureSessionOutput);

        ~CaptureSessionOutput();

        CaptureSessionOutput(const CaptureSessionOutput &other) = delete;

        CaptureSessionOutput &operator=(const CaptureSessionOutput &other) = delete;

        CaptureSessionOutput(CaptureSessionOutput &&other) noexcept;

        CaptureSessionOutput &operator=(CaptureSessionOutput &&other) noexcept;


        [[nodiscard]]
        ACaptureSessionOutput *getCaptureSessionOutput() const;
    };

} // ndkcamera
