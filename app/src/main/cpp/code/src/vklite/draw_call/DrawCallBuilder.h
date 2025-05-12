//
// Created by leixing on 2025/5/12.
//

#pragma once

#include <memory>
#include <cstdint>

#include "vklite/draw_call/DrawCall.h"

namespace vklite {

    class DrawCallBuilder {
    private:
    public:
        DrawCallBuilder();

        ~DrawCallBuilder();

        DrawCall build();

        std::unique_ptr<DrawCall> buildUnique();
    };

} // vklite
