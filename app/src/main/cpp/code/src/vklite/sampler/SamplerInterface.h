//
// Created by leixing on 2025/3/30.
//

#pragma once

#include "vulkan/vulkan.hpp"

namespace vklite {

    class SamplerInterface {
    private:

    public:
        SamplerInterface();

        virtual ~SamplerInterface() = 0;

        // 声明移动构造函数和移动赋值运算符
        SamplerInterface(SamplerInterface &&) noexcept = default;

        SamplerInterface &operator=(SamplerInterface &&) noexcept = default;

        [[nodiscard]]
        virtual const vk::Sampler &getSampler() const = 0;
    };

} // vklite
