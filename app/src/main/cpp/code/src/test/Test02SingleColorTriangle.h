//
// Created by leixing on 2025/1/4.
//

#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "game_activity/native_app_glue/android_native_app_glue.h"
#include "math/glm.h"
#include "test/common/TestBase.h"

#include "vklite/vklite.h"
#include "engine/graphic_pipeline_engine/GraphicPipelineEngine.h"
#include "engine/graphic_pipeline_engine/GraphicPipelineEngineBuilder.h"

namespace test02 {

    struct Vertex {
        glm::vec3 pos;
    };

    struct ColorUniformBufferObject {
        // 颜色数据
        alignas(16) glm::vec3 color;
    };


    class Test02SingleColorTriangle : public test::TestBase {
    private:
        const android_app &mApp;
        std::unique_ptr<vklite::GraphicPipelineEngine> mEngine;
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;
        std::vector<vklite::UniformBuffer> mUniformBuffers;

    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test02SingleColorTriangle(const android_app &app, const std::string &name);

        // 初始化操作
        void init() override;

        // 检查是否准备好
        bool isReady() override;

        // 绘制三角形帧
        void drawFrame() override;

        // 清理操作
        void cleanup() override;
    };

} // test
