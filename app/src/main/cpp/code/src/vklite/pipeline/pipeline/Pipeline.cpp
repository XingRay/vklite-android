//
// Created by leixing on 2024/12/28.
//

#include "Pipeline.h"

#include <utility>

namespace vklite {

    Pipeline::Pipeline(vk::Device device, vk::Pipeline pipeline)
            : mDevice(device), mPipeline(pipeline) {}

    Pipeline::~Pipeline() {
        if (mDevice != nullptr && mPipeline != nullptr) {
            mDevice.destroy(mPipeline);
            mDevice = nullptr;
            mPipeline = nullptr;
        }
    }

    Pipeline::Pipeline(Pipeline &&other) noexcept
            : mDevice(std::exchange(other.mDevice, nullptr)),
              mPipeline(std::exchange(other.mPipeline, nullptr)) {}

    Pipeline &Pipeline::operator=(Pipeline &&other) noexcept {
        if (this != &other) {
            mDevice = std::exchange(other.mDevice, nullptr);
            mPipeline = std::exchange(other.mPipeline, nullptr);
        }
        return *this;
    }

    const vk::Pipeline &Pipeline::getVkPipeline() const {
        return mPipeline;
    }

//    void GraphicsPipeline::drawFrame(const vk::CommandBuffer &commandBuffer,
//                                     const PipelineLayout &pipelineLayout,
//                                     const PipelineResource &pipelineResource,
//                                     const std::vector<vk::Viewport> &viewports,
//                                     const std::vector<vk::Rect2D> &scissors) {
//
//        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline);
//
//        /**
//         * firstViewport 在某些情况下，可能需要将视口绑定到特定的范围，而不是从索引 0 开始
//         * 类似于 copy中的 dst_Index [s,s,s] -> [_,_,_, d,d,d, _,_,...] (firstViewport=3)
//         */
//        commandBuffer.setViewport(0, viewports);
//        commandBuffer.setScissor(0, scissors);
//
//        // vertex buffer
//        /**
//         * firstBinding: shader 代码中 location 值即为绑定点, 参数给出多个buffer时, 对用连续的多个绑定点, 如 location = 1 2
//         * offsets 参数仅指定顶点数据的起始位置，
//         * 读取的数据量由绘制命令（如 vkCmdDraw 或 vkCmdDrawIndexed）的顶点数量决定，结合顶点输入绑定描述中的 stride 参数隐式计算总字节数
//         * 总字节数 = vertexCount × stride
//         * 示例:
//         * 起始位置：offsets[0]（如 256 字节）
//         * 结束位置：offsets[0] + vertexCount × stride（如 256 + 3×20 = 316 字节）
//         */
//        commandBuffer.bindVertexBuffers(0, pipelineResource.getVertexBuffers(), pipelineResource.getVertexBufferOffsets());
//
//        // index buffer
//        commandBuffer.bindIndexBuffer(pipelineResource.getIndexBuffer(), pipelineResource.getIndexBufferOffset(), pipelineResource.getIndexType());
//
//        // uniform/sampler/storage/...
//        commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.getPipelineLayout(), 0, pipelineResource.getDescriptorSets(), nullptr);
//
//        // push constants
//        const std::vector<PushConstant> &pushConstants = pipelineResource.getPushConstants();
//        for (const PushConstant &pushConstant: pushConstants) {
//            commandBuffer.pushConstants(pipelineLayout.getPipelineLayout(),
//                                        pushConstant.getStageFlags(),
//                                        pushConstant.getOffset(),
//                                        pushConstant.getSize(),
//                                        pushConstant.getData());
//        }
//
//        // draw call
//        /**
//         * uint32_t           indexCount,
//         * uint32_t           instanceCount,
//         * uint32_t           firstIndex,
//         * int32_t            vertexOffset,
//         * uint32_t           firstInstance,
//         */
//        commandBuffer.drawIndexed(pipelineResource.getIndicesCount(), 1, 0, 0, 0);
//    }

} // vklite