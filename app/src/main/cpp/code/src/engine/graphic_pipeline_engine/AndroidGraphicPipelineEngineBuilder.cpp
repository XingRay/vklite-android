//
// Created by leixing on 2025/6/4.
//

#include "AndroidGraphicPipelineEngineBuilder.h"

#include "vklite/platform/android/surface/AndroidSurfaceBuilder.h"
#include "vklite/platform/android/plugin/AndroidSurfacePlugin.h"

namespace vklite {
    /**
     * preset
     */
    GraphicPipelineEngineBuilder AndroidGraphicPipelineEngineBuilder::asDefault(ANativeWindow *window) {
        GraphicPipelineEngineBuilder graphicPipelineEngineBuilder;

        graphicPipelineEngineBuilder.asDefault()
                .addInstancePlugin(AndroidSurfacePlugin::buildUniqueCombined())
                .addDevicePlugin(AndroidSurfacePlugin::buildUniqueCombined())
                .surfaceBuilder([=](const vklite::Instance &instance) {
                    return AndroidSurfaceBuilder()
                            .instance(instance.getVkInstance())
                            .nativeWindow(window)
                            .buildUnique();
                });

        return std::move(graphicPipelineEngineBuilder);
    }

} // vklite