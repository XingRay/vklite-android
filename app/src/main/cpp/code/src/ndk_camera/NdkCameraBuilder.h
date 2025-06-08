//
// Created by leixing on 2025/6/8.
//

#pragma once

#include <memory>

#include "ndk_camera/NdkCamera.h"

namespace ndkcamera {

    class NdkCameraBuilder {
    private:

    public:
        NdkCamera build();

        std::unique_ptr<NdkCamera> buildUnique();
    };

} // ndk_camera
