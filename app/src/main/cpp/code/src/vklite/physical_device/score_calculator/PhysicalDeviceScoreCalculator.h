//
// Created by leixing on 2025/1/11.
//

#pragma once

#include <cstdint>

#include "vulkan/vulkan.hpp"
#include "vklite/physical_device/PhysicalDevice.h"

namespace vklite {

    class PhysicalDeviceScoreCalculator {
    private:
        // feature score
        uint32_t mGeometryShaderScore;      // 几何着色器加分
        uint32_t mSamplerAnisotropyScore;   // 各向异性过滤加分

        // device type score
        uint32_t mIntegratedGpuScore;       // 核显加分
        uint32_t mDiscreteGpuScore;         // 独显加分
        uint32_t mVirtualGpuScore;          // 虚拟GPU加分
        uint32_t mCpuScore;                 // CPU加分
        uint32_t mOtherDeviceTypeScore;     // 其他类型设备加分

    public:
        PhysicalDeviceScoreCalculator();

        PhysicalDeviceScoreCalculator(
                uint32_t geometryShaderScore,
                uint32_t samplerAnisotropyScore,
                uint32_t integratedGpuScore,
                uint32_t discreteGpuScore,
                uint32_t virtualGpuScore,
                uint32_t cpuScore,
                uint32_t otherDeviceTypeScore
        );

        ~PhysicalDeviceScoreCalculator();

        [[nodiscard]]
        uint32_t calculateScore(const PhysicalDevice &physicalDevice) const;
    };

} // vklite
