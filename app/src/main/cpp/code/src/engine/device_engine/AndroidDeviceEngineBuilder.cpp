//
// Created by leixing on 2025/6/4.
//

#include "AndroidDeviceEngineBuilder.h"

#include "vklite/platform/android/surface/AndroidSurfaceBuilder.h"
#include "vklite/platform/android/plugin/AndroidSurfacePlugin.h"

namespace vklite {
    /**
     * preset
     */
    DeviceEngineBuilder AndroidDeviceEngineBuilder::asDefault(ANativeWindow *window) {
        DeviceEngineBuilder deviceEngineBuilder;

        deviceEngineBuilder.asDefault()
                .addInstancePlugin(AndroidSurfacePlugin::buildUniqueCombined())
                .addDevicePlugin(AndroidSurfacePlugin::buildUniqueCombined())
                .surfaceBuilder([=](const vklite::Instance &instance) {
                    return AndroidSurfaceBuilder()
                            .instance(instance.getVkInstance())
                            .nativeWindow(window)
                            .buildUnique();
                });

        return std::move(deviceEngineBuilder);
    }


    std::unique_ptr<DeviceEngineBuilder> AndroidDeviceEngineBuilder::asDefaultUnique(ANativeWindow *window) {
        return std::make_unique<DeviceEngineBuilder>(asDefault(window));
    }

} // vklite