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

        auto meta = std::find_if(cameraMetadataList.begin(), cameraMetadataList.end(), [&](const CameraMetadata &metadata) {
            return metadata.queryCameraLensFacing() == CameraLensFacing::Value::FRONT;
        });
        if (meta == cameraMetadataList.end()) {
            throw std::runtime_error("front camera not found !");
        }

        LOG_D("CameraMetadata:\n\t\tid:%s\n\t\tlensFacing:%s\n\t\tsupportedHardwareLevel:%s\n\n",
              meta->getId(),
              CameraLensFacing::of(meta->queryCameraLensFacing()).name(),
              HardwareLevel::of(meta->querySupportedHardwareLevel()).name()
        );

        std::vector<StreamConfiguration> streamConfigurations = meta->queryScalerAvailableStreamConfigurations();
        for (const StreamConfiguration &configuration: streamConfigurations) {
            LOG_D("%s", configuration.toString().c_str());
        }

        mCameraDevice = mCameraManager->openCamera(meta->getId());

        // create session
        // todo: image size selector
        mImageReader = ImageReader::buildUnique(1920, 1080, AIMAGE_FORMAT_YUV_420_888, AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE, 2);
        mCaptureSessionOutput = mImageReader->createUniqueCaptureSessionOutput();

        mCaptureSessionOutputContainer = CaptureSessionOutputContainer::buildUnique();
        mCaptureSessionOutputContainer->add(*mCaptureSessionOutput);

        mCaptureRequest = mCameraDevice->createUniqueCaptureRequest();
        std::vector<Range> cameraFpsRanges = cameraMetadataList[0].queryCameraFpsRanges();
        LOG_D("Supported FPS ranges:");
        for (const Range &fpsRange: cameraFpsRanges) {
            LOG_D("\t[%d, %d]", fpsRange.getMin(), fpsRange.getMax());
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

    NdkCamera::~NdkCamera() = default;

    NdkCamera::NdkCamera(NdkCamera &&other) noexcept
            : mCameraManager(std::move(other.mCameraManager)),
              mCameraDevice(std::move(other.mCameraDevice)),
              mCaptureSession(std::move(other.mCaptureSession)),
              mImageReader(std::move(other.mImageReader)),
              mCaptureSessionOutputContainer(std::move(other.mCaptureSessionOutputContainer)),
              mCaptureSessionOutput(std::move(other.mCaptureSessionOutput)),
              mCaptureRequest(std::move(other.mCaptureRequest)),
              mCameraOutputTarget(std::move(other.mCameraOutputTarget)),
              mPreviewCallback(std::move(other.mPreviewCallback)) {}

    NdkCamera &NdkCamera::operator=(NdkCamera &&other) noexcept {
        if (this != &other) {
            mCameraManager = std::move(other.mCameraManager);
            mCameraDevice = std::move(other.mCameraDevice);
            mCaptureSession = std::move(other.mCaptureSession);
            mImageReader = std::move(other.mImageReader);
            mCaptureSessionOutputContainer = std::move(other.mCaptureSessionOutputContainer);
            mCaptureSessionOutput = std::move(other.mCaptureSessionOutput);
            mCaptureRequest = std::move(other.mCaptureRequest);
            mCameraOutputTarget = std::move(other.mCameraOutputTarget);
            mPreviewCallback = std::move(other.mPreviewCallback);
        }
        return *this;
    }

    void NdkCamera::setPreviewCallback(std::function<void(NdkCamera *camera, AHardwareBuffer *hardwareBuffer)> &&previewCallback) {
        mPreviewCallback = std::move(previewCallback);
    }

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

    Image NdkCamera::acquireLatestImageWithBuffer() {
        AHardwareBuffer *pHardwareBuffer = nullptr;
        std::optional<ndkcamera::Image> image = std::nullopt;
        while (pHardwareBuffer == nullptr) {
            LOG_D("waiting for getLatestHardwareBuffer...");
            image = acquireLatestImage();
            if (!image.has_value()) {
                continue;
            }
            pHardwareBuffer = image.value().getHardwareBuffer();
        }

        return std::move(image.value());
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