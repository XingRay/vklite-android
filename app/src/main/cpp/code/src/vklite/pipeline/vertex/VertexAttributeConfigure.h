//
// Created by leixing on 2025/1/14.
//

#pragma once

#include <cstdint>

#include <vulkan/vulkan.hpp>


namespace vklite {

    class VertexAttributeConfigure {
    private:
        vk::Format mFormat;
        uint32_t mOffset;

    public:
        explicit VertexAttributeConfigure(vk::Format format, uint32_t offset);

        ~VertexAttributeConfigure();

        VertexAttributeConfigure(const VertexAttributeConfigure &other);

        VertexAttributeConfigure &operator=(const VertexAttributeConfigure &other);

        VertexAttributeConfigure(VertexAttributeConfigure &&other) noexcept;

        VertexAttributeConfigure &operator=(VertexAttributeConfigure &&other) noexcept;

        [[nodiscard]]
        vk::Format getFormat() const;

        [[nodiscard]]
        uint32_t getOffset() const;
    };


} // vklite
