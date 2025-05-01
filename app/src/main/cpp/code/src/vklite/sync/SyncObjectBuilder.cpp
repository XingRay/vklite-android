//
// Created by leixing on 2025/5/1.
//

#include "SyncObjectBuilder.h"

namespace vklite {

    SyncObjectBuilder::SyncObjectBuilder()
            : mFrameCount(1) {}

    SyncObjectBuilder::~SyncObjectBuilder() = default;

    SyncObjectBuilder &SyncObjectBuilder::frameCount(uint32_t frameCount) {
        mFrameCount = frameCount;
        return *this;
    }

    std::unique_ptr<SyncObject> SyncObjectBuilder::build(const Device &device) {
        return std::make_unique<SyncObject>(device, mFrameCount);
    }

} // vklite