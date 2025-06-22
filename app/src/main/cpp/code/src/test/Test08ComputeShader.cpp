//
// Created by leixing on 2025/1/4.
//

#include "Test08ComputeShader.h"
#include "util/FileUtil.h"

#include <chrono>
#include <thread>
#include <filesystem>
#include <random>
#include <ctime>

namespace test08 {

    Test08ComputeShader::Test08ComputeShader(const android_app &app, const std::string &name)
            : TestBase(name), mApp(app) {

        std::vector<uint32_t> computeShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/08_compute.comp.spv");
        std::vector<uint32_t> vertexShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/08_compute.vert.spv");
        std::vector<uint32_t> fragmentShaderCode = FileUtil::loadSpvFile(mApp.activity->assetManager, "shaders/08_compute.frag.spv");

        uint32_t width = ANativeWindow_getWidth(mApp.window);
        uint32_t height = ANativeWindow_getHeight(mApp.window);

        // Initialize particles
        std::default_random_engine rndEngine((unsigned) time(nullptr));
        std::uniform_real_distribution<float> rndDist(0.0f, 1.0f);

        // Initial particle positions on a circle
        std::vector<Particle> particles(mParticleCount);
        for (auto &particle: particles) {
            float r = 0.25f * sqrt(rndDist(rndEngine));
            float theta = rndDist(rndEngine) * 2.0f * 3.14159265358979323846f;
            float x = r * cos(theta) * height / width;
            float y = r * sin(theta);
            particle.position = glm::vec2(x, y);
            particle.velocity = glm::normalize(glm::vec2(x, y)) * 0.00025f;
            particle.color = glm::vec4(rndDist(rndEngine), rndDist(rndEngine), rndDist(rndEngine), 1.0f);
        }

        VkDeviceSize shaderStorageBufferSize = sizeof(Particle) * mParticleCount;


        vklite::ShaderConfigure graphicShaderConfigure = vklite::ShaderConfigure()
                .vertexShaderCode(std::move(vertexShaderCode))
                .fragmentShaderCode(std::move(fragmentShaderCode))
                .addVertexBinding([&](vklite::VertexBindingConfigure &vertexBindingConfigure) {
                    vertexBindingConfigure
                            .binding(0)
                            .stride(sizeof(Particle))
//                            .inputRate(vk::VertexInputRate::eVertex)
                            .addAttribute(0, ShaderFormat::Vec2, offsetof(Particle, position))
                            .addAttribute(1, ShaderFormat::Vec4, offsetof(Particle, color));
                });

        vklite::ShaderConfigure computeShaderConfigure = vklite::ShaderConfigure()
                .computeShaderCode(std::move(computeShaderCode))
                .addDescriptorSetConfigure([&](vklite::DescriptorSetConfigure &descriptorSetConfigure) {
                    descriptorSetConfigure
                            .set(0)
                            .addUniformBuffer(0, 1, vk::ShaderStageFlagBits::eCompute)
                            .addStorageBuffer(1, 1, vk::ShaderStageFlagBits::eCompute)
                            .addStorageBuffer(2, 1, vk::ShaderStageFlagBits::eCompute);
                });

        mEngine = vklite::AndroidComputeGraphicEngineBuilder::asDefault(app.window)
                .clearColor(0.2f, 0.4f, 0.8f)
                .computeShaderConfigure(std::move(computeShaderConfigure))
                .graphicShaderConfigure(std::move(graphicShaderConfigure))
                .buildUnique();

        for (int i = 0; i < mEngine->getFrameCount(); i++) {
            vklite::StorageBuffer storageBuffer = mEngine->storageBufferBuilder()
                    .size(shaderStorageBufferSize)
                    .addUsage(vk::BufferUsageFlagBits::eVertexBuffer)
                    .build();
            storageBuffer.update(mEngine->getCommandPool(), particles.data(), shaderStorageBufferSize);
            mShaderStorageBuffers.push_back(std::move(storageBuffer));

            vklite::UniformBuffer uniformBuffer = mEngine->uniformBufferBuilder()
                    .size(sizeof(UniformBufferObject))
                    .build();

            mUniformBuffers.push_back(std::move(uniformBuffer));
        }

        mEngine->addVertexBuffers([&](uint32_t frameIndex, std::vector<vk::Buffer> &buffers, std::vector<vk::DeviceSize> &offsets) {
            buffers.push_back(mShaderStorageBuffers[frameIndex].getVkBuffer());
            offsets.push_back(0);
        });


        mEngine->updateDescriptorSets([&](uint32_t frameIndex, vklite::DescriptorSetMappingConfigure &configure) {
            configure
                    .descriptorSet(mEngine->getComputeDescriptorSets(frameIndex, 0))
                    .addMapping([&](vklite::DescriptorMapping &mapping) {
                        mapping
                                .binding(0)
                                .descriptorType(vk::DescriptorType::eUniformBuffer)
                                .addBufferInfo(mUniformBuffers[frameIndex].getCombinedMemoryBuffer().getBuffer());
                    })
                    .addMapping([&](vklite::DescriptorMapping &mapping) {
                        mapping
                                .binding(1)
                                .descriptorType(vk::DescriptorType::eStorageBuffer)
                                .addBufferInfo(mShaderStorageBuffers[(frameIndex - 1) % mEngine->getFrameCount()].getVkBuffer(), 0, (vk::DeviceSize) shaderStorageBufferSize);
                    })
                    .addMapping([&](vklite::DescriptorMapping &mapping) {
                        mapping
                                .binding(2)
                                .descriptorType(vk::DescriptorType::eStorageBuffer)
                                .addBufferInfo(mShaderStorageBuffers[frameIndex].getVkBuffer(), 0, (vk::DeviceSize) shaderStorageBufferSize);
                    });
        });
        mTimer.start();
        LOG_D("test created ");
    }

    void Test08ComputeShader::init() {

    }

    // 检查是否准备好
    bool Test08ComputeShader::isReady() {
        return true;
    }

    // 绘制帧
    void Test08ComputeShader::drawFrame() {
        UniformBufferObject ubo{};
        ubo.deltaTime = mTimer.getDeltaTimeMs() * 2.0f;
        LOG_D("ubo.deltaTime: %f", ubo.deltaTime);
        mUniformBuffers[mEngine->getFrameIndex()].update(mEngine->getCommandPool(), &ubo, sizeof(UniformBufferObject));

        mEngine->drawIndexed(mParticleCount);
    }

    // 清理操作
    void Test08ComputeShader::cleanup() {
        LOG_I("Cleaning up %s", getName().c_str());
    }
} // test