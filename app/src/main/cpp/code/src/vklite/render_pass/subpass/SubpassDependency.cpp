//
// Created by leixing on 2025-06-18.
//

#include "SubpassDependency.h"

namespace vklite {

    SubpassDependency::SubpassDependency(SubpassDependencyItem src,
                                         SubpassDependencyItem dst,
                                         vk::DependencyFlags flags)
            : mSrc(src), mDst(dst), mFlags(flags) {}

    SubpassDependency::~SubpassDependency() = default;

    SubpassDependency::SubpassDependency(const SubpassDependency &other)
            : mSrc(other.mSrc), mDst(other.mDst), mFlags(other.mFlags) {}

    SubpassDependency &SubpassDependency::operator=(const SubpassDependency &other) {
        if (this != &other) {
            mSrc = other.mSrc;
            mDst = other.mDst;
            mFlags = other.mFlags;
        }
        return *this;
    }

    SubpassDependency::SubpassDependency(SubpassDependency &&other) noexcept
            : mSrc(std::move(other.mSrc)), mDst(std::move(other.mDst)), mFlags(other.mFlags) {}

    SubpassDependency &SubpassDependency::operator=(SubpassDependency &&other) noexcept {
        if (this != &other) {
            mSrc = std::move(other.mSrc);
            mDst = std::move(other.mDst);
            mFlags = other.mFlags;
        }
        return *this;
    }

    [[nodiscard]]
    vk::SubpassDependency SubpassDependency::createSubpassDependency() const {
        return {
                mSrc.getSubpassIndex(),
                mDst.getSubpassIndex(),
                mSrc.getStage(),
                mDst.getStage(),
                mSrc.getAccess(),
                mDst.getAccess(),
                mFlags
        };
    }

} // vklite