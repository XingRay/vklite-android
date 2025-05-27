//
// Created by leixing on 2025/5/15.
//

#pragma once

#include <vector>
#include <cstddef>

#include <vulkan/vulkan.hpp>

#include "vklite/device/Device.h"
#include "vklite/command_buffer/CommandBuffer.h"

namespace vklite {

    class CommandPool;

    class CommandBuffers {
    private:
        const Device &mDevice;
        const CommandPool &mCommandPool;

        std::vector<CommandBuffer> mCommandBuffers;

    public:
        CommandBuffers(const Device &device,
                       const CommandPool &commandPool,
                       std::vector<CommandBuffer> &&commandBuffers);

        ~CommandBuffers();

        // 拷贝构造函数（显式删除） ---
        // 原因：mDevice 是 const 引用，无法重新绑定；
        CommandBuffers(const CommandBuffers &other) = delete;

        // 移动构造函数
        CommandBuffers(CommandBuffers &&other) noexcept;

        // 拷贝赋值运算符
        CommandBuffers &operator=(const CommandBuffers &) = delete;

        // 移动赋值运算符
        CommandBuffers &operator=(CommandBuffers &&) = delete;

        const CommandBuffer &operator[](size_t index);

    };

} // vklite
