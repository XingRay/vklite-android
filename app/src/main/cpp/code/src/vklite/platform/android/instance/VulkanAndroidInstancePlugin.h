//
// Created by leixing on 2025/3/15.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/instance/Instance.h"
#include "vklite/instance/InstancePlugin.h"

namespace vklite {

    class VulkanAndroidInstancePlugin : public InstancePlugin {

    public:
        VulkanAndroidInstancePlugin();

        ~VulkanAndroidInstancePlugin() override;

        void onInstanceCreated(const Instance &vulkanInstance) override;
    };

} // vklite
