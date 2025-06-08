//
// Created by leixing on 2025/1/4.
//

#include "Test05TextureImage.h"
#include "util/FileUtil.h"

#include "util/FileUtil.h"
#include "vklite/vklite_android.h"
#include "stb_image.h"
#include "image/StbImage.h"

namespace test05 {

    Test05TextureImage::Test05TextureImage(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/05_texture_image.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/05_texture_image.frag.spv");

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
                .addPushConstant(0, sizeof(glm::mat4), vk::ShaderStageFlagBits::eVertex)
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addSampler(0, vk::ShaderStageFlagBits::eFragment);
                });

        mEngine = vklite::AndroidGraphicPipelineEngineBuilder::asDefault(mApp.window)
                .shaderConfigure(std::move(shaderConfigure))
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();
    }

    void Test05TextureImage::init() {

        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},  // 左上
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 1.0f}},  // 右上
                {{-1.0f, 1.0f,  0.0f}, {0.0f, 0.0f}},  // 左下
                {{1.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},  // 右下

        };

        std::vector<uint32_t> indices = {
                0, 2, 1, 1, 2, 3,
        };

        std::unique_ptr<image::ImageInterface> textureImage = image::StbImage::loadImage("/storage/emulated/0/01.png");

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


        mMvpMatrix = MvpMatrix{};
        float scale = 1.0f;

        float screenWidth = ANativeWindow_getWidth(mApp.window);
        float screenHeight = ANativeWindow_getHeight(mApp.window);
        float aspectRatio = screenWidth / screenHeight;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;
        mMvpMatrix.view = glm::lookAt(glm::vec3(0.0f, 5.0f, 5.0f),
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 1.0f, 0.0f));
        mMvpMatrix.proj = glm::perspective(glm::radians(45.0f),
                                           aspectRatio,
                                           1.0f,
                                           20.0f);
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

        mEngine->updateDescriptorSets([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
            configure
                    .descriptorSet(mEngine->getDescriptorSets(frameIndex, 0))
                    .addSampler([&](vklite::SamplerDescriptorMapping &mapping) {
                        mapping
                                .addImageInfo(mSamplers[frameIndex].getSampler(), mSamplers[frameIndex].getImageView());
                    });
        });

    }

    // 检查是否准备好
    bool Test05TextureImage::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test05TextureImage::drawFrame() {
        float time = mTimer.getElapsedTimeSecond();
        float scale = 1.0f;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;

//        mMvpMatrix.model = glm::mat4(1.0f); // 单位矩阵
//        mMvpMatrix.view = glm::mat4(1.0f);  // 单位矩阵
//        mMvpMatrix.proj = glm::mat4(1.0f);  // 单位矩阵
        glm::mat4 mvp = mMvpMatrix.proj * mMvpMatrix.view * mMvpMatrix.model;

        mEngine->updatePushConstant(0, &mvp, sizeof(glm::mat4));

        mEngine->drawIndexed();
    }

    // 清理操作
    void Test05TextureImage::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
//        mVkLiteEngine.reset();
    }

} // test
