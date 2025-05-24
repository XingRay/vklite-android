//
// Created by leixing on 2025/1/19.
//

#include "ndk_camera/NdkCamera.h"
#include "ndk_camera/Log.h"

#include <camera/NdkCameraMetadata.h>

namespace ndkcamera {

    // sync camera / async camera
    // sync: getLatestHardwareBuffer / cleanLatestHardwareBuffer
    // async: setPreviewCallback
    NdkCamera::NdkCamera() {
        // select camera device
        mCameraManager = std::make_unique<CameraManager>();
        std::vector<CameraMetadata> cameraMetadataList = mCameraManager->queryCameraMetadataList();

        LOG_D("cameraInfoList: ");
        for (const CameraMetadata &cameraMetadata: cameraMetadataList) {
            LOG_D("CameraMetadata:\n\t\tid:%s\n\t\tlensFacing:%d\n\t\tsupportedHardwareLevel:%d\n\n",
                  cameraMetadata.getId(),
                  cameraMetadata.queryCameraLensFacing().value(),
                  cameraMetadata.querySupportedHardwareLevel().value()
            );
        }

        mCameraDevice = mCameraManager->openCamera(cameraMetadataList[0].getId());

        // create session
        // todo: image size selector
        mImageReader = ImageReader::buildUnique(1080, 1920, AIMAGE_FORMAT_PRIVATE, AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE, 2);
        mCaptureSessionOutput = mImageReader->createUniqueCaptureSessionOutput();

        mCaptureSessionOutputContainer = CaptureSessionOutputContainer::buildUnique();
        mCaptureSessionOutputContainer->add(*mCaptureSessionOutput);

        mCaptureRequest = mCameraDevice->createUniqueCaptureRequest();
        std::vector<CameraFpsRange> cameraFpsRanges = cameraMetadataList[0].queryCameraFpsRanges();
        for (const CameraFpsRange &fpsRange: cameraFpsRanges) {
            LOG_D("fpsRange: [%d, %d]", fpsRange.min, fpsRange.max);
        }
        mCaptureRequest->setFps(30);

        mCameraOutputTarget = mImageReader->createUniqueCameraOutputTarget();
        mCaptureRequest->addTarget(mCameraOutputTarget);

        mCaptureSession = mCameraDevice->createUniqueCaptureSessionWithSessionParameters(*mCaptureSessionOutputContainer, *mCaptureRequest);

        mImageReader->setImageListener([&](const ImageReader &imageReader) {
            LOG_D("imageReader:%p", &imageReader);
//            imageReader.
//            previewCallback();
        });
    }

    void NdkCamera::setPreviewCallback(std::function<void(NdkCamera *camera, AHardwareBuffer *hardwareBuffer)> previewCallback) {
        mPreviewCallback = previewCallback;
    }

    NdkCamera::~NdkCamera() = default;

    void NdkCamera::startPreview() {
        // 启动相机预览
        mCaptureSession->setRepeatingRequest(*mCaptureRequest);
    }

    void NdkCamera::stopPreview() {
        // 停止相机预览
        mCaptureSession->stopRepeating();
    }

    std::optional<Image> NdkCamera::acquireLatestImage() {
        return mImageReader->acquireLatestImage();
    }

    void NdkCamera::onImageAvailable(void *context, AImageReader *reader) {
        NdkCamera *ndkCamera = static_cast<NdkCamera *>(context);
        AImage *image = nullptr;
        AImageReader_acquireLatestImage(reader, &image);

        if (image) {
            // 获取 HardwareBuffer
            AHardwareBuffer *hardwareBuffer = nullptr;
            AImage_getHardwareBuffer(image, &hardwareBuffer);

            // 将 HardwareBuffer 传递给 Vulkan 进行渲染
            ndkCamera->mPreviewCallback(ndkCamera, hardwareBuffer);
            // 释放图像
            AImage_delete(image);
        }
    }

} // android