//
// Created by leixing on 2024/12/16.
//

#pragma once

#include <vector>
#include <functional>
#include <memory>

#include "vklite/instance/Instance.h"
#include "vklite/surface/Surface.h"
#include "vklite/physical_device/PhysicalDevice.h"
#include "vklite/device/Device.h"
#include "vklite/swapchain/Swapchain.h"
#include "vklite/render_pass/RenderPass.h"
#include "vklite/pipeline/graphics_pipeline/GraphicsPipeline.h"
#include "vklite/pipeline/compute_pipeline/ComputePipeline.h"
#include "vklite/pipeline/pipeline_resource/PipelineResource.h"
#include "vklite/command_buffer/CommandPool.h"
#include "vklite/vertex_buffer/VertexBuffer.h"
#include "vklite/buffer/host_visible/HostVisibleVertexBuffer.h"
#include "vklite/index_buffer/IndexBuffer.h"
#include "vklite/buffer/host_visible/HostVisibleIndexBuffer.h"
#include "vklite/buffer/device_local/DeviceLocalUniformBuffer.h"
#include "vklite/sync/SyncObject.h"
#include "vklite/frame_buffer/FrameBuffer.h"
#include "vklite/sampler/DefaultSampler.h"
#include "vklite/util/selector/StringListSelector.h"
#include "vklite/physical_device/PhysicalDeviceSelector.h"


namespace vklite {

    class VkLiteEngine {
    private:
        bool mFrameBufferResized = false;
        uint32_t mFrameCount = 2;
        uint32_t mCurrentFrameIndex = 0;
//        const std::array<float, 4> mClearColor = {0.05f, 0.05f, 0.05f, 1.0f};
        const std::array<float, 4> mClearColor = {0.2f, 0.4f, 0.6f, 1.0f};
        const std::array<float, 4> mDepthStencil = {1.0f, 0, 0, 0};

        std::unique_ptr<Instance> mInstance;
        std::unique_ptr<Surface> mSurface;
        std::unique_ptr<PhysicalDevice> mPhysicalDevice;
        std::unique_ptr<Device> mDevice;

        std::unique_ptr<Swapchain> mSwapchain;
        std::unique_ptr<RenderPass> mRenderPass;

        std::unique_ptr<GraphicsPipeline> mGraphicsPipeline;
        std::unique_ptr<ComputePipeline> mComputePipeline;

        std::vector<PipelineResource> mPipelineResources;

        std::unique_ptr<CommandPool> mCommandPool;
        std::unique_ptr<FrameBuffer> mFrameBuffer;

        std::unique_ptr<SyncObject> mSyncObject;

    public:
        VkLiteEngine(std::unique_ptr<Instance> instance,
                     std::unique_ptr<Surface> surface,
                     std::unique_ptr<PhysicalDevice> physicalDevice,
                     std::unique_ptr<Device> device,
                     std::unique_ptr<CommandPool> commandPool,
                     std::unique_ptr<Swapchain> swapchain,
                     std::unique_ptr<RenderPass> renderPass,
                     std::unique_ptr<GraphicsPipeline> graphicsPipeline,
                     std::unique_ptr<ComputePipeline> computePipeline,
                     std::vector<PipelineResource> &&pipelineResources,
                     std::unique_ptr<FrameBuffer> frameBuffer,
                     std::unique_ptr<SyncObject> syncObject,
                     uint32_t frameCount);

        ~VkLiteEngine();

        [[nodiscard]]
        vk::Instance getVKInstance() const;

        [[nodiscard]]
        vk::Device getVKDevice() const;

        [[nodiscard]]
        const CommandPool &getCommandPool() const;

        [[nodiscard]]
        uint32_t getCurrentFrameIndex() const;

        [[nodiscard]]
        GraphicsPipeline &getGraphicsPipeline() const;

        VkLiteEngine &createVertexBuffer(size_t size);

        VkLiteEngine &createVertexBuffer(uint32_t binding, size_t size);

        VkLiteEngine &createVertexBuffer(uint32_t pipelineIndex, uint32_t binding, size_t size);

        VkLiteEngine &updateVertexBuffer(const void *data, size_t size);

        VkLiteEngine &updateVertexBuffer(uint32_t index, const void *data, size_t size);

        template<typename T>
        VkLiteEngine &updateVertexBuffer(const std::vector<T> &data) {
            return updateVertexBuffer(0, data);
        }

        template<typename T>
        VkLiteEngine &updateVertexBuffer(uint32_t index, const std::vector<T> &data) {
//            getGraphicsPipeline().updateVertexBuffer(*mCommandPool, data.data(), data.size() * sizeof(T));
            return *this;
        }

        VkLiteEngine &createIndexBuffer(size_t size);

        VkLiteEngine &updateIndexBuffer(const std::vector<uint32_t> &indices);

        VkLiteEngine &updateUniformBuffer(uint32_t frameIndex, uint32_t set, uint32_t binding, const void *data, uint32_t size);

        template<class T>
        VkLiteEngine &updateUniformBuffer(uint32_t frameIndex, uint32_t set, uint32_t binding, const T &data) {
//            getGraphicsPipeline().updateUniformBuffer(*mCommandPool, frameIndex, set, binding, static_cast<const void *>(&data), sizeof(T));
            return *this;
        }

        VkLiteEngine &updateCurrentFrameUniformBuffer(uint32_t set, uint32_t binding, const void *data, uint32_t size);

        template<class T>
        VkLiteEngine &updateCurrentFrameUniformBuffer(uint32_t set, uint32_t binding, const T &data) {
            return updateCurrentFrameUniformBuffer(set, binding, static_cast<const void *>(&data), sizeof(T));
        }

        VkLiteEngine &updatePushConstant(uint32_t index, const void *data);

        void drawFrame();

    private:
        void recreateSwapChain();
    };


} // vklite
