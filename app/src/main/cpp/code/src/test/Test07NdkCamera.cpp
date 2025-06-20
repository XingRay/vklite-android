//
// Created by leixing on 2025/1/4.
//

#include "Test07NdkCamera.h"

#include <chrono>
#include <thread>

#include "util/FileUtil.h"
#include "engine/device_engine/AndroidDeviceEngineBuilder.h"

namespace std {

    template<>
    struct hash<test07::Vertex> {
        size_t operator()(test07::Vertex const &vertex) const {
            size_t seed = 0;

            // 哈希 position
            hash<glm::vec3> vec3Hash;
            seed ^= vec3Hash(vertex.position) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            // 哈希 uv
            hash<glm::vec2> vec2Hash;
            seed ^= vec2Hash(vertex.uv) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}


namespace test07 {
    Test07NdkCamera::Test07NdkCamera(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        mEngine = vklite::AndroidDeviceEngineBuilder::asDefault(mApp.window)
                .clearColor(0.2f, 0.4f, 0.8f)
                .addInstancePlugin(vklite::HardwareBufferPlugin::buildUnique())
                .addDevicePlugin(vklite::HardwareBufferPlugin::buildUnique())
                .buildUnique();


        mNdkCamera = std::make_unique<ndkcamera::NdkCamera>();
        mNdkCamera->startPreview();

        ndkcamera::Image image = mNdkCamera->loopAcquireImageWithBuffer();
        vklite::HardwareBuffer hardwareBuffer = vklite::HardwareBufferBuilder()
                .device(mEngine->getDevice().getDevice())
                .hardwareBuffer(image.getHardwareBuffer())
                .build();

        mSampler = vklite::CombinedHardwareBufferSamplerBuilder()
                .device(mEngine->getDevice().getDevice())
                .formatProperties(hardwareBuffer.getFormatProperties())
                .buildUnique();


        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/07_ndk_camera.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/07_ndk_camera.frag.spv");

        vklite::ShaderConfigure shaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3)
                            .addAttribute(1, ShaderFormat::Vec2);
                })
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addImmutableSampler(0, {mSampler->getSampler().getSampler()}, vk::ShaderStageFlagBits::eFragment);
                });

        mEngine->shaderConfigure(shaderConfigure);

        mFrameCounter = std::make_unique<util::FrameCounter>();
        LOG_D("test created ");
    }

    void Test07NdkCamera::init() {
        // x轴朝右, y轴朝下, z轴朝前, 右手系 (x,y)->z
        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}}, // 左上角
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f}}, // 右上角
                {{-1.0f, 1.0f,  0.0f}, {0.0f, 1.0f}}, // 左下角
                {{1.0f,  1.0f,  0.0f}, {0.0f, 0.0f}}, // 右下角
        };

        std::vector<uint32_t> indices = {
                0, 2, 1,
                1, 2, 3
        };

        uint32_t indicesSize = indices.size() * sizeof(uint32_t);
        mIndexBuffer = mEngine->indexBufferBuilder()
                .size(indicesSize)
                .buildUnique();
        mIndexBuffer->update(mEngine->getCommandPool(), indices);
        mEngine->indexBuffer(*mIndexBuffer, indices.size());

        uint32_t verticesSize = vertices.size() * sizeof(Vertex);
        mVertexBuffer = mEngine->vertexBufferBuilder()
                .size(verticesSize)
                .buildUnique();
        mVertexBuffer->update(mEngine->getCommandPool(), vertices.data(), verticesSize);
        mEngine->addVertexBuffer(*mVertexBuffer);
    }

    // 检查是否准备好
    bool Test07NdkCamera::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test07NdkCamera::drawFrame() {
//        LOG_D("Test07NdkCamera::drawFrame()");
        std::optional<ndkcamera::Image> image = mNdkCamera->acquireLatestImage();
        if (!image.has_value()) {
//            LOG_D("Test07NdkCamera::drawFrame(), no image");
            return;
        }
        AHardwareBuffer *pHardwareBuffer = image.value().getHardwareBuffer();
        if (pHardwareBuffer == nullptr) {
            LOG_D("Test07NdkCamera::drawFrame(), no hardwareBuffer");
            return;
        }

        vklite::HardwareBuffer hardwareBuffer = vklite::HardwareBufferBuilder()
                .device(mEngine->getDevice().getDevice())
                .hardwareBuffer(pHardwareBuffer)
                .build();
        mImageView = vklite::CombinedHardwareBufferImageViewBuilder()
                .device((*mEngine).getVkDevice())
                .hardwareBuffer(hardwareBuffer.getHardwareBuffer())
                .hardwareBufferFormatProperties(hardwareBuffer.getFormatProperties())
                .hardwareBufferDescription(hardwareBuffer.getAndroidHardwareBufferDescription())
                .hardwareBufferProperties(hardwareBuffer.getProperties())
                .memoryProperties((*mEngine).getMemoryProperties())
                .conversion((*mSampler).getConversion().getSamplerYcbcrConversion())
                .buildUnique();

        mEngine->updateDescriptorSets([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &mappingConfigure) {
            mappingConfigure
                    .descriptorSet(mEngine->getDescriptorSets(frameIndex, 0))
                    .addMapping([&](vklite::DescriptorMapping &descriptorMapping) {
                        descriptorMapping
                                .binding(0)
                                .descriptorType(vk::DescriptorType::eCombinedImageSampler)
                                .addImageInfo(mSampler->getSampler(), mImageView->getImageView());
                    });
        });

        mEngine->drawIndexed();

        // 增加帧计数器
        mFrameCounter->count();
        if (mFrameCounter->getAndResetIsFpsUpdated()) {
            LOG_D("FPS: %.2f", mFrameCounter->getFps());
        }
    }

    // 清理操作
    void Test07NdkCamera::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test
