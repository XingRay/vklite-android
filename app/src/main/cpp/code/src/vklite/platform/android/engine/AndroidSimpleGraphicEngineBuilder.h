//
// Created by leixing on 2025/6/4.
//

#pragma once

#include "vklite/engine/SimpleGraphicEngineBuilder.h"

namespace vklite {

    class AndroidSimpleGraphicEngineBuilder {
    private:// fields

    public:// methods

    public: // static
        /**
         * preset
         */
        static SimpleGraphicEngineBuilder asDefault(ANativeWindow *window);
    };

} // vklite
