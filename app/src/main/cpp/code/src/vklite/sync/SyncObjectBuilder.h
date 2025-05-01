//
// Created by leixing on 2025/5/1.
//

#pragma once

#include <memory>
#include <cstdint>

#include "vklite/sync/SyncObject.h"
#include "vklite/device/Device.h"

namespace vklite {

    class SyncObjectBuilder {
    private:
        uint32_t mFrameCount;

    public:
        SyncObjectBuilder();

        ~SyncObjectBuilder();

        SyncObjectBuilder &frameCount(uint32_t frameCount);

        std::unique_ptr<SyncObject> build(const Device &device);
    };

} // vklite
