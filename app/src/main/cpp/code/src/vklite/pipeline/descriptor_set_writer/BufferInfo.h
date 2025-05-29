//
// Created by leixing on 2025/3/18.
//

#pragma once

#include <cstdint>
#include <memory>

#include <vulkan/vulkan.hpp>
#include "vklite/buffer/BufferInterface.h"
#include "vklite/command_pool/CommandPool.h"

namespace vklite {

    class BufferInfo {
    private:
        std::shared_ptr<BufferInterface> mBuffer;
        uint32_t mOffset;
        uint32_t mRange;

    public:
        BufferInfo(const std::shared_ptr<BufferInterface> &buffer, uint32_t offset, uint32_t range);

        BufferInfo(const std::shared_ptr<BufferInterface> &buffer, uint32_t range);

        explicit BufferInfo(const std::shared_ptr<BufferInterface> &buffer);

        ~BufferInfo();

        [[nodiscard]]
        const std::shared_ptr<BufferInterface> &getVulkanBuffer() const;

        [[nodiscard]]
        uint32_t getOffset() const;

        [[nodiscard]]
        uint32_t getRange() const;

        void update(const CommandPool &commandPool, const void *data, uint32_t size);

        [[nodiscard]]
        vk::DescriptorBufferInfo createDescriptorBufferInfo() const;
    };

} // vklite
