//
// Created by leixing on 2025/3/7.
//

#pragma once

#include <memory>
#include <optional>

#include "Surface.h"
#include "vklite/instance/Instance.h"

namespace vklite {

    class SurfaceBuilder {
    private:

    public:
        SurfaceBuilder();

        virtual ~SurfaceBuilder();

        [[nodiscard]]
        virtual Surface build() const = 0;

        [[nodiscard]]
        std::unique_ptr<Surface> buildUnique() const;
    };
}