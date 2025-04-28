//
// Created by leixing on 2025/1/11.
//

#include "PhysicalDeviceScoreCalculator.h"

namespace vklite {

    PhysicalDeviceScoreCalculator::PhysicalDeviceScoreCalculator()
            : mGeometryShaderScore(10),
              mSamplerAnisotropyScore(10),
              mIntegratedGpuScore(10),
              mDiscreteGpuScore(20),
              mVirtualGpuScore(15),
              mCpuScore(5),
              mOtherDeviceTypeScore(0) {}

    PhysicalDeviceScoreCalculator::PhysicalDeviceScoreCalculator(
            uint32_t geometryShaderScore,
            uint32_t samplerAnisotropyScore,
            uint32_t integratedGpuScore,
            uint32_t discreteGpuScore,
            uint32_t virtualGpuScore,
            uint32_t cpuScore,
            uint32_t otherDeviceTypeScore
    )
            : mGeometryShaderScore(geometryShaderScore),
              mSamplerAnisotropyScore(samplerAnisotropyScore),
              mIntegratedGpuScore(integratedGpuScore),
              mDiscreteGpuScore(discreteGpuScore),
              mVirtualGpuScore(virtualGpuScore),
              mCpuScore(cpuScore),
              mOtherDeviceTypeScore(otherDeviceTypeScore) {}

    PhysicalDeviceScoreCalculator::~PhysicalDeviceScoreCalculator() = default;

    [[nodiscard]]
    uint32_t PhysicalDeviceScoreCalculator::calculateScore(const PhysicalDevice &physicalDevice) const {
        vk::PhysicalDevice vkPhysicalDevice = physicalDevice.getPhysicalDevice();
        vk::PhysicalDeviceProperties properties = vkPhysicalDevice.getProperties();
        vk::PhysicalDeviceFeatures features = vkPhysicalDevice.getFeatures();

        uint32_t score = 0;

        if (features.geometryShader) {
            score += mGeometryShaderScore;
        }

        if (features.samplerAnisotropy) {
            score += mSamplerAnisotropyScore;
        }

        switch (properties.deviceType) {
            case vk::PhysicalDeviceType::eIntegratedGpu:
                score += mIntegratedGpuScore;
                break;
            case vk::PhysicalDeviceType::eDiscreteGpu:
                score += mDiscreteGpuScore;
                break;
            case vk::PhysicalDeviceType::eVirtualGpu:
                score += mVirtualGpuScore;
                break;
            case vk::PhysicalDeviceType::eCpu:
                score += mCpuScore;
                break;
            case vk::PhysicalDeviceType::eOther:
            default:
                score += mOtherDeviceTypeScore;
                break;
        }

        return score;
    }

} // vklite