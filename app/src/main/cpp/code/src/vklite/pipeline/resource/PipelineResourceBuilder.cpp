//
// Created by leixing on 2025/4/25.
//

#include "PipelineResourceBuilder.h"

namespace vklite {

    PipelineResourceBuilder::PipelineResourceBuilder() {

    }

    PipelineResourceBuilder::~PipelineResourceBuilder() {

    }

    PipelineResourceBuilder &PipelineResourceBuilder::index(const std::function<void(IndexBufferBuilder &)> &configure) {
        IndexBufferBuilder config{};
        configure(config);
        mIndexConfigure = config;
        return *this;
    }

//    PipelineResourceBuilder &PipelineResourceBuilder::index(uint32_t capacity) {
//        mIndexConfigure.setIndexBuffer(capacity);
//        return *this;
//    }
//
//    PipelineResourceBuilder &PipelineResourceBuilder::index(uint32_t capacity, std::vector<uint32_t> &&indices) {
//        mIndexConfigure.setIndexBuffer(capacity, std::move(indices));
//        return *this;
//    }
//
//    PipelineResourceBuilder &PipelineResourceBuilder::index(std::vector<uint32_t> &&indices) {
//        mIndexConfigure.setIndexBuffer(std::move(indices));
//        return *this;
//    }
//
//    PipelineResourceBuilder &PipelineResourceBuilder::index(const std::shared_ptr<IndexBuffer> &indexBuffer) {
//        mIndexConfigure.setIndexBuffer(indexBuffer);
//        return *this;
//    }

    std::vector<PipelineResource> PipelineResourceBuilder::build(uint32_t frameCount) {
        std::vector<PipelineResource> pipelineResources;


        return pipelineResources;
    }

} // vklite