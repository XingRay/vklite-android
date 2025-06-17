//
// Created by leixing on 2025/2/26.
//

#pragma once

#include <memory>
#include "opencv2/opencv.hpp"

namespace image_process {

    class LetterBox {
    public: // fields

        // 原始图片的宽高
        int originalWidth;
        int originalHeight;

        // 原始图片到目标图片(去除边框)的缩放比例
        float scale;

        // 目标图片的宽高(加补充的边框)
        // letterBox.targetWidth  == letterBox.paddedWidth  + letterBox.paddingLeft + letterBox.paddingRight;
        // letterBox.targetHeight == letterBox.paddedHeight + letterBox.paddingTop  + letterBox.paddingBottom;
        int targetWidth;
        int targetHeight;

        // 目标图片显示区域的宽高(除去补充的边框)
        int paddedWidth;
        int paddedHeight;

        // 补充的四周的边框的宽度
        int paddingLeft;
        int paddingTop;
        int paddingBottom;
        int paddingRight;

    public://methods

        cv::Point2f transformBack(float x, float y);

        // 将一组关键点坐标从 padded 图像转换回原始图像坐标
        cv::Point2f transformBack(const cv::Point2f &point);

        std::vector<cv::Point> transformBack(const std::vector<cv::Point2f> &points);

        cv::Rect transformBack(const cv::Rect &box);

        cv::Mat copyMakeBorder(const cv::Mat &src, const cv::Scalar &value = cv::Scalar(0, 0, 0));

    public:// static methods
        static LetterBox calcLetterbox(int width, int height, int targetWidth, int targetHeight);
    };


} // image
