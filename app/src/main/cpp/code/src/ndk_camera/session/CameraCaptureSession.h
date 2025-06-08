//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <memory>

#include "camera/NdkCameraCaptureSession.h"

#include "CaptureSessionOutputContainer.h"
#include "ndk_camera/CaptureRequest.h"

#include "CameraCaptureSessionStateCallbacks.h"
#include "CameraCaptureSessionCaptureCallbacks.h"

namespace ndkcamera {

    class CameraCaptureSession {

    private:
        ACameraCaptureSession *mCameraCaptureSession;

        std::unique_ptr<CameraCaptureSessionStateCallbacks> mStateCallbacks;
        std::unique_ptr<CameraCaptureSessionCaptureCallbacks> mCaptureCallbacks;

//        int mCaptureSequenceId;

    public:

        CameraCaptureSession(ACameraCaptureSession *captureSession,
                             std::unique_ptr<CameraCaptureSessionStateCallbacks> stateCallbacks,
                             std::unique_ptr<CameraCaptureSessionCaptureCallbacks> captureCallbacks);

        CameraCaptureSession(ACameraCaptureSession *captureSession,
                             std::unique_ptr<CameraCaptureSessionStateCallbacks> stateCallbacks);

        ~CameraCaptureSession();

        CameraCaptureSession(const CameraCaptureSession &other) = delete;

        CameraCaptureSession &operator=(const CameraCaptureSession &other) = delete;

        CameraCaptureSession(CameraCaptureSession &&other) noexcept;

        CameraCaptureSession &operator=(CameraCaptureSession &&other) noexcept;

        void setRepeatingRequest(const CaptureRequest &captureRequest);

        void stopRepeating();

    private:
        // state callbacks
        void onClosed();

        void onReady();

        void onActive();

        // callbacks
        void onCaptureStarted(
                ACameraCaptureSession *session,
                const ACaptureRequest *request, int64_t timestamp);

        void onCaptureProgressed(ACameraCaptureSession *session,
                                 ACaptureRequest *request, const ACameraMetadata *result);

        void onCaptureCompleted(ACameraCaptureSession *session,
                                ACaptureRequest *request, const ACameraMetadata *result);

        void onCaptureFailed(ACameraCaptureSession *session,
                             ACaptureRequest *request, ACameraCaptureFailure *failure);

        void onCaptureSequenceCompleted(ACameraCaptureSession *session,
                                        int sequenceId, int64_t frameNumber);

        void onCaptureSequenceAborted(ACameraCaptureSession *session,
                                      int sequenceId);

        void onCaptureBufferLost(ACameraCaptureSession *session,
                                 ACaptureRequest *request, ACameraWindowType *window, int64_t frameNumber);

    public://static

        static std::unique_ptr<CameraCaptureSessionStateCallbacks> createUniqueStateCallbacks();

        static std::unique_ptr<CameraCaptureSessionCaptureCallbacks> createUniqueCaptureCallbacks();

    private://static

        // state callbacks
        static void onClosed(void *context, ACameraCaptureSession *session);

        static void onReady(void *context, ACameraCaptureSession *session);

        static void onActive(void *context, ACameraCaptureSession *session);


        // capture callbacks
        static void onCaptureStarted(
                void *context, ACameraCaptureSession *session,
                const ACaptureRequest *request, int64_t timestamp);

        static void onCaptureProgressed(void *context, ACameraCaptureSession *session,
                                        ACaptureRequest *request, const ACameraMetadata *result);

        static void onCaptureCompleted(void *context, ACameraCaptureSession *session,
                                       ACaptureRequest *request, const ACameraMetadata *result);

        static void onCaptureFailed(void *context, ACameraCaptureSession *session,
                                    ACaptureRequest *request, ACameraCaptureFailure *failure);

        static void onCaptureSequenceCompleted(void *context, ACameraCaptureSession *session,
                                               int sequenceId, int64_t frameNumber);

        static void onCaptureSequenceAborted(void *context, ACameraCaptureSession *session,
                                             int sequenceId);

        static void onCaptureBufferLost(void *context, ACameraCaptureSession *session,
                                        ACaptureRequest *request, ACameraWindowType *window, int64_t frameNumber);

    };

} // ndkcamera
