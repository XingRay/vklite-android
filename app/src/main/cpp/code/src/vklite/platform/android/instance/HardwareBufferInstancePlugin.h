//
// Created by leixing on 2025/5/28.
//

#pragma once

#include "vklite/instance/InstancePluginInterface.h"

namespace vklite {

    class HardwareBufferInstancePlugin : public InstancePluginInterface {
    private:

    public:
        HardwareBufferInstancePlugin();

        ~HardwareBufferInstancePlugin() override;

        std::vector<const char *> getExtensions() override;

        std::vector<const char *> getLayers() override;

        void onPreCreateInstance(vk::InstanceCreateInfo &instanceCreateInfo) override;

        void onInstanceCreated(Instance &instance) override;
    };

} // vklite
