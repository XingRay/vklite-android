//
// Created by leixing on 2025/1/20.
//

#include "ndk_camera/CaptureSessionOutput.h"
#include "camera/NdkCameraDevice.h"

#include <utility>

#include "Log.h"

namespace ndkcamera {

    CaptureSessionOutput::CaptureSessionOutput(ACaptureSessionOutput *captureSessionOutput)
            : mCaptureSessionOutput(captureSessionOutput) {}

    CaptureSessionOutput::~CaptureSessionOutput() {
        if (mCaptureSessionOutput != nullptr) {
            ACaptureSessionOutput_free(mCaptureSessionOutput);
        }
    }

    CaptureSessionOutput::CaptureSessionOutput(CaptureSessionOutput &&other) noexcept
            : mCaptureSessionOutput(std::exchange(other.mCaptureSessionOutput, nullptr)) {}

    CaptureSessionOutput &CaptureSessionOutput::operator=(CaptureSessionOutput &&other) noexcept {
        if (this != &other) {
            mCaptureSessionOutput = std::exchange(other.mCaptureSessionOutput, nullptr);
        }
        return *this;
    }

    ACaptureSessionOutput *CaptureSessionOutput::getCaptureSessionOutput() const {
        return mCaptureSessionOutput;
    }

} // ndkcamera