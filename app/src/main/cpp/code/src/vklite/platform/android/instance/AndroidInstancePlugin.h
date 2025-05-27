//
// Created by leixing on 2025/3/15.
//

#pragma once

#include <vector>

#include <vulkan/vulkan.hpp>

#include "vklite/instance/Instance.h"
#include "vklite/instance/InstancePluginInterface.h"

namespace vklite {

    class AndroidInstancePlugin : public InstancePluginInterface {

    public:
        AndroidInstancePlugin();

        ~AndroidInstancePlugin() override;

        std::vector<const char *> getExtensions() override;

        std::vector<const char *> getLayers() override;

        void onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) override;

        void onInstanceCreated(Instance &instance) override;
    };

} // vklite
