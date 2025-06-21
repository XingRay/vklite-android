//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class RenderPassBeginInfo {
    private:
        vk::RenderPassBeginInfo mBeginInfo;
        std::vector<vk::ClearValue> mClearValues;
        vk::SubpassContents mSubpassContents;

    public:
        explicit RenderPassBeginInfo(vk::RenderPassBeginInfo beginInfo, std::vector<vk::ClearValue> &&clearValues, vk::SubpassContents subpassContents);

        RenderPassBeginInfo();

        ~RenderPassBeginInfo();

        RenderPassBeginInfo(const RenderPassBeginInfo &other) = delete;

        RenderPassBeginInfo &operator=(const RenderPassBeginInfo &other) = delete;

        RenderPassBeginInfo(RenderPassBeginInfo &&other) noexcept;

        RenderPassBeginInfo &operator=(RenderPassBeginInfo &&other) noexcept;

        RenderPassBeginInfo &subpassContents(vk::SubpassContents subpassContents);

        RenderPassBeginInfo &clearValues(std::vector<vk::ClearValue> &&clearValues);

        RenderPassBeginInfo &updateClearValue(size_t index, vk::ClearValue clearValue);

        [[nodiscard]]
        vk::RenderPassBeginInfo &getBeginInfo();

        [[nodiscard]]
        vk::SubpassContents getSubpassContents();
    };

} // vklite
