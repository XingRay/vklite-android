//
// Created by leixing on 2025/1/20.
//

#include "CameraCaptureSession.h"

#include <array>
#include <utility>

#include "ndk_camera/Log.h"


namespace ndkcamera {

    CameraCaptureSession::CameraCaptureSession(ACameraCaptureSession *captureSession,
                                               std::unique_ptr<CameraCaptureSessionStateCallbacks> stateCallbacks,
                                               std::unique_ptr<CameraCaptureSessionCaptureCallbacks> captureCallbacks)
            : mCameraCaptureSession(captureSession),
              mStateCallbacks(std::move(stateCallbacks)),
              mCaptureCallbacks(std::move(captureCallbacks)) {

        mStateCallbacks->getStateCallbacks().context = this;
        mCaptureCallbacks->getCaptureCallbacks().context = this;
    }

    CameraCaptureSession::CameraCaptureSession(ACameraCaptureSession *captureSession,
                                               std::unique_ptr<CameraCaptureSessionStateCallbacks> stateCallbacks)
            : CameraCaptureSession(captureSession, std::move(stateCallbacks), createUniqueCaptureCallbacks()) {}

    CameraCaptureSession::~CameraCaptureSession() {
        if (mCameraCaptureSession != nullptr) {
            ACameraCaptureSession_close(mCameraCaptureSession);
        }
    }

    CameraCaptureSession::CameraCaptureSession(CameraCaptureSession &&other) noexcept
            : mCameraCaptureSession(std::exchange(other.mCameraCaptureSession, nullptr)),
              mStateCallbacks(std::move(other.mStateCallbacks)),
              mCaptureCallbacks(std::move(other.mCaptureCallbacks)) {

        mStateCallbacks->getStateCallbacks().context = this;
        mCaptureCallbacks->getCaptureCallbacks().context = this;
    }

    CameraCaptureSession &CameraCaptureSession::operator=(CameraCaptureSession &&other) noexcept {
        if (this != &other) {
            mCameraCaptureSession = std::exchange(other.mCameraCaptureSession, nullptr);
            mStateCallbacks = std::move(other.mStateCallbacks);
            mCaptureCallbacks = std::move(other.mCaptureCallbacks);

            mStateCallbacks->getStateCallbacks().context = this;
            mCaptureCallbacks->getCaptureCallbacks().context = this;
        }
        return *this;
    }

    void CameraCaptureSession::setRepeatingRequest(const CaptureRequest &captureRequest) {
        std::array<ACaptureRequest *, 1> requests{captureRequest.getCaptureRequest()};

//        int *id = &mCaptureSequenceId;
        int *id = nullptr;
        camera_status_t status = ACameraCaptureSession_setRepeatingRequest(mCameraCaptureSession, &(mCaptureCallbacks->getCaptureCallbacks()), 1, requests.data(), id);
        if (status != camera_status_t::ACAMERA_OK) {
            LOG_E("ACameraCaptureSession_setRepeatingRequest() failed, status:%d", status);
        }
    }

    void CameraCaptureSession::stopRepeating() {
        ACameraCaptureSession_stopRepeating(mCameraCaptureSession);
    }

    void CameraCaptureSession::onClosed() {
        LOG_D("CameraCaptureSession::onClosed(), this:%p", this);
    }

    void CameraCaptureSession::onReady() {
        LOG_D("CameraCaptureSession::onReady(), this:%p", this);
    }

    void CameraCaptureSession::onActive() {
        LOG_D("CameraCaptureSession::onActive(), this:%p", this);
    }

    void CameraCaptureSession::onCaptureStarted(
            ACameraCaptureSession *session,
            const ACaptureRequest *request, int64_t timestamp) {
//        LOG_D("CameraCaptureSession::onCaptureStarted, this:%p", this);
    }

    void CameraCaptureSession::onCaptureProgressed(ACameraCaptureSession *session,
                                                   ACaptureRequest *request, const ACameraMetadata *result) {
//        LOG_D("CameraCaptureSession::onCaptureProgressed, this:%p", this);
    }

    void CameraCaptureSession::onCaptureCompleted(ACameraCaptureSession *session,
                                                  ACaptureRequest *request, const ACameraMetadata *result) {
//        LOG_D("CameraCaptureSession::onCaptureCompleted, this:%p", this);
    }

    void CameraCaptureSession::onCaptureFailed(ACameraCaptureSession *session,
                                               ACaptureRequest *request, ACameraCaptureFailure *failure) {
        LOG_D("CameraCaptureSession::onCaptureFailed, this:%p", this);
    }

    void CameraCaptureSession::onCaptureSequenceCompleted(ACameraCaptureSession *session,
                                                          int sequenceId, int64_t frameNumber) {
        LOG_D("CameraCaptureSession::onCaptureSequenceCompleted, this:%p", this);
    }

    void CameraCaptureSession::onCaptureSequenceAborted(ACameraCaptureSession *session,
                                                        int sequenceId) {
        LOG_D("CameraCaptureSession::onCaptureSequenceAborted, this:%p", this);
    }

    void CameraCaptureSession::onCaptureBufferLost(ACameraCaptureSession *session,
                                                   ACaptureRequest *request, ACameraWindowType *window, int64_t frameNumber) {
        LOG_D("CameraCaptureSession::onCaptureBufferLost, this:%p", this);
    }


    // static methods
    std::unique_ptr<CameraCaptureSessionStateCallbacks> CameraCaptureSession::createUniqueStateCallbacks() {
        std::unique_ptr<CameraCaptureSessionStateCallbacks> callbacks = std::make_unique<CameraCaptureSessionStateCallbacks>();

        ACameraCaptureSession_stateCallbacks &stateCallbacks = callbacks->getStateCallbacks();
        stateCallbacks.onClosed = onClosed;
        stateCallbacks.onReady = onReady;
        stateCallbacks.onActive = onActive;

        return callbacks;
    }

    std::unique_ptr<CameraCaptureSessionCaptureCallbacks> CameraCaptureSession::createUniqueCaptureCallbacks() {
        std::unique_ptr<CameraCaptureSessionCaptureCallbacks> callbacks = std::make_unique<CameraCaptureSessionCaptureCallbacks>();

        ACameraCaptureSession_captureCallbacks &captureCallbacks = callbacks->getCaptureCallbacks();
        captureCallbacks.onCaptureStarted = onCaptureStarted;
        captureCallbacks.onCaptureProgressed = onCaptureProgressed;
        captureCallbacks.onCaptureCompleted = onCaptureCompleted;
        captureCallbacks.onCaptureFailed = onCaptureFailed;
        captureCallbacks.onCaptureSequenceCompleted = onCaptureSequenceCompleted;
        captureCallbacks.onCaptureSequenceAborted = onCaptureSequenceAborted;
        captureCallbacks.onCaptureBufferLost = onCaptureBufferLost;

        return callbacks;
    }

    void CameraCaptureSession::onClosed(void *context, ACameraCaptureSession *session) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onClosed();
    }

    void CameraCaptureSession::onReady(void *context, ACameraCaptureSession *session) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onReady();
    }

    void CameraCaptureSession::onActive(void *context, ACameraCaptureSession *session) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onActive();
    }

    void CameraCaptureSession::onCaptureStarted(
            void *context, ACameraCaptureSession *session,
            const ACaptureRequest *request, int64_t timestamp) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureStarted(session, request, timestamp);
    }

    void CameraCaptureSession::onCaptureProgressed(void *context, ACameraCaptureSession *session,
                                                   ACaptureRequest *request, const ACameraMetadata *result) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureProgressed(session, request, result);
    }

    void CameraCaptureSession::onCaptureCompleted(void *context, ACameraCaptureSession *session,
                                                  ACaptureRequest *request, const ACameraMetadata *result) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureCompleted(session, request, result);
    }

    void CameraCaptureSession::onCaptureFailed(void *context, ACameraCaptureSession *session,
                                               ACaptureRequest *request, ACameraCaptureFailure *failure) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureFailed(session, request, failure);
    }

    void CameraCaptureSession::onCaptureSequenceCompleted(void *context, ACameraCaptureSession *session,
                                                          int sequenceId, int64_t frameNumber) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureSequenceCompleted(session, sequenceId, frameNumber);
    }

    void CameraCaptureSession::onCaptureSequenceAborted(void *context, ACameraCaptureSession *session,
                                                        int sequenceId) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureSequenceAborted(session, sequenceId);
    }

    void CameraCaptureSession::onCaptureBufferLost(void *context, ACameraCaptureSession *session,
                                                   ACaptureRequest *request, ACameraWindowType *window, int64_t frameNumber) {
        CameraCaptureSession *cameraCaptureSession = static_cast<CameraCaptureSession *>(context);
        cameraCaptureSession->onCaptureBufferLost(session, request, window, frameNumber);
    }

} // ndkcamera