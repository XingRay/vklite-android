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


namespace std {
    template<>
    struct hash<test06::Vertex> {
        size_t operator()(test06::Vertex const &vertex) const {
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
                            .addUniform(0, vk::ShaderStageFlagBits::eVertex)
                            .addSampler(1, vk::ShaderStageFlagBits::eFragment);
                });

        mEngine = vklite::AndroidSimpleGraphicEngineBuilder::asDefault(mApp.window)
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

//        std::vector<Vertex> vertices = {
//                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},  // 左上
//                {{1.0f,  -1.0f, 0.0f}, {1.0f, 1.0f}},  // 右上
//                {{-1.0f, 1.0f,  0.0f}, {0.0f, 0.0f}},  // 左下
//                {{1.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},  // 右下
//
//        };
//
//        std::vector<uint32_t> indices = {
//                0, 2, 1, 1, 2, 3,
//        };

        std::vector<model::Vertex> vertices = model.getVertices();
        std::vector<uint32_t> indices = model.getIndices();
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

        glm::mat4 modelMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = modelMat;
        mMvpMatrix.view = glm::lookAt(glm::vec3(5.0f, 5.0f, 5.0f),
                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                      glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.proj = glm::perspective(glm::radians(45.0f),
                                           aspectRatio,
                                           1.0f,
                                           20.0f);
        mMvpMatrix.proj[1][1]*=-1;
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

        glm::mat4 mvp = mMvpMatrix.proj * mMvpMatrix.view * mMvpMatrix.model;
        for (uint32_t i = 0; i < mEngine->getFrameCount(); i++) {
            mUniformBuffers[i].update(mEngine->getCommandPool(), &mvp, sizeof(glm::mat4));
        }

        mEngine->updateDescriptorSets([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
            configure
                    .addMapping([&](vklite::DescriptorMapping &mapping) {
                        mapping
                                .descriptorSet(mEngine->getDescriptorSets(frameIndex, 0))
//                                        .binding(0)
                                .descriptorType(vk::DescriptorType::eUniformBuffer)
//                                        .descriptorIndex(0)
//                                        .descriptorCount(1)
                                .addBufferInfo(mUniformBuffers[frameIndex].getBuffer());
                    })
                    .addMapping([&](vklite::DescriptorMapping &mapping) {
                        mapping
                                .descriptorSet(mEngine->getDescriptorSets(frameIndex, 0))
                                .binding(1)
                                .descriptorType(vk::DescriptorType::eCombinedImageSampler)
//                                        .descriptorIndex(0)
//                                        .descriptorCount(1)
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
        float time = mTimer.getElapsedTimeSecond();

        float scale = 1.0f;

        glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
        model = glm::rotate(model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        mMvpMatrix.model = model;

        glm::mat4 mvp = mMvpMatrix.proj * mMvpMatrix.view * mMvpMatrix.model;
        mUniformBuffers[mEngine->getFrameIndex()].update(mEngine->getCommandPool(), &mvp, sizeof(glm::mat4));

        mEngine->drawIndexed();
    }

    // 清理操作
    void Test06Load3dModel::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test
