//
// Created by leixing on 2025/1/11.
//

#include "PhysicalDeviceScoreCalculator.h"

namespace vklite {

    PhysicalDeviceScoreCalculator::PhysicalDeviceScoreCalculator(
            uint32_t geometryShaderScore,
            uint32_t samplerAnisotropyScore,
            uint32_t integratedGpuScore,
            uint32_t discreteGpuScore,
            uint32_t virtualGpuScore,
            uint32_t cpuScore,
            uint32_t otherDeviceTypeScore
    ) : mGeometryShaderScore(geometryShaderScore),
        mSamplerAnisotropyScore(samplerAnisotropyScore) {

        mPhysicalDeviceTypeScore.try_emplace(vk::PhysicalDeviceType::eIntegratedGpu, integratedGpuScore);
        mPhysicalDeviceTypeScore.try_emplace(vk::PhysicalDeviceType::eDiscreteGpu, discreteGpuScore);
        mPhysicalDeviceTypeScore.try_emplace(vk::PhysicalDeviceType::eVirtualGpu, virtualGpuScore);
        mPhysicalDeviceTypeScore.try_emplace(vk::PhysicalDeviceType::eCpu, cpuScore);
        mPhysicalDeviceTypeScore.try_emplace(vk::PhysicalDeviceType::eOther, otherDeviceTypeScore);
    }

    PhysicalDeviceScoreCalculator::PhysicalDeviceScoreCalculator()
            : PhysicalDeviceScoreCalculator(10, 10, 10, 20, 15, 5, 0) {}

    PhysicalDeviceScoreCalculator::~PhysicalDeviceScoreCalculator() = default;

    [[nodiscard]]
    uint32_t PhysicalDeviceScoreCalculator::calculateScore(const PhysicalDevice &physicalDevice) const {
        vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
        vk::PhysicalDeviceFeatures features = physicalDevice.getFeatures();

        uint32_t score = 0;

        if (features.geometryShader) {
            score += mGeometryShaderScore;
        }

        if (features.samplerAnisotropy) {
            score += mSamplerAnisotropyScore;
        }

        score += mPhysicalDeviceTypeScore.at(properties.deviceType);
        return score;
    }

} // vklite