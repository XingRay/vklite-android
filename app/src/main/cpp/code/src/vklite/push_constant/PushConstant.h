//
// Created by leixing on 2025/3/10.
//

#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class PushConstant {
    private:
        std::vector<uint8_t> mData;
        uint32_t mOffset;
        vk::ShaderStageFlags mStageFlags;

    public:
        explicit PushConstant(uint32_t size, uint32_t offset, vk::ShaderStageFlags stageFlags);

        explicit PushConstant(const void *data, uint32_t size, uint32_t offset, vk::ShaderStageFlags stageFlags);

        explicit PushConstant(const void *data, uint32_t size, vk::ShaderStageFlags stageFlags);

        explicit PushConstant(const void *data, uint32_t size);

        explicit PushConstant(std::vector<uint8_t> &&data, uint32_t offset, vk::ShaderStageFlags stageFlags);

        explicit PushConstant(std::vector<uint8_t> &&data, vk::ShaderStageFlags stageFlags);

        explicit PushConstant(std::vector<uint8_t> &&data);

        ~PushConstant();

        PushConstant(const PushConstant &other);

        PushConstant &operator=(const PushConstant &other);

        PushConstant(PushConstant &&other) noexcept;

        PushConstant &operator=(PushConstant &&other) noexcept;

        void update(const void *data, uint32_t size);

        [[nodiscard]]
        vk::ShaderStageFlags getStageFlags() const;

        [[nodiscard]]
        uint32_t getOffset() const;

        [[nodiscard]]
        uint32_t getSize() const;

        [[nodiscard]]
        const void *getData() const;
    };

} // vklite
