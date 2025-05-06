//
// Created by leixing on 2025/3/15.
//

#pragma once

#include <vulkan/vulkan.hpp>

#include "vklite/instance/Instance.h"
#include "vklite/instance/InstancePlugin.h"

namespace vklite {

    class AndroidInstancePlugin : public InstancePlugin {

    public:
        AndroidInstancePlugin();

        ~AndroidInstancePlugin() override;

        void onInstanceCreated(const Instance &instance) override;
    };

} // vklite
