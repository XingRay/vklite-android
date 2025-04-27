//
// Created by leixing on 2025/3/15.
//

#pragma once

#include "Instance.h"

namespace vklite {

    class InstancePlugin {

    public:
        InstancePlugin();

        virtual ~InstancePlugin();

        virtual void onInstanceCreated(const Instance &instance) = 0;
    };

} // vklite
