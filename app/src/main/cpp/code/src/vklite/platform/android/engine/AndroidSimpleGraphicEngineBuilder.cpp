//
// Created by leixing on 2025/6/4.
//

#include "AndroidSimpleGraphicEngineBuilder.h"

#include "vklite/platform/android/surface/AndroidSurfaceBuilder.h"
#include "vklite/platform/android/plugin/AndroidSurfacePlugin.h"

namespace vklite {
    /**
     * preset
     */
    SimpleGraphicEngineBuilder AndroidSimpleGraphicEngineBuilder::asDefault(ANativeWindow *window) {
        SimpleGraphicEngineBuilder simpleGraphicEngineBuilder;

        simpleGraphicEngineBuilder.asDefault()
                .addInstancePlugin(AndroidSurfacePlugin::buildUniqueCombined())
                .addDevicePlugin(AndroidSurfacePlugin::buildUniqueCombined())
                .surfaceBuilder([=](const vklite::Instance &instance) {
                    return AndroidSurfaceBuilder()
                            .instance(instance.getInstance())
                            .nativeWindow(window)
                            .buildUnique();
                });

        return std::move(simpleGraphicEngineBuilder);
    }

} // vklite