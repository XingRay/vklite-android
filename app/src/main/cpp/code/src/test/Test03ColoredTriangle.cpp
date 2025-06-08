//
// Created by leixing on 2025/1/4.
//

#include "Test03ColoredTriangle.h"

#include "util/FileUtil.h"
#include "vklite/vklite_android.h"

namespace test03 {

    Test03ColoredTriangle::Test03ColoredTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/03_colored_triangle.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/03_colored_triangle.frag.spv");

        vklite::ShaderConfigure graphicShaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3)
                            .addAttribute(1, ShaderFormat::Vec3);
                });

        mEngine = vklite::AndroidGraphicPipelineEngineBuilder::asDefault(mApp.window)
                .shaderConfigure(std::move(graphicShaderConfigure))
                .clearColor(0.2f, 0.4f, 0.8f)
                .buildUnique();
    }

    void Test03ColoredTriangle::init() {
        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}, {0.0f, 0.0f, 1.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};

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
    bool Test03ColoredTriangle::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test03ColoredTriangle::drawFrame() {
        mEngine->drawIndexed();
    }

    // 清理操作
    void Test03ColoredTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test