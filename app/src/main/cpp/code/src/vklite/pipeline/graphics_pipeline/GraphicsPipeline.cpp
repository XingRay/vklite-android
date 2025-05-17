//
// Created by leixing on 2024/12/28.
//

#include "GraphicsPipeline.h"
#include "vklite/Log.h"

namespace vklite {

    GraphicsPipeline::GraphicsPipeline(const Device &device,
                                       const RenderPass &renderPass,
                                       const VulkanShaderModule &vertexShaderModule,
                                       const VulkanShaderModule &fragmentShaderModule,
                                       const std::vector<vk::VertexInputBindingDescription> &vertexInputBindingDescriptions,
                                       const std::vector<vk::VertexInputAttributeDescription> &vertexInputAttributeDescriptions,
                                       const PipelineLayout &pipelineLayout,
                                       const std::vector<vk::Viewport> &viewports,
                                       const std::vector<vk::Rect2D> &scissors,
                                       bool sampleShadingEnable,
                                       vk::SampleCountFlagBits sampleCount,
                                       bool depthTestEnable)
            : mDevice(device) {

        vk::Device vkDevice = device.getDevice();

        // input assembler
        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
        inputAssemblyStateCreateInfo
                .setTopology(vk::PrimitiveTopology::eTriangleList)
                .setPrimitiveRestartEnable(vk::False);


        std::vector<vk::DynamicState> dynamicStages = {
                vk::DynamicState::eViewport,
                vk::DynamicState::eScissor
        };

        vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
        dynamicStateCreateInfo
                .setDynamicStates(dynamicStages);

        vk::PipelineViewportStateCreateInfo viewportStateCreateInfo;
        viewportStateCreateInfo
                .setViewports(viewports)
                .setScissors(scissors);

        // vertex shader
        vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;
        vertexInputStateCreateInfo
                .setVertexBindingDescriptions(vertexInputBindingDescriptions)
                .setVertexAttributeDescriptions(vertexInputAttributeDescriptions);

        vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo;
        vertexShaderStageCreateInfo
                .setStage(vk::ShaderStageFlagBits::eVertex)
                .setModule(vertexShaderModule.getShaderModule())
                .setPName("main")
                .setPSpecializationInfo(nullptr);

        // tessellation

        // geometry shader

        // rasterization
        vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
        rasterizationStateCreateInfo
                // 如果depthClampEnable设置为VK_TRUE，则超出近平面和远平面的片段将被夹紧，而不是丢弃它们。
                // 这在某些特殊情况下很有用，例如阴影贴图。使用此功能需要启用 GPU 功能。
                .setDepthClampEnable(vk::False)
                        // 如果rasterizerDiscardEnable设置为VK_TRUE，则几何图形永远不会通过光栅化阶段。这基本上禁用了帧缓冲区的任何输出。
                .setRasterizerDiscardEnable(vk::False)
                        //确定polygonMode如何为几何体生成片段。可以使用以下模式：
                        //VK_POLYGON_MODE_FILL：用碎片填充多边形区域
                        //VK_POLYGON_MODE_LINE：多边形边缘绘制为线
                        //VK_POLYGON_MODE_POINT：多边形顶点绘制为点
                .setPolygonMode(vk::PolygonMode::eFill)
                        // 使用填充以外的任何模式都需要设置 lineWidth :
                .setLineWidth(1.0f)
                        // 设置面剔除策略, 这里设置为反面被剔除
                .setCullMode(vk::CullModeFlagBits::eBack)
                        // 设置正面的方向
                .setFrontFace(vk::FrontFace::eCounterClockwise)
                .setDepthBiasEnable(vk::False)
                .setDepthBiasConstantFactor(0.0f)
                .setDepthBiasClamp(0.0f)
                .setDepthBiasSlopeFactor(0.0f);

        // depth & stencil testing
        vk::PipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;
        depthStencilStateCreateInfo
                .setDepthTestEnable(depthTestEnable)
                .setDepthWriteEnable(vk::True)
                .setDepthCompareOp(vk::CompareOp::eLess)
                .setDepthBoundsTestEnable(vk::False)
                .setMinDepthBounds(0.0f)
                .setMaxDepthBounds(1.0f)
                .setStencilTestEnable(vk::False)
                .setFront(vk::StencilOpState{})
                .setBack(vk::StencilOpState{});


        // Multisampling
        vk::PipelineMultisampleStateCreateInfo multiSampleStateCreateInfo;
        LOG_D("PipelineMultisampleStateCreateInfo, sampleShadingEnable:%d, sampleCount:%d", sampleShadingEnable, sampleCount);
        multiSampleStateCreateInfo
                // 并非直接控制MSAA的开关，而是控制逐样本着色（Sample Shading）的启用状态,
                // 设置为true时，会启用样本着色，即对每个样本执行独立的着色计算，这可以提高图像质量但增加计算开销。
                // 设置为false时，仅对每个像素执行一次着色计算，结果被复制到所有样本，节省性能但可能降低质量。
                .setSampleShadingEnable(sampleShadingEnable)
                        // 光栅化时的采样数, 采样数越多，抗锯齿效果越好，但性能开销也越大
                        // 当传入 vk::SampleCountFlagBits::e1 时，关闭MSAA（单采样模式）,禁用多重采样（默认）。
                        // 传入 vk::SampleCountFlagBits::e2、e4、e8、e16 等时，开启MSAA(多重采样)，采样数对应抗锯齿倍数
                .setRasterizationSamples(sampleCount)
                        // 最小采样着色率, 仅在 sampleShadingEnable 为 vk::True 时有效
                        // 0.0：禁用采样着色, 禁用（默认）。
                        // 1.0：对每个采样点执行完整的片段着色器, 最高质量。
                        // 0.2：对 20% 的采样点执行片段着色器
                .setMinSampleShading(0.2f)
                        // 采样掩码（Sample Mask），用于控制哪些采样点被启用,
                        // 每个采样点对应掩码中的一个位（bit），1 表示启用，0 表示禁用。
                .setPSampleMask(nullptr)
                        // 启用或禁用 Alpha 到覆盖（Alpha-to-Coverage）。
                        // 如果启用，片段的 Alpha 值将用于生成覆盖掩码，从而实现透明物体的抗锯齿
                .setAlphaToCoverageEnable(vk::False)
                        // 启用或禁用 Alpha 到一（Alpha-to-One）
                        // 如果启用，片段的 Alpha 值将被强制设置为 1.0
                .setAlphaToOneEnable(vk::False);

        // fragment shader
        vk::PipelineShaderStageCreateInfo fragmentShaderStageCreateInfo;
        fragmentShaderStageCreateInfo
                .setStage(vk::ShaderStageFlagBits::eFragment)
                .setModule(fragmentShaderModule.getShaderModule())
                .setPName("main")
                .setPSpecializationInfo(nullptr);

        // color blending
        vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{};
        colorBlendAttachmentState
//                .setBlendEnable(vk::True)
                .setBlendEnable(vk::False)
                .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG
                                   | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
                .setSrcColorBlendFactor(vk::BlendFactor::eSrcAlpha)
                .setDstColorBlendFactor(vk::BlendFactor::eOneMinusSrcAlpha)
                .setColorBlendOp(vk::BlendOp::eAdd)
                .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
                .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
                .setAlphaBlendOp(vk::BlendOp::eAdd);
        std::array<vk::PipelineColorBlendAttachmentState, 1> colorBlendAttachmentStates = {colorBlendAttachmentState};

        vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
        colorBlendStateCreateInfo
                .setLogicOpEnable(vk::False)
                .setLogicOp(vk::LogicOp::eCopy)
                .setAttachments(colorBlendAttachmentStates)
                .setBlendConstants(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f});


        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageCreateInfos = {vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo};

        vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
        graphicsPipelineCreateInfo
                .setStages(shaderStageCreateInfos)
                .setPVertexInputState(&vertexInputStateCreateInfo)
                .setPInputAssemblyState(&inputAssemblyStateCreateInfo)
                .setPViewportState(&viewportStateCreateInfo)
                .setPRasterizationState(&rasterizationStateCreateInfo)
                .setPMultisampleState(&multiSampleStateCreateInfo)
                .setPDepthStencilState(&depthStencilStateCreateInfo)
                .setPColorBlendState(&colorBlendStateCreateInfo)
                .setPDynamicState(&dynamicStateCreateInfo)
                .setLayout(pipelineLayout.getPipelineLayout())
                .setRenderPass(renderPass.getRenderPass())
                .setSubpass(0)
                .setBasePipelineHandle(nullptr)
                .setBasePipelineIndex(-1);

        auto [result, pipeline] = vkDevice.createGraphicsPipeline(nullptr, graphicsPipelineCreateInfo);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("createGraphicsPipelines failed");
        }
        mPipeline = pipeline;

    }

    GraphicsPipeline::~GraphicsPipeline() {
        LOG_D("GraphicsPipeline::~GraphicsPipeline");
        const vk::Device &vkDevice = mDevice.getDevice();
        vkDevice.destroy(mPipeline);
    }

    const vk::Pipeline &GraphicsPipeline::getPipeline() const {
        return mPipeline;
    }

    void GraphicsPipeline::drawFrame(const vk::CommandBuffer &commandBuffer,
                                     const PipelineLayout &pipelineLayout,
                                     const PipelineResource &pipelineResource,
                                     const std::vector<vk::Viewport> &viewports,
                                     const std::vector<vk::Rect2D> &scissors) {

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, mPipeline);

        /**
         * firstViewport 在某些情况下，可能需要将视口绑定到特定的范围，而不是从索引 0 开始
         * 类似于 copy中的 dst_Index [s,s,s] -> [_,_,_, d,d,d, _,_,...] (firstViewport=3)
         */
        commandBuffer.setViewport(0, viewports);
        commandBuffer.setScissor(0, scissors);

        // vertex buffer
        /**
         * firstBinding: shader 代码中 location 值即为绑定点, 参数给出多个buffer时, 对用连续的多个绑定点, 如 location = 1 2
         * offsets 参数仅指定顶点数据的起始位置，
         * 读取的数据量由绘制命令（如 vkCmdDraw 或 vkCmdDrawIndexed）的顶点数量决定，结合顶点输入绑定描述中的 stride 参数隐式计算总字节数
         * 总字节数 = vertexCount × stride
         * 示例:
         * 起始位置：offsets[0]（如 256 字节）
         * 结束位置：offsets[0] + vertexCount × stride（如 256 + 3×20 = 316 字节）
         */
        commandBuffer.bindVertexBuffers(0, pipelineResource.getVertexBuffers(), pipelineResource.getVertexBufferOffsets());

        // index buffer
        commandBuffer.bindIndexBuffer(pipelineResource.getIndexBuffer(), pipelineResource.getIndexBufferOffset(), pipelineResource.getIndexType());

        // uniform/sampler/storage/...
        commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout.getPipelineLayout(), 0, pipelineResource.getDescriptorSets(), nullptr);

        // push constants
        const std::vector<PushConstant> &pushConstants = pipelineResource.getPushConstants();
        for (const PushConstant &pushConstant: pushConstants) {
            commandBuffer.pushConstants(pipelineLayout.getPipelineLayout(),
                                        pushConstant.getStageFlags(),
                                        pushConstant.getOffset(),
                                        pushConstant.getSize(),
                                        pushConstant.getData());
        }

        // draw call
        /**
         * uint32_t           indexCount,
         * uint32_t           instanceCount,
         * uint32_t           firstIndex,
         * int32_t            vertexOffset,
         * uint32_t           firstInstance,
         */
        commandBuffer.drawIndexed(pipelineResource.getIndicesCount(), 1, 0, 0, 0);
    }

} // vklite