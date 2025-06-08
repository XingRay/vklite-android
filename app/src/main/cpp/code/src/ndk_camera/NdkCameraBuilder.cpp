//
// Created by leixing on 2025/6/8.
//

#include "NdkCameraBuilder.h"

namespace ndkcamera {

    NdkCamera NdkCameraBuilder::build() {
        return NdkCamera();
    }

    std::unique_ptr<NdkCamera> NdkCameraBuilder::buildUnique() {
        return std::make_unique<NdkCamera>(build());
    }

} // ndk_camera