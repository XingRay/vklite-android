//
// Created by leixing on 2025/4/28.
//

#include "DeviceBuilder.h"

#include <unordered_set>

#include "vklite/Log.h"
#include "vklite/util/CStringUtil.h"
#include "vklite/util/VulkanUtil.h"

namespace vklite {

    DeviceBuilder::DeviceBuilder()
            : mCheckPhysicalDeviceFeatures(false) {
        mDeviceCreateInfo = vk::DeviceCreateInfo{};
        mDeviceCreateInfo
                .setFlags(vk::DeviceCreateFlags{})
                .setQueueCreateInfos(mDeviceQueueCreateInfos)
                .setPEnabledFeatures(&mRequiredPhysicalDeviceFeatures)
                .setPEnabledExtensionNames(mExtensions)
                .setPEnabledLayerNames(mLayers);
    }

    DeviceBuilder::~DeviceBuilder() = default;

    DeviceBuilder::DeviceBuilder(DeviceBuilder &&other) noexcept
            : mPhysicalDevice(std::move(other.mPhysicalDevice)),
              mDeviceCreateInfo(std::move(other.mDeviceCreateInfo)),
              mDeviceQueueCreateInfos(std::move(other.mDeviceQueueCreateInfos)),
              mQueuePriorities(std::move(other.mQueuePriorities)),
              mCheckPhysicalDeviceFeatures(std::exchange(other.mCheckPhysicalDeviceFeatures, false)),
              mRequiredPhysicalDeviceFeatures(std::move(other.mRequiredPhysicalDeviceFeatures)),
              mExtensions(std::move(other.mExtensions)),
              mLayers(std::move(other.mLayers)),
              mPlugins(std::move(other.mPlugins)) {
        other.mDeviceCreateInfo = vk::DeviceCreateInfo{}; // 重置为默认状态
        other.mRequiredPhysicalDeviceFeatures = vk::PhysicalDeviceFeatures{};
    }

    DeviceBuilder &DeviceBuilder::operator=(DeviceBuilder &&other) noexcept {
        if (this != &other) {
            mPhysicalDevice = std::move(other.mPhysicalDevice);
            mDeviceCreateInfo = std::move(other.mDeviceCreateInfo);
            mDeviceQueueCreateInfos = std::move(other.mDeviceQueueCreateInfos);
            mQueuePriorities = std::move(other.mQueuePriorities);
            mCheckPhysicalDeviceFeatures = std::exchange(other.mCheckPhysicalDeviceFeatures, false);
            mRequiredPhysicalDeviceFeatures = std::move(other.mRequiredPhysicalDeviceFeatures);
            mExtensions = std::move(other.mExtensions);
            mLayers = std::move(other.mLayers);
            mPlugins = std::move(other.mPlugins);

            // 清理源对象状态
            other.mDeviceCreateInfo = vk::DeviceCreateInfo{};
            other.mRequiredPhysicalDeviceFeatures = vk::PhysicalDeviceFeatures{};
        }
        return *this;
    }


    DeviceBuilder &DeviceBuilder::physicalDevice(vk::PhysicalDevice physicalDevice) {
        mPhysicalDevice = physicalDevice;
        return *this;
    }

    DeviceBuilder &DeviceBuilder::flags(vk::DeviceCreateFlags flags) {
        mDeviceCreateInfo.setFlags(flags);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::addPlugin(std::unique_ptr<PluginInterface> plugin) {
        mPlugins.push_back(std::move(plugin));
        return *this;
    }

    // addQueueFamily
    DeviceBuilder &DeviceBuilder::addQueueFamily(uint32_t queueFamilyIndex, std::vector<float> &&priorities) {
        auto it = std::find_if(mDeviceQueueCreateInfos.begin(), mDeviceQueueCreateInfos.end(), [&](vk::DeviceQueueCreateInfo &info) {
            return info.queueFamilyIndex == queueFamilyIndex;
        });

        if (it == mDeviceQueueCreateInfos.end()) {
            // not found, then insert
            mQueuePriorities.push_back(std::move(priorities));
            vk::DeviceQueueCreateInfo deviceQueueCreateInfo = {};
            deviceQueueCreateInfo
                    .setQueueFamilyIndex(queueFamilyIndex)
                    .setQueuePriorities(mQueuePriorities.back());
            mDeviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
        } else {
            // found, then update
            // 计算索引
            LOG_WF("DeviceBuilder::addQueueFamily, queueFamilyIndex:{} exists, overwrite it", queueFamilyIndex);
            size_t index = std::distance(mDeviceQueueCreateInfos.begin(), it);
            mQueuePriorities[index] = std::move(priorities);

            vk::DeviceQueueCreateInfo deviceQueueCreateInfo = {};
            deviceQueueCreateInfo
                    .setQueueFamilyIndex(queueFamilyIndex)
                    .setQueuePriorities(mQueuePriorities[index]);

            mDeviceQueueCreateInfos[index] = deviceQueueCreateInfo;
        }

        mDeviceCreateInfo.setQueueCreateInfos(mDeviceQueueCreateInfos);

        return *this;
    }

    DeviceBuilder &DeviceBuilder::addQueueFamily(uint32_t queueFamilyIndex) {
        addQueueFamily(queueFamilyIndex, std::vector<float>{1.0f});
        return *this;
    }

    DeviceBuilder &DeviceBuilder::extensions(std::vector<const char *> &&extensions) {
        mExtensions = std::move(extensions);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::layers(std::vector<const char *> &&layers) {
        mLayers = std::move(layers);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::physicalDeviceFeaturesConfigure(const std::function<void(vk::PhysicalDeviceFeatures &physicalDeviceFeatures)> &configure) {
        configure(mRequiredPhysicalDeviceFeatures);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::enableSamplerAnisotropy() {
        mRequiredPhysicalDeviceFeatures.setSamplerAnisotropy(vk::True);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::enableSampleRateShading() {
        mRequiredPhysicalDeviceFeatures.setSampleRateShading(vk::True);
        return *this;
    }

    DeviceBuilder &DeviceBuilder::checkPhysicalDeviceFeatures(bool enable) {
        mCheckPhysicalDeviceFeatures = enable;
        return *this;
    }

    std::optional<Device> DeviceBuilder::build() {

        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = mPhysicalDevice.getQueueFamilyProperties();
        uint32_t queueFamilyPropertiesSize = queueFamilyProperties.size();
        for (const auto &queueCreateInfo: mDeviceQueueCreateInfos) {
            uint32_t familyIndex = queueCreateInfo.queueFamilyIndex;
            if (familyIndex >= queueFamilyPropertiesSize) {
                LOG_EF("Invalid queue family index:{}", familyIndex);
                throw std::runtime_error("Invalid queue family index: " + std::to_string(familyIndex));
            }
            // 验证队列家族支持的最小队列数
            const uint32_t maxQueues = queueFamilyProperties[familyIndex].queueCount;
            if (maxQueues < 1) {
                throw std::runtime_error("Queue family " + std::to_string(familyIndex) + " does not support any queues.");
            }
        }


        // features
        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            plugin->physicalDeviceFeaturesConfigure(mRequiredPhysicalDeviceFeatures);
        }

        // checking
        if (mCheckPhysicalDeviceFeatures) {
            checkPhysicalDeviceFeatures();
        }

        std::vector<vk::ExtensionProperties> extensionProperties = mPhysicalDevice.enumerateDeviceExtensionProperties();
        LOG_D("PhysicalDevice Supported Extensions:[%ld]", extensionProperties.size());
        VulkanUtil::printExtensions(extensionProperties);

        for (const std::unique_ptr<PluginInterface> &plugin: mPlugins) {
            std::vector<const char *> deviceExtensions = plugin->getDeviceExtensions();
            mExtensions.insert(mExtensions.end(), std::move_iterator(deviceExtensions.begin()), std::move_iterator(deviceExtensions.end()));

            std::vector<const char *> layers = plugin->getDeviceLayers();
            mLayers.insert(mLayers.end(), std::move_iterator(layers.begin()), std::move_iterator(layers.end()));
        }

        mExtensions = CStringUtil::removeDuplicates(mExtensions);
        LOG_D("DeviceBuilder::build: extensions:");
        for (const char *name: mExtensions) {
            LOG_D("\t%s", name);
        }
        mDeviceCreateInfo.setPEnabledExtensionNames(mExtensions);

        mLayers = CStringUtil::removeDuplicates(mLayers);
        LOG_D("DeviceBuilder::build: layers:");
        for (const char *name: mLayers) {
            LOG_D("\t%s", name);
        }
        mDeviceCreateInfo.setPEnabledLayerNames(mLayers);

//        vk::DeviceCreateInfo deviceCreateInfo;
//        deviceCreateInfo
//                .setFlags(mFlags)
//                .setQueueCreateInfos(queueCreateInfos)
//                .setPEnabledFeatures(&mRequiredPhysicalDeviceFeatures)
//                .setPEnabledExtensionNames(mExtensions)
//                .setPEnabledLayerNames(mLayers);

        for (const std::unique_ptr<PluginInterface> &devicePlugin: mPlugins) {
            devicePlugin->onPreCreateDevice(mDeviceCreateInfo);
        }

        vk::Device device = mPhysicalDevice.createDevice(mDeviceCreateInfo);
        return Device(device);
    }

    std::unique_ptr<Device> DeviceBuilder::buildUnique() {
        std::optional<Device> device = build();
        if (!device.has_value()) {
            return nullptr;
        }
        return std::make_unique<Device>(std::move(device.value()));
    }

    void DeviceBuilder::checkPhysicalDeviceFeatures() {
        vk::PhysicalDeviceFeatures supportedPhysicalDeviceFeatures = mPhysicalDevice.getFeatures();

        // 核心图形特性检查
        if (mRequiredPhysicalDeviceFeatures.robustBufferAccess && !supportedPhysicalDeviceFeatures.robustBufferAccess) {
            throw std::runtime_error("robustBufferAccess not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.fullDrawIndexUint32 && !supportedPhysicalDeviceFeatures.fullDrawIndexUint32) {
            throw std::runtime_error("fullDrawIndexUint32 not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.imageCubeArray && !supportedPhysicalDeviceFeatures.imageCubeArray) {
            throw std::runtime_error("imageCubeArray not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.independentBlend && !supportedPhysicalDeviceFeatures.independentBlend) {
            throw std::runtime_error("independentBlend not supported");
        }

        // 着色器阶段支持检查
        if (mRequiredPhysicalDeviceFeatures.geometryShader && !supportedPhysicalDeviceFeatures.geometryShader) {
            throw std::runtime_error("geometryShader not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.tessellationShader && !supportedPhysicalDeviceFeatures.tessellationShader) {
            throw std::runtime_error("tessellationShader not supported");
        }

        // 采样与混合特性检查
        if (mRequiredPhysicalDeviceFeatures.sampleRateShading && !supportedPhysicalDeviceFeatures.sampleRateShading) {
            throw std::runtime_error("sampleRateShading not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.dualSrcBlend && !supportedPhysicalDeviceFeatures.dualSrcBlend) {
            throw std::runtime_error("dualSrcBlend not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.logicOp && !supportedPhysicalDeviceFeatures.logicOp) {
            throw std::runtime_error("logicOp not supported");
        }

        // 绘制命令特性检查
        if (mRequiredPhysicalDeviceFeatures.multiDrawIndirect && !supportedPhysicalDeviceFeatures.multiDrawIndirect) {
            throw std::runtime_error("multiDrawIndirect not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.drawIndirectFirstInstance && !supportedPhysicalDeviceFeatures.drawIndirectFirstInstance) {
            throw std::runtime_error("drawIndirectFirstInstance not supported");
        }

        // 深度与模板特性检查
        if (mRequiredPhysicalDeviceFeatures.depthClamp && !supportedPhysicalDeviceFeatures.depthClamp) {
            throw std::runtime_error("depthClamp not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.depthBiasClamp && !supportedPhysicalDeviceFeatures.depthBiasClamp) {
            throw std::runtime_error("depthBiasClamp not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.fillModeNonSolid && !supportedPhysicalDeviceFeatures.fillModeNonSolid) {
            throw std::runtime_error("fillModeNonSolid not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.depthBounds && !supportedPhysicalDeviceFeatures.depthBounds) {
            throw std::runtime_error("depthBounds not supported");
        }

        // 图元特性检查
        if (mRequiredPhysicalDeviceFeatures.wideLines && !supportedPhysicalDeviceFeatures.wideLines) {
            throw std::runtime_error("wideLines not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.largePoints && !supportedPhysicalDeviceFeatures.largePoints) {
            throw std::runtime_error("largePoints not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.alphaToOne && !supportedPhysicalDeviceFeatures.alphaToOne) {
            throw std::runtime_error("alphaToOne not supported");
        }

        // 视口与裁剪特性检查
        if (mRequiredPhysicalDeviceFeatures.multiViewport && !supportedPhysicalDeviceFeatures.multiViewport) {
            throw std::runtime_error("multiViewport not supported");
        }

        // 采样器特性检查
        if (mRequiredPhysicalDeviceFeatures.samplerAnisotropy && !supportedPhysicalDeviceFeatures.samplerAnisotropy) {
            throw std::runtime_error("samplerAnisotropy not supported");
        }

        // 纹理压缩格式检查
        if (mRequiredPhysicalDeviceFeatures.textureCompressionETC2 && !supportedPhysicalDeviceFeatures.textureCompressionETC2) {
            throw std::runtime_error("textureCompressionETC2 not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.textureCompressionASTC_LDR && !supportedPhysicalDeviceFeatures.textureCompressionASTC_LDR) {
            throw std::runtime_error("textureCompressionASTC_LDR not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.textureCompressionBC && !supportedPhysicalDeviceFeatures.textureCompressionBC) {
            throw std::runtime_error("textureCompressionBC not supported");
        }

        // 查询特性检查
        if (mRequiredPhysicalDeviceFeatures.occlusionQueryPrecise && !supportedPhysicalDeviceFeatures.occlusionQueryPrecise) {
            throw std::runtime_error("occlusionQueryPrecise not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.pipelineStatisticsQuery && !supportedPhysicalDeviceFeatures.pipelineStatisticsQuery) {
            throw std::runtime_error("pipelineStatisticsQuery not supported");
        }

        // 原子操作特性检查
        if (mRequiredPhysicalDeviceFeatures.vertexPipelineStoresAndAtomics && !supportedPhysicalDeviceFeatures.vertexPipelineStoresAndAtomics) {
            throw std::runtime_error("vertexPipelineStoresAndAtomics not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.fragmentStoresAndAtomics && !supportedPhysicalDeviceFeatures.fragmentStoresAndAtomics) {
            throw std::runtime_error("fragmentStoresAndAtomics not supported");
        }

        // 着色器特性检查
        if (mRequiredPhysicalDeviceFeatures.shaderTessellationAndGeometryPointSize && !supportedPhysicalDeviceFeatures.shaderTessellationAndGeometryPointSize) {
            throw std::runtime_error("shaderTessellationAndGeometryPointSize not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderImageGatherExtended && !supportedPhysicalDeviceFeatures.shaderImageGatherExtended) {
            throw std::runtime_error("shaderImageGatherExtended not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderStorageImageExtendedFormats && !supportedPhysicalDeviceFeatures.shaderStorageImageExtendedFormats) {
            throw std::runtime_error("shaderStorageImageExtendedFormats not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderStorageImageMultisample && !supportedPhysicalDeviceFeatures.shaderStorageImageMultisample) {
            throw std::runtime_error("shaderStorageImageMultisample not supported");
        }

        // 存储图像格式检查
        if (mRequiredPhysicalDeviceFeatures.shaderStorageImageReadWithoutFormat && !supportedPhysicalDeviceFeatures.shaderStorageImageReadWithoutFormat) {
            throw std::runtime_error("shaderStorageImageReadWithoutFormat not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderStorageImageWriteWithoutFormat && !supportedPhysicalDeviceFeatures.shaderStorageImageWriteWithoutFormat) {
            throw std::runtime_error("shaderStorageImageWriteWithoutFormat not supported");
        }

        // 动态索引特性检查
        if (mRequiredPhysicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing && !supportedPhysicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing) {
            throw std::runtime_error("shaderUniformBufferArrayDynamicIndexing not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing && !supportedPhysicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing) {
            throw std::runtime_error("shaderSampledImageArrayDynamicIndexing not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing && !supportedPhysicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing) {
            throw std::runtime_error("shaderStorageBufferArrayDynamicIndexing not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing && !supportedPhysicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing) {
            throw std::runtime_error("shaderStorageImageArrayDynamicIndexing not supported");
        }

        // 裁剪与距离特性检查
        if (mRequiredPhysicalDeviceFeatures.shaderClipDistance && !supportedPhysicalDeviceFeatures.shaderClipDistance) {
            throw std::runtime_error("shaderClipDistance not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderCullDistance && !supportedPhysicalDeviceFeatures.shaderCullDistance) {
            throw std::runtime_error("shaderCullDistance not supported");
        }

        // 数值精度特性检查
        if (mRequiredPhysicalDeviceFeatures.shaderFloat64 && !supportedPhysicalDeviceFeatures.shaderFloat64) {
            throw std::runtime_error("shaderFloat64 not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderInt64 && !supportedPhysicalDeviceFeatures.shaderInt64) {
            throw std::runtime_error("shaderInt64 not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderInt16 && !supportedPhysicalDeviceFeatures.shaderInt16) {
            throw std::runtime_error("shaderInt16 not supported");
        }

        // 资源管理特性检查
        if (mRequiredPhysicalDeviceFeatures.shaderResourceResidency && !supportedPhysicalDeviceFeatures.shaderResourceResidency) {
            throw std::runtime_error("shaderResourceResidency not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.shaderResourceMinLod && !supportedPhysicalDeviceFeatures.shaderResourceMinLod) {
            throw std::runtime_error("shaderResourceMinLod not supported");
        }

        // 稀疏内存特性检查
        if (mRequiredPhysicalDeviceFeatures.sparseBinding && !supportedPhysicalDeviceFeatures.sparseBinding) {
            throw std::runtime_error("sparseBinding not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.sparseResidencyBuffer && !supportedPhysicalDeviceFeatures.sparseResidencyBuffer) {
            throw std::runtime_error("sparseResidencyBuffer not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.sparseResidencyImage2D && !supportedPhysicalDeviceFeatures.sparseResidencyImage2D) {
            throw std::runtime_error("sparseResidencyImage2D not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.sparseResidencyImage3D && !supportedPhysicalDeviceFeatures.sparseResidencyImage3D) {
            throw std::runtime_error("sparseResidencyImage3D not supported");
        }

        // 稀疏采样特性检查
        if (mRequiredPhysicalDeviceFeatures.sparseResidency2Samples && !supportedPhysicalDeviceFeatures.sparseResidency2Samples) {
            throw std::runtime_error("sparseResidency2Samples not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.sparseResidency4Samples && !supportedPhysicalDeviceFeatures.sparseResidency4Samples) {
            throw std::runtime_error("sparseResidency4Samples not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.sparseResidency8Samples && !supportedPhysicalDeviceFeatures.sparseResidency8Samples) {
            throw std::runtime_error("sparseResidency8Samples not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.sparseResidency16Samples && !supportedPhysicalDeviceFeatures.sparseResidency16Samples) {
            throw std::runtime_error("sparseResidency16Samples not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.sparseResidencyAliased && !supportedPhysicalDeviceFeatures.sparseResidencyAliased) {
            throw std::runtime_error("sparseResidencyAliased not supported");
        }

        // 其他特性检查
        if (mRequiredPhysicalDeviceFeatures.variableMultisampleRate && !supportedPhysicalDeviceFeatures.variableMultisampleRate) {
            throw std::runtime_error("variableMultisampleRate not supported");
        }

        if (mRequiredPhysicalDeviceFeatures.inheritedQueries && !supportedPhysicalDeviceFeatures.inheritedQueries) {
            throw std::runtime_error("inheritedQueries not supported");
        }
    }

} // vklite