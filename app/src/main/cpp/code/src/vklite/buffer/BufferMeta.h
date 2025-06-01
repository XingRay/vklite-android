//
// Created by leixing on 2025/5/31.
//

#pragma once

#include <vulkan/vulkan.hpp>

namespace vklite {

    class BufferMeta {
    private:
        vk::DeviceSize mSize;

    public:
        explicit BufferMeta(vk::DeviceSize size);

        ~BufferMeta();

        BufferMeta(const BufferMeta &other);

        BufferMeta &operator=(const BufferMeta &other);

        BufferMeta(BufferMeta &&other) noexcept;

        BufferMeta &operator=(BufferMeta &&other) noexcept;

        [[nodiscard]]
        vk::DeviceSize getSize() const;
    };

} // vklite
