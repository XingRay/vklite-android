//
// Created by leixing on 2025/4/27.
//

#include "AndroidSurfaceBuilder.h"
#include "vklite/platform/android/surface/AndroidSurface.h"

namespace vklite {

    AndroidSurfaceBuilder::AndroidSurfaceBuilder(ANativeWindow *window)
            : mWindow(window) {}

    AndroidSurfaceBuilder::~AndroidSurfaceBuilder() = default;

    std::unique_ptr<Surface> AndroidSurfaceBuilder::build(const Instance &instance) const {
        return std::make_unique<AndroidSurface>(instance, mWindow);
    }

} // vklite