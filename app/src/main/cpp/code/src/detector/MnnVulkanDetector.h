//
// Created by leixing on 2025/2/20.
//

#pragma once

#include "MNN/Interpreter.hpp"
#include "android/hardware_buffer.h"

namespace detector {

    class MnnVulkanDetector {
    private:

        std::shared_ptr<MNN::Interpreter> mnn_net;
        MNN::Session *session;

        MNN::Tensor *input_tensor;
        int input_width;
        int input_height;
        int input_channels;

        MNN::Tensor *input_host;
        float *input_data;

        MNN::Tensor *output_classificators;
        MNN::Tensor *output_regressors;

        MNN::Tensor *output_classificators_host;
        MNN::Tensor *output_regressors_host;

        float *class_data;
        float *reg_data;

    public:
        MnnVulkanDetector();

        ~MnnVulkanDetector();

        std::optional<std::pair<std::vector<float>, std::vector<float> > > detect(AHardwareBuffer *hardwareBuffer);
    };

} // detector
