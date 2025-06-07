//
// Created by leixing on 2025/6/7.
//

#pragma once

#include "model/Model.h"

namespace model {

    class ModelLoader {
    private:

    public:

        [[nodiscard]]
        static Model load(const char *path);
    };

} // util
