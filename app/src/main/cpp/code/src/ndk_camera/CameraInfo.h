//
// Created by leixing on 2025/1/20.
//

#pragma once

#include <string>
#include "ndk_camera/CameraMetadata.h"

namespace ndkcamera {

    class CameraInfo {
    public:
        const char *id;
        CameraLensFacing lensFacing;
        SupportedHardwareLevel supportedHardwareLevel;
    };

} // ndkcamera
