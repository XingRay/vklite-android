//
// Created by leixing on 2025/4/25.
//

#pragma once

#include <vector>
#include <memory>

#include <vulkan/vulkan.hpp>

namespace vklite {

    class WriteDescriptorSet {
    private:
        vk::WriteDescriptorSet mWriteDescriptorSet;

    public:
        explicit WriteDescriptorSet();

        ~WriteDescriptorSet();

        const vk::WriteDescriptorSet &getWriteDescriptorSet() const;
    };

} // vklite
