//
// Created by leixing on 2025-06-18.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/render_pass/subpass/SubpassDependencyItem.h"

namespace vklite {

    class SubpassDependency {
    private:
        SubpassDependencyItem mSrc;
        SubpassDependencyItem mDst;
        vk::DependencyFlags mFlags;

    public:
        explicit SubpassDependency(SubpassDependencyItem src,
                                   SubpassDependencyItem dst,
                                   vk::DependencyFlags flags);

        ~SubpassDependency();

        SubpassDependency(const SubpassDependency &other);

        SubpassDependency &operator=(const SubpassDependency &other);

        SubpassDependency(SubpassDependency &&other) noexcept;

        SubpassDependency &operator=(SubpassDependency &&other) noexcept;

        [[nodiscard]]
        vk::SubpassDependency createSubpassDependency() const;
    };

} // vklite
