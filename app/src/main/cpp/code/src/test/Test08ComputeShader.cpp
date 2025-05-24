//
// Created by leixing on 2025/1/4.
//

#include "Test08ComputeShader.h"
#include "util/FileUtil.h"

#include <chrono>
#include <thread>
#include <filesystem>

#include <opencv2/opencv.hpp>


#include "ncnn/net.h"

#include "MNN/Interpreter.hpp"

namespace std {

    template<>
    struct hash<test08::Vertex> {
        size_t operator()(test08::Vertex const &vertex) const {
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


namespace test08 {

    Test08ComputeShader::Test08ComputeShader(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        std::vector<std::string> instanceExtensions = {
                VK_KHR_SURFACE_EXTENSION_NAME,
                VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,

                // old version
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
                // new version
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };

        std::vector<std::string> instanceLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<const char *> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        std::vector<const char *> deviceLayers = {
                "VK_LAYER_KHRONOS_validation"
        };

        std::vector<uint32_t> computeShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/08_compute.comp.spv");
        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/08_compute.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/08_compute.frag.spv");


    }

    void Test08ComputeShader::init() {

        // x轴朝右, y轴朝下, z轴朝前, 右手系 (x,y)->z
        std::vector<Vertex> vertices = {
                {{-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}}, // 左上角
                {{1.0f,  -1.0f, 0.0f}, {0.0f, 0.0f}}, // 右上角
                {{-1.0f, 1.0f,  0.0f}, {1.0f, 1.0f}}, // 左下角
                {{1.0f,  1.0f,  0.0f}, {1.0f, 0.0f}}, // 右下角
        };

        std::vector<uint32_t> indices = {0, 2, 1, 1, 2, 3};

//        LOG_D("mVkLiteEngine->createStagingTransferVertexBuffer");
//        mVkLiteEngine->createStagingTransferVertexBuffer(vertices.size() * sizeof(Vertex));
//
//        LOG_D("mVkLiteEngine->updateVertexBuffer");
//        mVkLiteEngine->updateVertexBuffer(vertices);
//
//        LOG_D("mVkLiteEngine->createStagingTransferIndexBuffer");
//        mVkLiteEngine->createStagingTransferIndexBuffer(indices.size() * sizeof(uint32_t));
//        LOG_D("mVkLiteEngine->updateIndexBuffer");
//        mVkLiteEngine->updateIndexBuffer(indices);


        std::string original_img_path = R"(/storage/emulated/0/test/image/face_image_1080_1920.png)";

        // 加载图像
        cv::Mat originalImg = cv::imread(original_img_path, cv::IMREAD_UNCHANGED);
        if (originalImg.empty()) {
            LOG_E("图片未找到: %s", original_img_path.c_str());
            return;
        }

        // 转换通道：如果图像有 4 通道，转换为 RGB；否则从 BGR 转换为 RGB
        if (originalImg.channels() == 4) {
            LOG_D("COLOR_BGRA2RGB");
            cv::cvtColor(originalImg, originalImg, cv::COLOR_BGRA2RGB);
        } else {
            LOG_D("COLOR_BGR2RGB");
            cv::cvtColor(originalImg, originalImg, cv::COLOR_BGR2RGB);
        }

        // 1. letterbox处理后得到 padded 图像，尺寸为 128x128，格式为 RGB
//        PaddingParams padding_params{};
//        cv::Mat padded = letterbox_padding(originalImg, cv::Size(128, 128), padding_params);
//
//        cv::Mat padded_float;
//        padded.convertTo(padded_float, CV_32FC3, 1.0 / 255.0);

    }

    // 检查是否准备好
    bool Test08ComputeShader::isReady() {
        return true;
    }

    // 绘制三角形帧
    void Test08ComputeShader::drawFrame() {
        // 静态变量用于帧率统计
        static auto startTime = std::chrono::steady_clock::now(); // 统计开始时间
        static int frameCount = 0;                               // 帧计数器


//        LOG_D("Test07NdkCamera::drawFrame()");
        AHardwareBuffer *buffer;// = mNdkCamera->getLatestHardwareBuffer();
//        LOG_D("AHardwareBuffer:%p", buffer);
        if (buffer != nullptr) {
//            mVkLiteEngine->updateUniformBuffer(mVkLiteEngine->getCurrentFrameIndex(), 0, 0, buffer, 0);
//            mVkLiteEngine->drawFrame();

            // 增加帧计数器
            frameCount++;

            // 计算时间差
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

            // 每 5 秒输出一次帧率
            if (elapsedTime >= 5) {
                float fps = static_cast<float>(frameCount) / elapsedTime; // 计算帧率
                LOG_D("FPS: %.2f", fps);                                 // 输出帧率

                // 重置统计
                startTime = currentTime;
                frameCount = 0;
            }
        }

    }

    // 清理操作
    void Test08ComputeShader::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
//        mVkLiteEngine.reset();
    }
} // test