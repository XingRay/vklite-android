//
// Created by leixing on 2025/3/15.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "Instance.h"

namespace vklite {

    class InstancePluginInterface {

    public:
        InstancePluginInterface();

        virtual ~InstancePluginInterface();


        virtual std::vector<const char *> getExtensions() = 0;

        virtual std::vector<const char *> getLayers() = 0;

        virtual void onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) = 0;

        virtual void onInstanceCreated(Instance &instance) = 0;
    };

} // vklite
