//
// Created by leixing on 2025/3/7.
//

#pragma once

#include <memory>

#include "Surface.h"
#include "vklite/instance/Instance.h"

namespace vklite {

    class SurfaceBuilder {

    public:
        virtual ~SurfaceBuilder() = default;

        [[nodiscard]]
        virtual std::unique_ptr<Surface> build(const Instance &) const = 0;
    };
}