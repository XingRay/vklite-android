//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>
#include <optional>

#include <camera/NdkCameraDevice.h>

#include "CaptureSessionOutput.h"

namespace ndkcamera {

    class CaptureSessionOutputContainer {
    private:
        ACaptureSessionOutputContainer *mCaptureSessionOutputContainer;

    public:
        explicit CaptureSessionOutputContainer(ACaptureSessionOutputContainer *captureSessionOutputContainer);

        ~CaptureSessionOutputContainer();

        CaptureSessionOutputContainer(const CaptureSessionOutputContainer &other) = delete;

        CaptureSessionOutputContainer &operator=(const CaptureSessionOutputContainer &other) = delete;

        CaptureSessionOutputContainer(CaptureSessionOutputContainer &&other) noexcept;

        CaptureSessionOutputContainer &operator=(CaptureSessionOutputContainer &&other) noexcept;

        [[nodiscard]]
        ACaptureSessionOutputContainer *getCaptureSessionOutputContainer() const;

        camera_status_t add(const CaptureSessionOutput &sessionOutput);

    public://static
        static CaptureSessionOutputContainer build();

        static std::unique_ptr<CaptureSessionOutputContainer> buildUnique();
    };

} // ndkcamera
