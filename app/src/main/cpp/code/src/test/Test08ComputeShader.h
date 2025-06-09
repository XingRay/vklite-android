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
#include "vklite/vklite_android.h"
#include "util/Timer.h"
#include "engine/engines.h"

namespace test08 {

    struct UniformBufferObject {
        float deltaTime = 1.0f;
    };

    struct Particle {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 color;
    };

    class Test08ComputeShader : public test::TestBase {
    private:

        const android_app &mApp;
        std::unique_ptr<vklite::ComputeGraphicEngine> mEngine;
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;

        static constexpr uint32_t mParticleCount = 8192;
        //status
        util::Timer mTimer;
        std::vector<vklite::StorageBuffer> mShaderStorageBuffers;
        std::vector<vklite::UniformBuffer> mUniformBuffers;

    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test08ComputeShader(const android_app &app, const std::string &name);

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
