//
// Created by leixing on 2025/2/26.
//

#include "image_process/LetterBox.h"
#include <algorithm>

namespace image_process {

    cv::Point2f LetterBox::transformBack(float x, float y) {
        float x_unpad = x - static_cast<float>(paddingLeft);
        float y_unpad = y - static_cast<float>(paddingTop);
        float x_orig = x_unpad / scale;
        float y_orig = y_unpad / scale;
        x_orig = std::clamp(x_orig, 0.0f, static_cast<float>(originalWidth));
        y_orig = std::clamp(y_orig, 0.0f, static_cast<float>(originalHeight));
        return cv::Point2f{x_orig, y_orig};
    }

    cv::Point2f LetterBox::transformBack(const cv::Point2f &point) {
        return transformBack(point.x, point.y);
    }

    std::vector<cv::Point> LetterBox::transformBack(const std::vector<cv::Point2f> &points) {
        std::vector<cv::Point> srcPoints;
        srcPoints.reserve(points.size());

        for (const auto &kp: points) {
            cv::Point2f p = transformBack(kp);
            srcPoints.emplace_back(std::round(p.x), std::round(p.y));
        }

        return srcPoints;
    }

    cv::Rect LetterBox::transformBack(const cv::Rect &box) {

        cv::Point2f position = transformBack(box.x, box.y);
        cv::Point2f rightBottom = transformBack(box.x + box.width, box.y + box.height);

        int new_x = std::round(position.x);
        int new_y = std::round(position.y);
        int new_w = std::round(rightBottom.x - position.x);
        int new_h = std::round(rightBottom.y - position.y);
//        int new_w = box.width/scale;
//        int new_h = box.height/scale;

        return cv::Rect(new_x, new_y, new_w, new_h);
    };

    cv::Mat LetterBox::copyMakeBorder(const cv::Mat &src, const cv::Scalar &value) {
        cv::Mat resized;
        cv::resize(src, resized, cv::Size(paddedWidth, paddedHeight));

        cv::Mat result;
        cv::copyMakeBorder(resized, result, paddingTop, paddingBottom, paddingLeft, paddingRight, cv::BORDER_CONSTANT, value);
        return result;
    }

    LetterBox LetterBox::calcLetterbox(int width, int height, int targetWidth, int targetHeight) {
        LetterBox letterBox{};
        letterBox.originalWidth = width;
        letterBox.originalHeight = height;
        letterBox.targetWidth = targetWidth;
        letterBox.targetHeight = targetHeight;

        letterBox.scale = std::min((float) targetWidth / width, (float) targetHeight / height);
        letterBox.paddedWidth = std::round(width * letterBox.scale);
        letterBox.paddedHeight = std::round(height * letterBox.scale);

        letterBox.paddingLeft = (letterBox.targetWidth - letterBox.paddedWidth) / 2;
        letterBox.paddingTop = (letterBox.targetHeight - letterBox.paddedHeight) / 2;
        letterBox.paddingRight = letterBox.targetWidth - letterBox.paddedWidth - letterBox.paddingLeft;
        letterBox.paddingBottom = letterBox.targetHeight - letterBox.paddedHeight - letterBox.paddingTop;

        return letterBox;
    }

} // image