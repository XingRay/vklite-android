//
// Created by leixing on 2025/1/14.
//

#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include <vulkan//vulkan.hpp>

#include "VertexAttributeConfigure.h"
#include "vklite/ShaderFormat.h"

namespace vklite {

    class VertexBindingConfigure {
    private:
        uint32_t mBinding;
        uint32_t mStride;
        vk::VertexInputRate mVertexInputRate;

        // location -> VertexAttributeConfigure
        std::unordered_map<uint32_t, VertexAttributeConfigure> mAttributes;

        uint32_t mCurrentAttributeLocation = -1;
        uint32_t mCurrentAttributeOffset = 0;

    public:
        VertexBindingConfigure();

        ~VertexBindingConfigure();

        VertexBindingConfigure(const VertexBindingConfigure &other);

        VertexBindingConfigure &operator=(const VertexBindingConfigure &other);

        VertexBindingConfigure(VertexBindingConfigure &&other) noexcept;

        VertexBindingConfigure &operator=(VertexBindingConfigure &&other) noexcept;

        [[nodiscard]]
        uint32_t getBinding() const;

        [[nodiscard]]
        uint32_t getStride() const;

        [[nodiscard]]
        vk::VertexInputRate getVertexInputRate() const;

        [[nodiscard]]
        const std::unordered_map<uint32_t, VertexAttributeConfigure> &getAttributes() const;

        VertexBindingConfigure &binding(uint32_t binding);

        VertexBindingConfigure &stride(uint32_t stride);

        VertexBindingConfigure &inputRate(vk::VertexInputRate inputRate);

        VertexBindingConfigure &addAttribute(uint32_t location, uint32_t binding, ShaderFormat format, uint32_t offset);

        VertexBindingConfigure &addAttribute(uint32_t location, uint32_t binding, vk::Format format, uint32_t offset);

        VertexBindingConfigure &addAttribute(uint32_t location, ShaderFormat format, uint32_t offset);

        VertexBindingConfigure &addAttribute(uint32_t location, vk::Format format, uint32_t offset);

        VertexBindingConfigure &addAttribute(uint32_t location, ShaderFormat format);

        VertexBindingConfigure &addAttribute(uint32_t location, vk::Format format);

        VertexBindingConfigure &addAttribute(ShaderFormat format);

        VertexBindingConfigure &addAttribute(vk::Format format);
    };

} // vklite
