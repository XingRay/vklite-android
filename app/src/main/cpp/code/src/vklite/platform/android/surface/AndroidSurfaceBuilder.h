//
// Created by leixing on 2025/4/27.
//

#pragma once

#include "android/native_window.h"
#include <vulkan/vulkan.hpp>

#include "vklite/surface/Surface.h"
#include "vklite/instance/Instance.h"
#include "vklite/surface/SurfaceBuilder.h"

namespace vklite {

    class AndroidSurfaceBuilder : public SurfaceBuilder {
    private:
        ANativeWindow *mWindow;

    public:
        explicit AndroidSurfaceBuilder(ANativeWindow *window);

        ~AndroidSurfaceBuilder() override;

        [[nodiscard]]
        std::unique_ptr<Surface> build(const Instance &instance) const override;
    };

} // vklite
