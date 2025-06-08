//
// Created by leixing on 2025/6/4.
//

#pragma once

#include "vklite/engine/graphic_pipeline_engine/GraphicPipelineEngineBuilder.h"

namespace vklite {

    class AndroidGraphicPipelineEngineBuilder {
    private:// fields

    public:// methods

    public: // static
        /**
         * preset
         */
        static GraphicPipelineEngineBuilder asDefault(ANativeWindow *window);
    };

} // vklite
