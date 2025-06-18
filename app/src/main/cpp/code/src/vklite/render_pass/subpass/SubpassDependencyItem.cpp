//
// Created by leixing on 2025-06-18.
//

#include "SubpassDependencyItem.h"

namespace vklite {

    SubpassDependencyItem::SubpassDependencyItem(uint32_t subpassIndex,
                                                 vk::PipelineStageFlags stage,
                                                 vk::AccessFlags access)
            : mSubpassIndex(subpassIndex), mStage(stage), mAccess(access) {}

    SubpassDependencyItem::~SubpassDependencyItem() = default;

    SubpassDependencyItem::SubpassDependencyItem(const SubpassDependencyItem &other) = default;

    SubpassDependencyItem &SubpassDependencyItem::operator=(const SubpassDependencyItem &other) {
        if (this != &other) {
            mSubpassIndex = other.mSubpassIndex;
            mStage = other.mStage;
            mAccess = other.mAccess;
        }
        return *this;
    }

    SubpassDependencyItem::SubpassDependencyItem(SubpassDependencyItem &&other) noexcept
            : mSubpassIndex(other.mSubpassIndex),
              mStage(other.mStage),
              mAccess(other.mAccess) {}

    SubpassDependencyItem &SubpassDependencyItem::operator=(SubpassDependencyItem &&other) noexcept {
        if (this != &other) {
            mSubpassIndex = other.mSubpassIndex;
            mStage = other.mStage;
            mAccess = other.mAccess;
        }
        return *this;
    }

    [[nodiscard]]
    uint32_t SubpassDependencyItem::getSubpassIndex() const {
        return mSubpassIndex;
    }

    [[nodiscard]]
    vk::PipelineStageFlags SubpassDependencyItem::getStage() const {
        return mStage;
    }

    [[nodiscard]]
    vk::AccessFlags SubpassDependencyItem::getAccess() const {
        return mAccess;
    }

} // vklite