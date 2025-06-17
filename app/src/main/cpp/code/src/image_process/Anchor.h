//
// Created by leixing on 2025/2/26.
//

#pragma once

#include <vector>

namespace image_process {

    // 矩形框
    class Anchor {
    public:
        float centerX;
        float centerY;

        float width;
        float height;

        static std::vector<Anchor> generateAnchors(int input_size = 128);
    };


} // image
