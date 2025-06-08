//
// Created by leixing on 2025/1/20.
//

#include "CaptureSessionOutputContainer.h"

#include "ndk_camera/Log.h"

#include <utility>

namespace ndkcamera {

    CaptureSessionOutputContainer::CaptureSessionOutputContainer(ACaptureSessionOutputContainer *captureSessionOutputContainer)
            : mCaptureSessionOutputContainer(captureSessionOutputContainer) {}

    CaptureSessionOutputContainer::~CaptureSessionOutputContainer() {
        if (mCaptureSessionOutputContainer != nullptr) {
            ACaptureSessionOutputContainer_free(mCaptureSessionOutputContainer);
        }
    }

    CaptureSessionOutputContainer::CaptureSessionOutputContainer(CaptureSessionOutputContainer &&other) noexcept
            : mCaptureSessionOutputContainer(std::exchange(other.mCaptureSessionOutputContainer, nullptr)) {}

    CaptureSessionOutputContainer &CaptureSessionOutputContainer::operator=(CaptureSessionOutputContainer &&other) noexcept {
        if (this != &other) {
            mCaptureSessionOutputContainer = std::exchange(other.mCaptureSessionOutputContainer, nullptr);
        }
        return *this;
    }

    ACaptureSessionOutputContainer *CaptureSessionOutputContainer::getCaptureSessionOutputContainer() const {
        return mCaptureSessionOutputContainer;
    }

    camera_status_t CaptureSessionOutputContainer::add(const CaptureSessionOutput &sessionOutput) {
        camera_status_t status = ACaptureSessionOutputContainer_add(mCaptureSessionOutputContainer, sessionOutput.getCaptureSessionOutput());
        if (status != camera_status_t::ACAMERA_OK) {
            LOG_E("ACaptureSessionOutputContainer_add() failed, status:%d", status);
        }
        return status;
    }

    std::optional<CaptureSessionOutputContainer> CaptureSessionOutputContainer::build() {
        ACaptureSessionOutputContainer *captureSessionOutputContainer;
        camera_status_t status = ACaptureSessionOutputContainer_create(&captureSessionOutputContainer);
        if (status != camera_status_t::ACAMERA_OK || captureSessionOutputContainer == nullptr) {
            LOG_E("ACaptureSessionOutputContainer_create() failed, status:%d, captureSessionOutputContainer:%p", status, captureSessionOutputContainer);
            return std::nullopt;
        }
        return CaptureSessionOutputContainer(captureSessionOutputContainer);
    }

    std::unique_ptr<CaptureSessionOutputContainer> CaptureSessionOutputContainer::buildUnique() {
        ACaptureSessionOutputContainer *captureSessionOutputContainer;
        camera_status_t status = ACaptureSessionOutputContainer_create(&captureSessionOutputContainer);
        if (status != camera_status_t::ACAMERA_OK || captureSessionOutputContainer == nullptr) {
            LOG_E("ACaptureSessionOutputContainer_create() failed, status:%d, captureSessionOutputContainer:%p", status, captureSessionOutputContainer);
            return nullptr;
        }
        return std::make_unique<CaptureSessionOutputContainer>(captureSessionOutputContainer);
    }

} // ndkcamera