//
// Created by leixing on 2025/5/29.
//

#include "SurfaceBuilder.h"

namespace vklite {

    SurfaceBuilder::SurfaceBuilder() = default;

    SurfaceBuilder::~SurfaceBuilder() = default;

    std::unique_ptr<Surface> SurfaceBuilder::buildUnique() const {
        return std::make_unique<Surface>(build());
    }
}