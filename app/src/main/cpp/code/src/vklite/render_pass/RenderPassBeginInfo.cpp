//
// Created by leixing on 2025/5/31.
//

#include "RenderPassBeginInfo.h"

namespace vklite {

    RenderPassBeginInfo::RenderPassBeginInfo(vk::RenderPassBeginInfo beginInfo, std::vector<vk::ClearValue> &&clearValues, vk::SubpassContents subpassContents)
            : mBeginInfo(beginInfo),
              mClearValues(std::move(clearValues)),
              mSubpassContents(subpassContents) {
        mBeginInfo.setClearValues(mClearValues);
    }

    RenderPassBeginInfo::RenderPassBeginInfo()
            : RenderPassBeginInfo(vk::RenderPassBeginInfo{}, {}, vk::SubpassContents::eInline) {}

    RenderPassBeginInfo::~RenderPassBeginInfo() = default;

    RenderPassBeginInfo::RenderPassBeginInfo(RenderPassBeginInfo &&other) noexcept
            : mBeginInfo(other.mBeginInfo),
              mClearValues(std::move(other.mClearValues)),
              mSubpassContents(other.mSubpassContents) {
        mBeginInfo.setClearValues(mClearValues);
    }

    RenderPassBeginInfo &RenderPassBeginInfo::operator=(RenderPassBeginInfo &&other) noexcept {
        if (this != &other) {
            mBeginInfo = other.mBeginInfo;
            mClearValues = std::move(other.mClearValues);
            mSubpassContents = other.mSubpassContents;

            mBeginInfo.setClearValues(mClearValues);
        }
        return *this;
    }

    RenderPassBeginInfo &RenderPassBeginInfo::subpassContents(vk::SubpassContents subpassContents) {
        mSubpassContents = subpassContents;
        return *this;
    }

    RenderPassBeginInfo &RenderPassBeginInfo::clearValues(std::vector<vk::ClearValue> &&clearValues) {
        mClearValues = std::move(clearValues);
        mBeginInfo.setClearValues(mClearValues);
        return *this;
    }

    RenderPassBeginInfo &RenderPassBeginInfo::updateClearValue(size_t index, vk::ClearValue clearValue) {
        mClearValues[index] = clearValue;
        return *this;
    }

    vk::RenderPassBeginInfo &RenderPassBeginInfo::getBeginInfo() {
        return mBeginInfo;
    }

    vk::SubpassContents RenderPassBeginInfo::getSubpassContents() {
        return mSubpassContents;
    }

} // vklite