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

#include "ndk_camera/NdkCamera.h"
#include "util/FrameCounter.h"
#include "engine/engines.h"

namespace test07 {

    struct Vertex {
        glm::vec3 position;
        glm::vec2 uv;

        bool operator==(const Vertex &other) const {
            return position == other.position && uv == other.uv;
        }
    };

    class Test07NdkCamera : public test::TestBase {
    private:

        const android_app &mApp;
        std::unique_ptr<vklite::DeviceEngine> mEngine;
        std::unique_ptr<vklite::VertexBuffer> mVertexBuffer;
        std::unique_ptr<vklite::IndexBuffer> mIndexBuffer;

        std::vector<vklite::CombinedImageSampler> mSamplers;
        // hardware buffer
        std::unique_ptr<vklite::SamplerYcbcrConversion> mConversion;
        std::unique_ptr<vklite::Sampler> mHardwareBufferSampler;

        std::unique_ptr<vklite::Image> mHardwareBufferImage;
        std::unique_ptr<vklite::ImageView> mHardwareBufferImageView;


        std::unique_ptr<util::FrameCounter> mFrameCounter;

        std::unique_ptr<ndkcamera::NdkCamera> mNdkCamera;


    public:
        // 构造函数初始化基类 TestBase，并传递 name
        explicit Test07NdkCamera(const android_app &app, const std::string &name);

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
