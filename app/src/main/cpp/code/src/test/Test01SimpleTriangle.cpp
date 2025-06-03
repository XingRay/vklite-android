//
// Created by leixing on 2025/1/4.
//

#include "Test01SimpleTriangle.h"
#include "util/FileUtil.h"
#include "vklite/util/VulkanUtil.h"
#include "vklite/physical_device/msaa/MaxMsaaSampleCountSelector.h"
#include "vklite/vklite_android.h"

namespace test01 {

    Test01SimpleTriangle::Test01SimpleTriangle(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/01_triangle.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/01_triangle.frag.spv");

        vklite::ShaderConfigure graphicShaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Vertex))
                            .addAttribute(0, ShaderFormat::Vec3);
                });

        mEngine = vklite::AndroidSimpleGraphicEngineBuilder::asDefault(mApp.window)
                .shaderConfigure(std::move(graphicShaderConfigure))
                .buildUnique();

        LOG_D("test created ");
    }

    void Test01SimpleTriangle::init() {
        std::vector<Vertex> vertices = {
                {{1.0f,  -1.0f, 0.0f}},
                {{-1.0f, -1.0f, 0.0f}},
                {{0.0f,  1.0f,  0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2};


        uint32_t indicesSize = indices.size() * sizeof(uint32_t);
        mIndexBuffer = mEngine->indexBufferBuilder()
                .size(indicesSize)
                .buildUnique();
        mIndexBuffer->update(mEngine->getCommandPool(), indices);

        uint32_t verticesSize = vertices.size() * sizeof(Vertex);
        mVertexBuffer = mEngine->vertexBufferBuilder()
                .size(verticesSize)
                .buildUnique();
        mVertexBuffer->update(mEngine->getCommandPool(), vertices.data(), verticesSize);

        mEngine->addVertexBuffer(*mVertexBuffer)
                .indexBuffer(*mIndexBuffer, indices.size());

    }

    // 检查是否准备好
    bool Test01SimpleTriangle::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test01SimpleTriangle::drawFrame() {
        mEngine->drawIndexed();
    }

    // 清理操作
    void Test01SimpleTriangle::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }

} // test