//
// Created by leixing on 2025-06-18.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class SubpassDependencyItem {
    private:
        uint32_t mSubpassIndex;
        vk::PipelineStageFlags mStage;
        vk::AccessFlags mAccess;

    public:
        explicit SubpassDependencyItem(uint32_t subpassIndex,
                                       vk::PipelineStageFlags stage,
                                       vk::AccessFlags access);

        ~SubpassDependencyItem();

        SubpassDependencyItem(const SubpassDependencyItem &other);

        SubpassDependencyItem &operator=(const SubpassDependencyItem &other);

        SubpassDependencyItem(SubpassDependencyItem &&other) noexcept;

        SubpassDependencyItem &operator=(SubpassDependencyItem &&other) noexcept;

        [[nodiscard]]
        uint32_t getSubpassIndex() const;

        [[nodiscard]]
        vk::PipelineStageFlags getStage() const;

        [[nodiscard]]
        vk::AccessFlags getAccess() const;

    };

} // vklite
