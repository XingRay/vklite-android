//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <unordered_map>

#include <vulkan/vulkan.hpp>
#include "VertexBindingConfigure.h"

namespace vklite {

    class VertexConfigure {
    private:
        /**
         * vertex
         */
        // binding -> VertexBindingConfigure
        std::unordered_map<uint32_t, VertexBindingConfigure> mVertexBindingConfigures;

    public:
        explicit VertexConfigure();

        ~VertexConfigure();

        VertexConfigure(const VertexConfigure &other);

        VertexConfigure &operator=(const VertexConfigure &other);

        VertexConfigure(VertexConfigure &&other) noexcept;

        VertexConfigure &operator=(VertexConfigure &&other) noexcept;

        VertexConfigure &add(VertexBindingConfigure &&vertexConfigure);

        [[nodiscard]]
        std::vector<vk::VertexInputBindingDescription> createVertexInputBindingDescriptions() const;

        [[nodiscard]]
        std::vector<vk::VertexInputAttributeDescription> createVertexInputAttributeDescriptions() const;

    };

} // vklite
