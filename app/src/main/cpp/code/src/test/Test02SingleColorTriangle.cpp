//
// Created by leixing on 2025/1/4.
//

#include "Test02SingleColorTriangle.h"

#include "util/FileUtil.h"
#include "vklite/vklite_android.h"

namespace test02 {

    Test02SingleColorTriangle::Test02SingleColorTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/02_triangle_color.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/02_triangle_color.frag.spv");

        vklite::ShaderConfigure shaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3);
                })
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniformBuffer([&](vklite::UniformBufferConfigure &uniformConfigure) {
                                uniformConfigure
                                        .binding(0)
                                        .descriptorCount(1)
                                        .shaderStageFlags(vk::ShaderStageFlagBits::eVertex);
                            });
                });

        mEngine = vklite::AndroidGraphicPipelineEngineBuilder::asDefault(mApp.window)
                .shaderConfigure(std::move(shaderConfigure))
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();
    }

    void Test02SingleColorTriangle::init() {
        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

        ColorUniformBufferObject colorUniformBufferObject{{0.8f, 0.4f, 0.2f}};


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

        mUniformBuffers = mEngine->uniformBufferBuilder()
                .size(sizeof(ColorUniformBufferObject))
                .build(mEngine->getFrameCount());

        for (uint32_t i = 0; i < mEngine->getFrameCount(); i++) {
            mUniformBuffers[i].update(mEngine->getCommandPool(), &colorUniformBufferObject, sizeof(ColorUniformBufferObject));
        }

        mEngine->updateDescriptorSets([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
            configure
                    .descriptorSet(mEngine->getDescriptorSets(frameIndex, 0))
                    .addUniformBuffer([&](vklite::UniformBufferDescriptorMapping &mapping) {
                        mapping
                                .addBufferInfo(mUniformBuffers[frameIndex].getBuffer());
                    });
        });
    }

    // 检查是否准备好
    bool Test02SingleColorTriangle::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test02SingleColorTriangle::drawFrame() {
        mEngine->drawIndexed();
    }

    // 清理操作
    void Test02SingleColorTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test