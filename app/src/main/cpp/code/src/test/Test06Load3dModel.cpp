//
// Created by leixing on 2025/1/4.
//

#include "Test06Load3dModel.h"

#include "util/FileUtil.h"
#include "vklite/vklite_android.h"
#include "stb_image.h"
#include "image/StbImage.h"
#include "model/ModelLoader.h"

#include <tiny_obj_loader.h>

namespace test06 {

    Test06Load3dModel::Test06Load3dModel(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/06_load_3d_model.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/06_load_3d_model.frag.spv");

        vklite::ShaderConfigure shaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(model::Vertex))
                            .addAttribute(0, ShaderFormat::Vec3)
                            .addAttribute(1, ShaderFormat::Vec2);
                })
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniformBuffer(0, vk::ShaderStageFlagBits::eVertex)
                            .addCombinedImageSampler(1, vk::ShaderStageFlagBits::eFragment);
                });

        mEngine = vklite::AndroidGraphicPipelineEngineBuilder::asDefault(mApp.window)
                .shaderConfigure(std::move(shaderConfigure))
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();

        LOG_D("test created ");
    }

    void Test06Load3dModel::init() {
        const char *MODEL_PATH = "/storage/emulated/0/test/model/viking_room/viking_room.obj";
        const char *TEXTURE_PATH = "/storage/emulated/0/test/model/viking_room/viking_room.png";

        model::Model model = model::ModelLoader::load(MODEL_PATH);
        std::unique_ptr<image::StbImage> textureImage = image::StbImage::loadImageAsRgb(TEXTURE_PATH);

        mIndexBuffer = mEngine->indexBufferBuilder()
                .size(model.getIndicesBytes())
                .buildUnique();
        mIndexBuffer->update(mEngine->getCommandPool(), model.getIndices());
        mEngine->indexBuffer(*mIndexBuffer, model.getIndicesCount());

        mVertexBuffer = mEngine->vertexBufferBuilder()
                .size(model.getVerticesBytes())
                .buildUnique();
        mVertexBuffer->update(mEngine->getCommandPool(), model.getVerticesData(), model.getVerticesBytes());
        mEngine->addVertexBuffer(*mVertexBuffer);


        mMvpMatrix = math::MvpMatrix{};
        float screenWidth = ANativeWindow_getWidth(mApp.window);
        float screenHeight = ANativeWindow_getHeight(mApp.window);
        float aspectRatio = screenWidth / screenHeight;

        mMvpMatrix.view(glm::lookAt(glm::vec3(0.0f, 5.0f, 5.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.0f, 0.0f, 1.0f)));
        mMvpMatrix.projection(glm::perspective(glm::radians(45.0f),
                                               aspectRatio,
                                               1.0f,
                                               20.0f));
        mMvpMatrix.projectionFlipY();
        mTimer.start();

        mSamplers = mEngine->samplerBuilder()
                .width(textureImage->getWidth())
                .height(textureImage->getHeight())
                .format(textureImage->getFormat())
                .build(mEngine->getFrameCount());

        for (uint32_t i = 0; i < mEngine->getFrameCount(); i++) {
            mSamplers[i].getImage().transitionImageLayout(mEngine->getCommandPool());
            mSamplers[i].update(mEngine->getCommandPool(), textureImage->getPixels(), textureImage->getPixelBytes());
        }

        mUniformBuffers = mEngine->uniformBufferBuilder()
                .size(sizeof(math::MvpMatrix))
                .build(mEngine->getFrameCount());

        mEngine->updateDescriptorSets([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
            configure
                    .descriptorSet(mEngine->getDescriptorSets(frameIndex, 0))
                    .addUniformBuffer([&](vklite::UniformBufferDescriptorMapping &mapping) {
                        mapping
                                .binding(0)
                                .addBufferInfo(mUniformBuffers[frameIndex].getBuffer());
                    })
                    .addCombinedImageSampler([&](vklite::CombinedImageSamplerDescriptorMapping &mapping) {
                        mapping
                                .binding(1)
                                .addImageInfo(mSamplers[frameIndex].getSampler(), mSamplers[frameIndex].getImageView());
                    });
        });
    }

    // 检查是否准备好
    bool Test06Load3dModel::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test06Load3dModel::drawFrame() {
        float deltaTime = mTimer.getDeltaTimeSecond();

        mMvpMatrix.modelRotateZ(deltaTime * glm::radians(90.0f));

        glm::mat4 mvp = mMvpMatrix.calcMvp();
        mUniformBuffers[mEngine->getFrameIndex()].update(mEngine->getCommandPool(), &mvp, sizeof(glm::mat4));

        mEngine->drawIndexed();
    }

    // 清理操作
    void Test06Load3dModel::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test
