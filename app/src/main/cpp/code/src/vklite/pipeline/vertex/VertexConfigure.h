//
// Created by leixing on 2025/3/11.
//

#pragma once

#include <memory>
#include <unordered_map>

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
        VertexConfigure();

        ~VertexConfigure();

        VertexConfigure &add(VertexBindingConfigure &&vertexConfigure);

        [[nodiscard]]
        std::vector<vk::VertexInputBindingDescription> createVertexInputBindingDescriptions() const;

        [[nodiscard]]
        std::vector<vk::VertexInputAttributeDescription> createVertexInputAttributeDescriptions() const;

    };

} // vklite
