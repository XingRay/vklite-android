//
// Created by leixing on 2025/3/7.
//

#include "GraphicsPipelineBuilder.h"
#include "vklite/pipeline/shader_module/ShaderModuleBuilder.h"

namespace vklite {

    GraphicsPipelineBuilder::GraphicsPipelineBuilder()
            : mBlendConstants(std::array<float, 4>{0.0f, 0.0f, 0.0f, 0.0f}) {

        mInputAssemblyStateCreateInfo = vk::PipelineInputAssemblyStateCreateInfo{};
        mInputAssemblyStateCreateInfo
                .setTopology(vk::PrimitiveTopology::eTriangleList)
                .setPrimitiveRestartEnable(vk::False);

        vk::PipelineShaderStageCreateInfo vertexShaderStageCreateInfo{};
        vertexShaderStageCreateInfo
                .setStage(vk::ShaderStageFlagBits::eVertex)
                .setPName("main")
                .setPSpecializationInfo(nullptr);

        vk::PipelineShaderStageCreateInfo fragmentShaderStageCreateInfo{};
        fragmentShaderStageCreateInfo
                .setStage(vk::ShaderStageFlagBits::eFragment)
                .setPName("main")
                .setPSpecializationInfo(nullptr);

        mShaderStageCreateInfos = {vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo};
        mShaderStageCreateInfoMap.emplace(vk::ShaderStageFlagBits::eVertex, std::ref(mShaderStageCreateInfos[0]));
        mShaderStageCreateInfoMap.emplace(vk::ShaderStageFlagBits::eFragment, std::ref(mShaderStageCreateInfos[1]));

        mMultiSampleStateCreateInfo
                // 并非直接控制MSAA的开关，而是控制逐样本着色（Sample Shading）的启用状态,
                // 设置为true时，会启用样本着色，即对每个样本执行独立的着色计算，这可以提高图像质量但增加计算开销。
                // 设置为false时，仅对每个像素执行一次着色计算，结果被复制到所有样本，节省性能但可能降低质量。
                .setSampleShadingEnable(false)
                        // 光栅化时的采样数, 采样数越多，抗锯齿效果越好，但性能开销也越大
                        // 当传入 vk::SampleCountFlagBits::e1 时，关闭MSAA（单采样模式）,禁用多重采样（默认）。
                        // 传入 vk::SampleCountFlagBits::e2、e4、e8、e16 等时，开启MSAA(多重采样)，采样数对应抗锯齿倍数
                .setRasterizationSamples(vk::SampleCountFlagBits::e1)
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


        mDynamicStages = {
                vk::DynamicState::eViewport,
                vk::DynamicState::eScissor
        };
        mDynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo{};
        mDynamicStateCreateInfo
                .setDynamicStates(mDynamicStages);


        mViewportStateCreateInfo = vk::PipelineViewportStateCreateInfo{};
        mViewportStateCreateInfo
                .setViewports(mViewports)
                .setScissors(mScissors);


        mRasterizationStateCreateInfo
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


        mDepthStencilStateCreateInfo
                .setDepthTestEnable(false)
                .setDepthWriteEnable(vk::True)
                .setDepthCompareOp(vk::CompareOp::eLess)
                .setDepthBoundsTestEnable(vk::False)
                .setMinDepthBounds(0.0f)
                .setMaxDepthBounds(1.0f)
                .setStencilTestEnable(vk::False)
                .setFront(vk::StencilOpState{})
                .setBack(vk::StencilOpState{});


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
        mColorBlendAttachmentStates.push_back(colorBlendAttachmentState);

        mColorBlendStateCreateInfo
                .setLogicOpEnable(vk::False)
                .setLogicOp(vk::LogicOp::eCopy)
                .setAttachments(mColorBlendAttachmentStates)
                .setBlendConstants(mBlendConstants);

        mGraphicsPipelineCreateInfo
                .setStages(mShaderStageCreateInfos)
                .setPVertexInputState(&mVertexInputStateCreateInfo)
                .setPInputAssemblyState(&mInputAssemblyStateCreateInfo)
                .setPViewportState(&mViewportStateCreateInfo)
                .setPRasterizationState(&mRasterizationStateCreateInfo)
                .setPMultisampleState(&mMultiSampleStateCreateInfo)
                .setPDepthStencilState(&mDepthStencilStateCreateInfo)
                .setPColorBlendState(&mColorBlendStateCreateInfo)
                .setPDynamicState(&mDynamicStateCreateInfo)
                .setSubpass(0)
                .setBasePipelineHandle(nullptr)
                .setBasePipelineIndex(-1);
    };

    GraphicsPipelineBuilder::~GraphicsPipelineBuilder() = default;

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::device(const vk::Device &device) {
        mDevice = device;
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::renderPass(const vk::RenderPass &renderPass) {
        mGraphicsPipelineCreateInfo.setRenderPass(renderPass);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::pipelineLayout(const vk::PipelineLayout &pipelineLayout) {
        mGraphicsPipelineCreateInfo.setLayout(pipelineLayout);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::viewports(const std::vector<vk::Viewport> &viewports) {
        mViewports = viewports;
        mViewportStateCreateInfo.setViewports(mViewports);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::viewports(std::vector<vk::Viewport> &&viewports) {
        mViewports = std::move(viewports);
        mViewportStateCreateInfo.setViewports(mViewports);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::scissors(const std::vector<vk::Rect2D> &scissors) {
        mScissors = scissors;
        mViewportStateCreateInfo.setScissors(mScissors);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::scissors(std::vector<vk::Rect2D> &&scissors) {
        mScissors = std::move(scissors);
        mViewportStateCreateInfo.setScissors(mScissors);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::topology(vk::PrimitiveTopology topology) {
        mInputAssemblyStateCreateInfo.setTopology(topology);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::vertexShader(std::unique_ptr<ShaderModule> vertexShaderModule) {
        mVertexShaderModule = std::move(vertexShaderModule);
//        mShaderStageCreateInfos[0].setModule(mFragmentShaderModule->getShaderModule());
        mShaderStageCreateInfoMap.at(vk::ShaderStageFlagBits::eVertex).get().setModule(mVertexShaderModule->getVkShaderModule());
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::vertexBindingDescriptions(std::vector<vk::VertexInputBindingDescription> &&vertexBindingDescriptions) {
        mVertexBindingDescriptions = std::move(vertexBindingDescriptions);
        mVertexInputStateCreateInfo.setVertexBindingDescriptions(mVertexBindingDescriptions);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::vertexAttributeDescriptions(std::vector<vk::VertexInputAttributeDescription> &&vertexAttributeDescriptions) {
        mVertexAttributeDescriptions = std::move(vertexAttributeDescriptions);
        mVertexInputStateCreateInfo.setVertexAttributeDescriptions(mVertexAttributeDescriptions);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::fragmentShader(std::unique_ptr<ShaderModule> fragmentShaderModule) {
        mFragmentShaderModule = std::move(fragmentShaderModule);
//        mShaderStageCreateInfos[1].setModule(mFragmentShaderModule->getShaderModule());
        mShaderStageCreateInfoMap.at(vk::ShaderStageFlagBits::eFragment).get().setModule(mFragmentShaderModule->getVkShaderModule());
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::sampleShadingEnable(bool sampleShadingEnable) {
        mMultiSampleStateCreateInfo.setSampleShadingEnable(sampleShadingEnable);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::sampleCount(vk::SampleCountFlagBits sampleCount) {
        mMultiSampleStateCreateInfo.setRasterizationSamples(sampleCount);
        return *this;
    }


    GraphicsPipelineBuilder &GraphicsPipelineBuilder::polygonMode(vk::PolygonMode polygonMode) {
        mRasterizationStateCreateInfo.setPolygonMode(polygonMode);
        return *this;
    }

    // 使用填充以外的任何模式都需要设置 lineWidth :
    GraphicsPipelineBuilder &GraphicsPipelineBuilder::lineWidth(float lineWidth) {
        mRasterizationStateCreateInfo.setLineWidth(lineWidth);
        return *this;
    }

    // 设置面剔除策略, 这里设置为反面被剔除
    GraphicsPipelineBuilder &GraphicsPipelineBuilder::cullMode(vk::CullModeFlags cullMode) {
        mRasterizationStateCreateInfo.setCullMode(cullMode);
        return *this;
    }

    // 设置正面的方向
    GraphicsPipelineBuilder &GraphicsPipelineBuilder::frontFace(vk::FrontFace frontFace) {
        mRasterizationStateCreateInfo.setFrontFace(frontFace);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::depthTestEnable(bool depthTestEnable) {
        mDepthStencilStateCreateInfo.setDepthTestEnable(depthTestEnable);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::subpass(uint32_t subpass) {
        mGraphicsPipelineCreateInfo.setSubpass(subpass);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::basePipelineHandle(vk::Pipeline basePipelineHandle) {
        mGraphicsPipelineCreateInfo.setBasePipelineHandle(basePipelineHandle);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::basePipelineIndex(int32_t basePipelineIndex) {
        mGraphicsPipelineCreateInfo.setBasePipelineIndex(basePipelineIndex);
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::pipelineCache(vk::PipelineCache pipelineCache) {
        mPipelineCache = pipelineCache;
        return *this;
    }

    GraphicsPipelineBuilder &GraphicsPipelineBuilder::configShader(vklite::ShaderConfigure &graphicShaderConfigure) {
        if (mDevice == nullptr) {
            throw std::runtime_error("");
        }

        std::unique_ptr<ShaderModule> vertexShader = ShaderModuleBuilder()
                .device(mDevice)
                .code(std::move(graphicShaderConfigure.getVertexShaderCode()))
                .buildUnique();

        std::unique_ptr<ShaderModule> fragmentShader = ShaderModuleBuilder()
                .device(mDevice)
                .code(std::move(graphicShaderConfigure.getFragmentShaderCode()))
                .buildUnique();

        (*this)
                .vertexShader(std::move(vertexShader))
                .vertexBindingDescriptions(graphicShaderConfigure.createVertexBindingDescriptions())
                .vertexAttributeDescriptions(graphicShaderConfigure.createVertexAttributeDescriptions())
                .fragmentShader(std::move(fragmentShader));

        return *this;
    }

    Pipeline GraphicsPipelineBuilder::build() {
        if (mDevice == nullptr) {
            throw std::runtime_error("GraphicsPipelineBuilder::build() mDevice == nullptr");
        }
        if (mGraphicsPipelineCreateInfo.renderPass == nullptr) {
            throw std::runtime_error("GraphicsPipelineBuilder::build() mGraphicsPipelineCreateInfo.renderPass == nullptr");
        }
        if (mGraphicsPipelineCreateInfo.layout == nullptr) {
            throw std::runtime_error("GraphicsPipelineBuilder::build() mGraphicsPipelineCreateInfo.layout == nullptr");
        }

        auto [result, pipeline] = mDevice.createGraphicsPipeline(mPipelineCache, mGraphicsPipelineCreateInfo);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("createGraphicsPipelines failed");
        }

        return Pipeline{mDevice, pipeline};
    }

    std::unique_ptr<Pipeline> GraphicsPipelineBuilder::buildUnique() {
        return std::make_unique<Pipeline>(build());
    }

} // vklite