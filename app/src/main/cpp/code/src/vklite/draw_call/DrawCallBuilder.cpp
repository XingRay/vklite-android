//
// Created by leixing on 2025/5/12.
//

#include "DrawCallBuilder.h"

namespace vklite {
    DrawCallBuilder::DrawCallBuilder() {

    }

    DrawCallBuilder::~DrawCallBuilder() {

    }

    DrawCall DrawCallBuilder::build() {
        return DrawCall();
    }

    std::unique_ptr<DrawCall> DrawCallBuilder::buildUnique() {
        return std::unique_ptr<DrawCall>();
    }

} // vklite