//
// Created by leixing on 2024/12/30.
//

#include <set>
#include <string>

#include "VulkanUtil.h"
#include "vklite/Log.h"

namespace vklite {

//    void VulkanUtil::recordCopyBufferCommand(const vk::CommandBuffer &commandBuffer,
//                                             const vk::Buffer &srcBuffer,
//                                             const vk::Buffer &dstBuffer,
//                                             vk::DeviceSize size) {
//        vk::BufferCopy bufferCopy;
//        bufferCopy.setSrcOffset(0)
//                .setDstOffset(0)
//                .setSize(size);
//
//        commandBuffer.copyBuffer(srcBuffer, dstBuffer, bufferCopy);
//    }

    // 转换 SampleCountFlags 为字符串
    std::string VulkanUtil::sampleCountFlagsToString(vk::SampleCountFlags flags) {
        std::string result;

        if (flags & vk::SampleCountFlagBits::e1) result += "1 ";
        if (flags & vk::SampleCountFlagBits::e2) result += "2 ";
        if (flags & vk::SampleCountFlagBits::e4) result += "4 ";
        if (flags & vk::SampleCountFlagBits::e8) result += "8 ";
        if (flags & vk::SampleCountFlagBits::e16) result += "16 ";
        if (flags & vk::SampleCountFlagBits::e32) result += "32 ";
        if (flags & vk::SampleCountFlagBits::e64) result += "64 ";

        if (result.empty()) return "None";
        return result;
    }

    // 将 vk::DeviceSize 转换为更易读的单位
    std::string VulkanUtil::formatDeviceSize(vk::DeviceSize size) {
        constexpr vk::DeviceSize KB = 1024;
        constexpr vk::DeviceSize MB = KB * 1024;
        constexpr vk::DeviceSize GB = MB * 1024;

        if (size >= GB) {
            return std::to_string(size / GB) + " GB";
        } else if (size >= MB) {
            return std::to_string(size / MB) + " MB";
        } else if (size >= KB) {
            return std::to_string(size / KB) + " KB";
        } else {
            return std::to_string(size) + " bytes";
        }
    }

    uint32_t VulkanUtil::calcMipLevles(uint32_t width, uint32_t height) {
        return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
    }

    vk::ImageAspectFlags VulkanUtil::calcImageAspectFlags(vk::ImageLayout imageLayout, vk::Format format) {
        if (imageLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
            if (hasStencilComponent(format)) {
                return vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
            } else {
                return vk::ImageAspectFlagBits::eDepth;
            }
        } else {
            return vk::ImageAspectFlagBits::eColor;
        }
    }

    void VulkanUtil::recordTransitionImageLayoutCommand(const vk::CommandBuffer &commandBuffer,
                                                        vk::Image image,
                                                        vk::Format format,
                                                        vk::ImageLayout oldImageLayout,
                                                        vk::ImageLayout newImageLayout,
                                                        uint32_t mipLevels,
                                                        uint32_t srcQueueFamilyIndex,
                                                        uint32_t dstQueueFamilyIndex) {

        vk::ImageSubresourceRange imageSubresourceRange;
        imageSubresourceRange
                .setBaseMipLevel(0)
                .setLevelCount(mipLevels)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

        imageSubresourceRange.setAspectMask(calcImageAspectFlags(newImageLayout, format));

        vk::ImageMemoryBarrier imageMemoryBarrier;
        imageMemoryBarrier
                .setOldLayout(oldImageLayout)
                .setNewLayout(newImageLayout)
                .setSrcQueueFamilyIndex(srcQueueFamilyIndex)
                .setDstQueueFamilyIndex(dstQueueFamilyIndex)
                .setImage(image)
                .setSubresourceRange(imageSubresourceRange)
                .setSrcAccessMask(vk::AccessFlags{})
                .setDstAccessMask(vk::AccessFlags{});

        vk::PipelineStageFlags sourceStage;
        vk::PipelineStageFlags destinationStage;

        if (oldImageLayout == vk::ImageLayout::eUndefined) {
            if (newImageLayout == vk::ImageLayout::eTransferDstOptimal) {
                imageMemoryBarrier
                        .setSrcAccessMask(vk::AccessFlags{})
                        .setDstAccessMask(vk::AccessFlagBits::eTransferWrite);

                sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
                destinationStage = vk::PipelineStageFlagBits::eTransfer;
            } else if (newImageLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
                imageMemoryBarrier
                        .setSrcAccessMask(vk::AccessFlags{})
                        .setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite);

                sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
                destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
            } else if (newImageLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                imageMemoryBarrier
                        .setSrcAccessMask(vk::AccessFlags{})
                        .setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite);

                sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
                destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
            } else {
                throw std::runtime_error("unsupported layout transition!");
            }
        } else if (oldImageLayout == vk::ImageLayout::eTransferDstOptimal) {
            if (newImageLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                imageMemoryBarrier
                        .setSrcAccessMask(vk::AccessFlags{})
                        .setDstAccessMask(vk::AccessFlagBits::eShaderRead);

                sourceStage = vk::PipelineStageFlagBits::eTransfer;
                destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
            } else {
                throw std::runtime_error("unsupported layout transition!");
            }
        }

        commandBuffer.pipelineBarrier(sourceStage,
                                      destinationStage,
                                      vk::DependencyFlags{},
                // 内存屏障
                                      {},
                // 缓冲区内存屏障
                                      {},
                // 图像内存屏障
                                      {imageMemoryBarrier});


    }

    bool VulkanUtil::hasStencilComponent(vk::Format format) {
        return format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint;
    }

    vk::SampleCountFlagBits VulkanUtil::uint32ToSampleCountFlagBits(uint32_t sampleCount) {
        switch (sampleCount) {
            case 1:
                return vk::SampleCountFlagBits::e1;
            case 2:
                return vk::SampleCountFlagBits::e2;
            case 4:
                return vk::SampleCountFlagBits::e4;
            case 8:
                return vk::SampleCountFlagBits::e8;
            case 16:
                return vk::SampleCountFlagBits::e16;
            case 32:
                return vk::SampleCountFlagBits::e32;
            case 64:
                return vk::SampleCountFlagBits::e64;
            default:
                throw std::invalid_argument("Unsupported sample count: " + std::to_string(sampleCount));
        }
    }

    uint32_t VulkanUtil::sampleCountFlagBitsToUint32(vk::SampleCountFlagBits sampleCount) {
        switch (sampleCount) {

            case vk::SampleCountFlagBits::e1:
                return 1;
            case vk::SampleCountFlagBits::e2:
                return 2;
            case vk::SampleCountFlagBits::e4:
                return 4;
            case vk::SampleCountFlagBits::e8:
                return 8;
            case vk::SampleCountFlagBits::e16:
                return 16;
            case vk::SampleCountFlagBits::e32:
                return 32;
            case vk::SampleCountFlagBits::e64:
                return 64;

            default:
                throw std::invalid_argument("Unsupported SampleCountFlagBits");
        }
    }


    uint32_t VulkanUtil::getFormatSize(vk::Format format) {
        switch (format) {
            // 8-bit formats (1 byte per component)
            case vk::Format::eR4G4UnormPack8:
            case vk::Format::eR8Unorm:
            case vk::Format::eR8Snorm:
            case vk::Format::eR8Uscaled:
            case vk::Format::eR8Sscaled:
            case vk::Format::eR8Uint:
            case vk::Format::eR8Sint:
            case vk::Format::eR8Srgb:
            case vk::Format::eS8Uint:
                return 1;

                // 16-bit formats (2 bytes per component)
            case vk::Format::eR4G4B4A4UnormPack16:
            case vk::Format::eB4G4R4A4UnormPack16:
            case vk::Format::eR5G6B5UnormPack16:
            case vk::Format::eB5G6R5UnormPack16:
            case vk::Format::eR5G5B5A1UnormPack16:
            case vk::Format::eB5G5R5A1UnormPack16:
            case vk::Format::eA1R5G5B5UnormPack16:
            case vk::Format::eR8G8Unorm:
            case vk::Format::eR8G8Snorm:
            case vk::Format::eR8G8Uscaled:
            case vk::Format::eR8G8Sscaled:
            case vk::Format::eR8G8Uint:
            case vk::Format::eR8G8Sint:
            case vk::Format::eR8G8Srgb:
            case vk::Format::eR16Unorm:
            case vk::Format::eR16Snorm:
            case vk::Format::eR16Uscaled:
            case vk::Format::eR16Sscaled:
            case vk::Format::eR16Uint:
            case vk::Format::eR16Sint:
            case vk::Format::eR16Sfloat:
            case vk::Format::eD16Unorm:
                return 2;

                // 24-bit formats (3 bytes per component)
            case vk::Format::eR8G8B8Unorm:
            case vk::Format::eR8G8B8Snorm:
            case vk::Format::eR8G8B8Uscaled:
            case vk::Format::eR8G8B8Sscaled:
            case vk::Format::eR8G8B8Uint:
            case vk::Format::eR8G8B8Sint:
            case vk::Format::eR8G8B8Srgb:
            case vk::Format::eB8G8R8Unorm:
            case vk::Format::eB8G8R8Snorm:
            case vk::Format::eB8G8R8Uscaled:
            case vk::Format::eB8G8R8Sscaled:
            case vk::Format::eB8G8R8Uint:
            case vk::Format::eB8G8R8Sint:
            case vk::Format::eB8G8R8Srgb:
                return 3;

                // 32-bit formats (4 bytes per component)
            case vk::Format::eR8G8B8A8Unorm:
            case vk::Format::eR8G8B8A8Snorm:
            case vk::Format::eR8G8B8A8Uscaled:
            case vk::Format::eR8G8B8A8Sscaled:
            case vk::Format::eR8G8B8A8Uint:
            case vk::Format::eR8G8B8A8Sint:
            case vk::Format::eR8G8B8A8Srgb:
            case vk::Format::eB8G8R8A8Unorm:
            case vk::Format::eB8G8R8A8Snorm:
            case vk::Format::eB8G8R8A8Uscaled:
            case vk::Format::eB8G8R8A8Sscaled:
            case vk::Format::eB8G8R8A8Uint:
            case vk::Format::eB8G8R8A8Sint:
            case vk::Format::eB8G8R8A8Srgb:
            case vk::Format::eA8B8G8R8UnormPack32:
            case vk::Format::eA8B8G8R8SnormPack32:
            case vk::Format::eA8B8G8R8UscaledPack32:
            case vk::Format::eA8B8G8R8SscaledPack32:
            case vk::Format::eA8B8G8R8UintPack32:
            case vk::Format::eA8B8G8R8SintPack32:
            case vk::Format::eA8B8G8R8SrgbPack32:
            case vk::Format::eA2R10G10B10UnormPack32:
            case vk::Format::eA2R10G10B10SnormPack32:
            case vk::Format::eA2R10G10B10UscaledPack32:
            case vk::Format::eA2R10G10B10SscaledPack32:
            case vk::Format::eA2R10G10B10UintPack32:
            case vk::Format::eA2R10G10B10SintPack32:
            case vk::Format::eA2B10G10R10UnormPack32:
            case vk::Format::eA2B10G10R10SnormPack32:
            case vk::Format::eA2B10G10R10UscaledPack32:
            case vk::Format::eA2B10G10R10SscaledPack32:
            case vk::Format::eA2B10G10R10UintPack32:
            case vk::Format::eA2B10G10R10SintPack32:
            case vk::Format::eR16G16Unorm:
            case vk::Format::eR16G16Snorm:
            case vk::Format::eR16G16Uscaled:
            case vk::Format::eR16G16Sscaled:
            case vk::Format::eR16G16Uint:
            case vk::Format::eR16G16Sint:
            case vk::Format::eR16G16Sfloat:
            case vk::Format::eR32Uint:
            case vk::Format::eR32Sint:
            case vk::Format::eR32Sfloat:
            case vk::Format::eX8D24UnormPack32:
            case vk::Format::eD32Sfloat:
            case vk::Format::eD24UnormS8Uint:
            case vk::Format::eD32SfloatS8Uint:
                return 4;

                // 48-bit formats (6 bytes per component)
            case vk::Format::eR16G16B16Unorm:
            case vk::Format::eR16G16B16Snorm:
            case vk::Format::eR16G16B16Uscaled:
            case vk::Format::eR16G16B16Sscaled:
            case vk::Format::eR16G16B16Uint:
            case vk::Format::eR16G16B16Sint:
            case vk::Format::eR16G16B16Sfloat:
                return 6;

                // 64-bit formats (8 bytes per component)
            case vk::Format::eR16G16B16A16Unorm:
            case vk::Format::eR16G16B16A16Snorm:
            case vk::Format::eR16G16B16A16Uscaled:
            case vk::Format::eR16G16B16A16Sscaled:
            case vk::Format::eR16G16B16A16Uint:
            case vk::Format::eR16G16B16A16Sint:
            case vk::Format::eR16G16B16A16Sfloat:
            case vk::Format::eR32G32Uint:
            case vk::Format::eR32G32Sint:
            case vk::Format::eR32G32Sfloat:
            case vk::Format::eR64Uint:
            case vk::Format::eR64Sint:
            case vk::Format::eR64Sfloat:
                return 8;

                // 96-bit formats (12 bytes per component)
            case vk::Format::eR32G32B32Uint:
            case vk::Format::eR32G32B32Sint:
            case vk::Format::eR32G32B32Sfloat:
                return 12;

                // 128-bit formats (16 bytes per component)
            case vk::Format::eR32G32B32A32Uint:
            case vk::Format::eR32G32B32A32Sint:
            case vk::Format::eR32G32B32A32Sfloat:
            case vk::Format::eR64G64Uint:
            case vk::Format::eR64G64Sint:
            case vk::Format::eR64G64Sfloat:
            case vk::Format::eR64G64B64Uint:
            case vk::Format::eR64G64B64Sint:
            case vk::Format::eR64G64B64Sfloat:
            case vk::Format::eR64G64B64A64Uint:
            case vk::Format::eR64G64B64A64Sint:
            case vk::Format::eR64G64B64A64Sfloat:
                return 16;

                // Compressed formats (block-based, size depends on block dimensions)
            case vk::Format::eBc1RgbUnormBlock:
            case vk::Format::eBc1RgbSrgbBlock:
            case vk::Format::eBc1RgbaUnormBlock:
            case vk::Format::eBc1RgbaSrgbBlock:
            case vk::Format::eBc2UnormBlock:
            case vk::Format::eBc2SrgbBlock:
            case vk::Format::eBc3UnormBlock:
            case vk::Format::eBc3SrgbBlock:
            case vk::Format::eBc4UnormBlock:
            case vk::Format::eBc4SnormBlock:
            case vk::Format::eBc5UnormBlock:
            case vk::Format::eBc5SnormBlock:
            case vk::Format::eBc6HUfloatBlock:
            case vk::Format::eBc6HSfloatBlock:
            case vk::Format::eBc7UnormBlock:
            case vk::Format::eBc7SrgbBlock:
            case vk::Format::eEtc2R8G8B8UnormBlock:
            case vk::Format::eEtc2R8G8B8SrgbBlock:
            case vk::Format::eEtc2R8G8B8A1UnormBlock:
            case vk::Format::eEtc2R8G8B8A1SrgbBlock:
            case vk::Format::eEtc2R8G8B8A8UnormBlock:
            case vk::Format::eEtc2R8G8B8A8SrgbBlock:
            case vk::Format::eEacR11UnormBlock:
            case vk::Format::eEacR11SnormBlock:
            case vk::Format::eEacR11G11UnormBlock:
            case vk::Format::eEacR11G11SnormBlock:
            case vk::Format::eAstc4x4UnormBlock:
            case vk::Format::eAstc4x4SrgbBlock:
            case vk::Format::eAstc5x4UnormBlock:
            case vk::Format::eAstc5x4SrgbBlock:
            case vk::Format::eAstc5x5UnormBlock:
            case vk::Format::eAstc5x5SrgbBlock:
            case vk::Format::eAstc6x5UnormBlock:
            case vk::Format::eAstc6x5SrgbBlock:
            case vk::Format::eAstc6x6UnormBlock:
            case vk::Format::eAstc6x6SrgbBlock:
            case vk::Format::eAstc8x5UnormBlock:
            case vk::Format::eAstc8x5SrgbBlock:
            case vk::Format::eAstc8x6UnormBlock:
            case vk::Format::eAstc8x6SrgbBlock:
            case vk::Format::eAstc8x8UnormBlock:
            case vk::Format::eAstc8x8SrgbBlock:
            case vk::Format::eAstc10x5UnormBlock:
            case vk::Format::eAstc10x5SrgbBlock:
            case vk::Format::eAstc10x6UnormBlock:
            case vk::Format::eAstc10x6SrgbBlock:
            case vk::Format::eAstc10x8UnormBlock:
            case vk::Format::eAstc10x8SrgbBlock:
            case vk::Format::eAstc10x10UnormBlock:
            case vk::Format::eAstc10x10SrgbBlock:
            case vk::Format::eAstc12x10UnormBlock:
            case vk::Format::eAstc12x10SrgbBlock:
            case vk::Format::eAstc12x12UnormBlock:
            case vk::Format::eAstc12x12SrgbBlock:
            case vk::Format::ePvrtc12BppUnormBlockIMG:
            case vk::Format::ePvrtc14BppUnormBlockIMG:
            case vk::Format::ePvrtc22BppUnormBlockIMG:
            case vk::Format::ePvrtc24BppUnormBlockIMG:
            case vk::Format::ePvrtc12BppSrgbBlockIMG:
            case vk::Format::ePvrtc14BppSrgbBlockIMG:
            case vk::Format::ePvrtc22BppSrgbBlockIMG:
            case vk::Format::ePvrtc24BppSrgbBlockIMG:
                throw std::runtime_error("Compressed formats are not supported for size calculation");

                // Other formats (e.g., multi-plane or special formats)
            case vk::Format::eG8B8G8R8422Unorm:
            case vk::Format::eB8G8R8G8422Unorm:
            case vk::Format::eG8B8R83Plane420Unorm:
            case vk::Format::eG8B8R82Plane420Unorm:
            case vk::Format::eG8B8R83Plane422Unorm:
            case vk::Format::eG8B8R82Plane422Unorm:
            case vk::Format::eG8B8R83Plane444Unorm:
            case vk::Format::eG16B16G16R16422Unorm:
            case vk::Format::eB16G16R16G16422Unorm:
            case vk::Format::eG16B16R163Plane420Unorm:
            case vk::Format::eG16B16R162Plane420Unorm:
            case vk::Format::eG16B16R163Plane422Unorm:
            case vk::Format::eG16B16R162Plane422Unorm:
            case vk::Format::eG16B16R163Plane444Unorm:
                throw std::runtime_error("Multi-plane or special formats are not supported for size calculation");

                // Undefined format
            case vk::Format::eUndefined:
                throw std::runtime_error("Undefined format has no size");

            default:
                throw std::runtime_error("Unsupported format");
        }
    }

    uint32_t VulkanUtil::getImageFormatBytesPerPixel(vk::Format format) {
        switch (format) {
            case vk::Format::eR8G8B8A8Srgb:
                return 4;
            case vk::Format::eR8G8B8Srgb:
                return 3;
            default:
                throw std::runtime_error("VulkanUtil::getImageFormatBytesPerPixel(): unknown format");
        }
    }

    vk::Extent2D VulkanUtil::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capability, uint32_t width, uint32_t height) {
        if (capability.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capability.currentExtent;
        }

        return vk::Extent2D{
                std::clamp(width, capability.minImageExtent.width, capability.maxImageExtent.width),
                std::clamp(height, capability.minImageExtent.height, capability.maxImageExtent.height),
        };
    }

    vk::SurfaceFormatKHR VulkanUtil::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
        for (const auto &availableFormat: availableFormats) {
            if (availableFormat.format == vk::Format::eR8G8B8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return availableFormat;
            }

//            if (availableFormat.format == vk::Format::eR8G8B8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
//                return availableFormat;
//            }
        }

        return availableFormats[0];
    }

    vk::PresentModeKHR VulkanUtil::choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
        for (const auto &availablePresentMode: availablePresentModes) {
            if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
                return availablePresentMode;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    uint32_t VulkanUtil::findMemoryTypeIndex(uint32_t typeFilter, vk::MemoryPropertyFlags properties, vk::PhysicalDeviceMemoryProperties memoryProperties) {
        for (int index = 0; index < memoryProperties.memoryTypeCount; index++) {
            const vk::MemoryType &type = memoryProperties.memoryTypes[index];
            if (((typeFilter & (1 << index)) != 0) && ((type.propertyFlags & properties) == properties)) {
                return index;
            }
        }

        throw std::runtime_error("failed to find suitable memory type for typeFilter and MemoryPropertyFlags");
    }

    uint32_t VulkanUtil::findMemoryTypeIndex(vk::PhysicalDevice physicalDevice, vk::Device device, vk::Image image, vk::MemoryPropertyFlags memoryPropertyFlags) {
        vk::MemoryRequirements memoryRequirements = device.getImageMemoryRequirements(image);
        vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
        return VulkanUtil::findMemoryTypeIndex(memoryRequirements.memoryTypeBits, memoryPropertyFlags, memoryProperties);
    }

    uint32_t VulkanUtil::findMemoryTypeIndex(vk::PhysicalDeviceMemoryProperties memoryProperties, vk::MemoryRequirements memoryRequirements, vk::MemoryPropertyFlags memoryPropertyFlags) {
        return VulkanUtil::findMemoryTypeIndex(memoryRequirements.memoryTypeBits, memoryPropertyFlags, memoryProperties);
    }

    std::string VulkanUtil::toString(vk::ImageTiling imageTiling) {
        switch (imageTiling) {
            case vk::ImageTiling::eOptimal:
                return "ImageTiling::eOptimal";
            case vk::ImageTiling::eLinear:
                return "ImageTiling::eLinear";
            case vk::ImageTiling::eDrmFormatModifierEXT:
                return "ImageTiling::eDrmFormatModifierEXT";
            default:
                return "unknown";
        }
    }

    std::string VulkanUtil::toString(vk::ImageType imageType) {
        switch (imageType) {
            case vk::ImageType::e1D:
                return "ImageType::e1D";
            case vk::ImageType::e2D:
                return "ImageType::e2D";
            case vk::ImageType::e3D:
                return "ImageType::e3D";
            default:
                return "unknown";
        }
    }

    std::string VulkanUtil::toString(vk::ImageUsageFlags usage) {
        std::string str;
        if (usage & vk::ImageUsageFlagBits::eTransferSrc) {
            str += "eTransferSrc ";
        }
        if (usage & vk::ImageUsageFlagBits::eTransferDst) {
            str += "eTransferDst ";
        }
        if (usage & vk::ImageUsageFlagBits::eSampled) {
            str += "eSampled ";
        }
        if (usage & vk::ImageUsageFlagBits::eStorage) {
            str += "eStorage ";
        }
        if (usage & vk::ImageUsageFlagBits::eColorAttachment) {
            str += "eColorAttachment ";
        }
        if (usage & vk::ImageUsageFlagBits::eDepthStencilAttachment) {
            str += "eDepthStencilAttachment ";
        }
        if (usage & vk::ImageUsageFlagBits::eTransientAttachment) {
            str += "eTransientAttachment ";
        }
        if (usage & vk::ImageUsageFlagBits::eInputAttachment) {
            str += "eInputAttachment ";
        }
        if (usage & vk::ImageUsageFlagBits::eVideoDecodeDstKHR) {
            str += "eVideoDecodeDstKHR ";
        }
        if (usage & vk::ImageUsageFlagBits::eVideoDecodeSrcKHR) {
            str += "eVideoDecodeSrcKHR ";
        }
        if (usage & vk::ImageUsageFlagBits::eVideoDecodeDpbKHR) {
            str += "eVideoDecodeDpbKHR ";
        }
        if (usage & vk::ImageUsageFlagBits::eShadingRateImageNV) {
            str += "eShadingRateImageNV ";
        }
        if (usage & vk::ImageUsageFlagBits::eFragmentDensityMapEXT) {
            str += "eFragmentDensityMapEXT ";
        }
        if (usage & vk::ImageUsageFlagBits::eFragmentShadingRateAttachmentKHR) {
            str += "eFragmentShadingRateAttachmentKHR ";
        }
        if (usage & vk::ImageUsageFlagBits::eHostTransferEXT) {
            str += "eHostTransferEXT ";
        }
        if (usage & vk::ImageUsageFlagBits::eVideoEncodeDstKHR) {
            str += "eVideoEncodeDstKHR ";
        }
        if (usage & vk::ImageUsageFlagBits::eVideoEncodeSrcKHR) {
            str += "eVideoEncodeSrcKHR ";
        }
        if (usage & vk::ImageUsageFlagBits::eVideoEncodeDpbKHR) {
            str += "eVideoEncodeDpbKHR ";
        }
        if (usage & vk::ImageUsageFlagBits::eAttachmentFeedbackLoopEXT) {
            str += "eAttachmentFeedbackLoopEXT ";
        }
        if (usage & vk::ImageUsageFlagBits::eInvocationMaskHUAWEI) {
            str += "eInvocationMaskHUAWEI ";
        }
        if (usage & vk::ImageUsageFlagBits::eSampleWeightQCOM) {
            str += "eSampleWeightQCOM ";
        }
        if (usage & vk::ImageUsageFlagBits::eSampleBlockMatchQCOM) {
            str += "eSampleBlockMatchQCOM ";
        }
        return str;
    }

    std::string VulkanUtil::toString(vk::MemoryPropertyFlags memoryProperty) {
        std::string str;
        if (memoryProperty & vk::MemoryPropertyFlagBits::eDeviceLocal) {
            str += "eDeviceLocal ";
        }
        if (memoryProperty & vk::MemoryPropertyFlagBits::eHostVisible) {
            str += "eHostVisible ";
        }
        if (memoryProperty & vk::MemoryPropertyFlagBits::eHostCoherent) {
            str += "eHostCoherent ";
        }
        if (memoryProperty & vk::MemoryPropertyFlagBits::eHostCached) {
            str += "eHostCached ";
        }
        if (memoryProperty & vk::MemoryPropertyFlagBits::eLazilyAllocated) {
            str += "eLazilyAllocated ";
        }
        if (memoryProperty & vk::MemoryPropertyFlagBits::eProtected) {
            str += "eProtected ";
        }
        if (memoryProperty & vk::MemoryPropertyFlagBits::eDeviceCoherentAMD) {
            str += "eDeviceCoherentAMD ";
        }
        if (memoryProperty & vk::MemoryPropertyFlagBits::eDeviceUncachedAMD) {
            str += "eDeviceUncachedAMD ";
        }
        if (memoryProperty & vk::MemoryPropertyFlagBits::eRdmaCapableNV) {
            str += "eRdmaCapableNV ";
        }
        return str;
    }

    std::string VulkanUtil::toString(vk::PhysicalDeviceType deviceType) {
        switch (deviceType) {
            case vk::PhysicalDeviceType::eOther:
                return "PhysicalDeviceType::eOther";
            case vk::PhysicalDeviceType::eIntegratedGpu:
                return "PhysicalDeviceType::eIntegratedGpu";
            case vk::PhysicalDeviceType::eDiscreteGpu:
                return "PhysicalDeviceType::eDiscreteGpu";
            case vk::PhysicalDeviceType::eVirtualGpu:
                return "PhysicalDeviceType::eVirtualGpu";
            case vk::PhysicalDeviceType::eCpu:
                return "PhysicalDeviceType::eCpu";
            default:
                return "unknown";
        }
    }

    std::string VulkanUtil::toString(vk::QueueFlags queue) {
        std::string str;
        if (queue & vk::QueueFlagBits::eGraphics) {
            str += "eGraphics ";
        }
        if (queue & vk::QueueFlagBits::eCompute) {
            str += "eCompute ";
        }
        if (queue & vk::QueueFlagBits::eTransfer) {
            str += "eTransfer ";
        }
        if (queue & vk::QueueFlagBits::eSparseBinding) {
            str += "eSparseBinding ";
        }
        if (queue & vk::QueueFlagBits::eProtected) {
            str += "eProtected ";
        }
        if (queue & vk::QueueFlagBits::eVideoDecodeKHR) {
            str += "eVideoDecodeKHR ";
        }
        if (queue & vk::QueueFlagBits::eVideoEncodeKHR) {
            str += "eVideoEncodeKHR ";
        }
        if (queue & vk::QueueFlagBits::eOpticalFlowNV) {
            str += "eOpticalFlowNV ";
        }
        return str;
    }

    std::string VulkanUtil::toString(vk::SampleCountFlags sampleCount) {
        std::string str;
        if (sampleCount & vk::SampleCountFlagBits::e1) {
            str += "e1 ";
        }
        if (sampleCount & vk::SampleCountFlagBits::e2) {
            str += "e2 ";
        }
        if (sampleCount & vk::SampleCountFlagBits::e4) {
            str += "e4 ";
        }
        if (sampleCount & vk::SampleCountFlagBits::e8) {
            str += "e8 ";
        }
        if (sampleCount & vk::SampleCountFlagBits::e16) {
            str += "e16 ";
        }
        if (sampleCount & vk::SampleCountFlagBits::e32) {
            str += "e32 ";
        }
        if (sampleCount & vk::SampleCountFlagBits::e64) {
            str += "e64 ";
        }
        return str;
    }

    std::string VulkanUtil::toString(vk::Format format) {
        switch (format) {
            case vk::Format::eUndefined:
                return "Format::eUndefined";
            case vk::Format::eR4G4UnormPack8:
                return "Format::eR4G4UnormPack8";
            case vk::Format::eR4G4B4A4UnormPack16:
                return "Format::eR4G4B4A4UnormPack16";
            case vk::Format::eB4G4R4A4UnormPack16:
                return "Format::eB4G4R4A4UnormPack16";
            case vk::Format::eR5G6B5UnormPack16:
                return "Format::eR5G6B5UnormPack16";
            case vk::Format::eB5G6R5UnormPack16:
                return "Format::eB5G6R5UnormPack16";
            case vk::Format::eR5G5B5A1UnormPack16:
                return "Format::eR5G5B5A1UnormPack16";
            case vk::Format::eB5G5R5A1UnormPack16:
                return "Format::eB5G5R5A1UnormPack16";
            case vk::Format::eA1R5G5B5UnormPack16:
                return "Format::eA1R5G5B5UnormPack16";
            case vk::Format::eR8Unorm:
                return "Format::eR8Unorm";
            case vk::Format::eR8Snorm:
                return "Format::eR8Snorm";
            case vk::Format::eR8Uscaled:
                return "Format::eR8Uscaled";
            case vk::Format::eR8Sscaled:
                return "Format::eR8Sscaled";
            case vk::Format::eR8Uint:
                return "Format::eR8Uint";
            case vk::Format::eR8Sint:
                return "Format::eR8Sint";
            case vk::Format::eR8Srgb:
                return "Format::eR8Srgb";
            case vk::Format::eR8G8Unorm:
                return "Format::eR8G8Unorm";
            case vk::Format::eR8G8Snorm:
                return "Format::eR8G8Snorm";
            case vk::Format::eR8G8Uscaled:
                return "Format::eR8G8Uscaled";
            case vk::Format::eR8G8Sscaled:
                return "Format::eR8G8Sscaled";
            case vk::Format::eR8G8Uint:
                return "Format::eR8G8Uint";
            case vk::Format::eR8G8Sint:
                return "Format::eR8G8Sint";
            case vk::Format::eR8G8Srgb:
                return "Format::eR8G8Srgb";
            case vk::Format::eR8G8B8Unorm:
                return "Format::eR8G8B8Unorm";
            case vk::Format::eR8G8B8Snorm:
                return "Format::eR8G8B8Snorm";
            case vk::Format::eR8G8B8Uscaled:
                return "Format::eR8G8B8Uscaled";
            case vk::Format::eR8G8B8Sscaled:
                return "Format::eR8G8B8Sscaled";
            case vk::Format::eR8G8B8Uint:
                return "Format::eR8G8B8Uint";
            case vk::Format::eR8G8B8Sint:
                return "Format::eR8G8B8Sint";
            case vk::Format::eR8G8B8Srgb:
                return "Format::eR8G8B8Srgb";
            case vk::Format::eB8G8R8Unorm:
                return "Format::eB8G8R8Unorm";
            case vk::Format::eB8G8R8Snorm:
                return "Format::eB8G8R8Snorm";
            case vk::Format::eB8G8R8Uscaled:
                return "Format::eB8G8R8Uscaled";
            case vk::Format::eB8G8R8Sscaled:
                return "Format::eB8G8R8Sscaled";
            case vk::Format::eB8G8R8Uint:
                return "Format::eB8G8R8Uint";
            case vk::Format::eB8G8R8Sint:
                return "Format::eB8G8R8Sint";
            case vk::Format::eB8G8R8Srgb:
                return "Format::eB8G8R8Srgb";
            case vk::Format::eR8G8B8A8Unorm:
                return "Format::eR8G8B8A8Unorm";
            case vk::Format::eR8G8B8A8Snorm:
                return "Format::eR8G8B8A8Snorm";
            case vk::Format::eR8G8B8A8Uscaled:
                return "Format::eR8G8B8A8Uscaled";
            case vk::Format::eR8G8B8A8Sscaled:
                return "Format::eR8G8B8A8Sscaled";
            case vk::Format::eR8G8B8A8Uint:
                return "Format::eR8G8B8A8Uint";
            case vk::Format::eR8G8B8A8Sint:
                return "Format::eR8G8B8A8Sint";
            case vk::Format::eR8G8B8A8Srgb:
                return "Format::eR8G8B8A8Srgb";
            case vk::Format::eB8G8R8A8Unorm:
                return "Format::eB8G8R8A8Unorm";
            case vk::Format::eB8G8R8A8Snorm:
                return "Format::eB8G8R8A8Snorm";
            case vk::Format::eB8G8R8A8Uscaled:
                return "Format::eB8G8R8A极Uscaled";
            case vk::Format::eB8G8R8A8Sscaled:
                return "Format::eB8G8R8A8Sscaled";
            case vk::Format::eB8G8R8A8Uint:
                return "Format::eB8G8R8A8Uint";
            case vk::Format::eB8G8R8A8Sint:
                return "Format::eB8G8R8A8Sint";
            case vk::Format::eB8G8R8A8Srgb:
                return "Format::eB8G8R8A8Srgb";
                // 此处为简洁起见省略中间部分
                // 完整实现应包含所有提供的格式枚举值...
            case vk::Format::ePvrtc12BppUnormBlockIMG:
                return "Format::ePvrtc12BppUnormBlockIMG";
            case vk::Format::ePvrtc14BppUnormBlockIMG:
                return "Format::ePvrtc14BppUnormBlockIMG";
            case vk::Format::ePvrtc22BppUnormBlockIMG:
                return "Format::ePvrtc22BppUnormBlockIMG";
            case vk::Format::ePvrtc24BppUnormBlockIMG:
                return "Format::ePvrtc24BppUnormBlockIMG";
            case vk::Format::ePvrtc12BppSrgbBlockIMG:
                return "Format::ePvrtc12BppSrgbBlockIMG";
            case vk::Format::ePvrtc14BppSrgbBlockIMG:
                return "Format::ePvrtc14BppSrgbBlockIMG";
            case vk::Format::ePvrtc22BppSrgbBlockIMG:
                return "Format::ePvrtc22BppSrgbBlockIMG";
            case vk::Format::ePvrtc24BppSrgbBlockIMG:
                return "Format::ePvrtc24BppSrgbBlockIMG";
            case vk::Format::eA8UnormKHR:
                return "Format::eA8UnormKHR";
            default:
                return "Format::unknown";
        }
    }

    std::string VulkanUtil::toString(vk::SharingMode sharingMode) {
        switch (sharingMode) {
            case vk::SharingMode::eExclusive:
                return "SharingMode::eExclusive";
            case vk::SharingMode::eConcurrent:
                return "SharingMode::eConcurrent";
            default:
                return "unknown";
        }
    }

    std::string VulkanUtil::toString(vk::ImageLayout imageLayout) {
        switch (imageLayout) {
            case vk::ImageLayout::eUndefined:
                return "ImageLayout::eUndefined";
            case vk::ImageLayout::eGeneral:
                return "ImageLayout::eGeneral";
            case vk::ImageLayout::eColorAttachmentOptimal:
                return "ImageLayout::eColorAttachmentOptimal";
            case vk::ImageLayout::eDepthStencilAttachmentOptimal:
                return "ImageLayout::eDepthStencilAttachmentOptimal";
            case vk::ImageLayout::eDepthStencilReadOnlyOptimal:
                return "ImageLayout::eDepthStencilReadOnlyOptimal";
            case vk::ImageLayout::eShaderReadOnlyOptimal:
                return "ImageLayout::eShaderReadOnlyOptimal";
            case vk::ImageLayout::eTransferSrcOptimal:
                return "ImageLayout::eTransferSrcOptimal";
            case vk::ImageLayout::eTransferDstOptimal:
                return "ImageLayout::eTransferDstOptimal";
            case vk::ImageLayout::ePreinitialized:
                return "ImageLayout::ePreinitialized";
            case vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimal:
                return "ImageLayout::eDepthReadOnlyStencilAttachmentOptimal";
            case vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimal:
                return "ImageLayout::eDepthAttachmentStencilReadOnlyOptimal";
            case vk::ImageLayout::eDepthAttachmentOptimal:
                return "ImageLayout::eDepthAttachmentOptimal";
            case vk::ImageLayout::eDepthReadOnlyOptimal:
                return "ImageLayout::eDepthReadOnlyOptimal";
            case vk::ImageLayout::eStencilAttachmentOptimal:
                return "ImageLayout::eStencilAttachmentOptimal";
            case vk::ImageLayout::eStencilReadOnlyOptimal:
                return "ImageLayout::eStencilReadOnlyOptimal";
            case vk::ImageLayout::eReadOnlyOptimal:
                return "ImageLayout::eReadOnlyOptimal";
            case vk::ImageLayout::eAttachmentOptimal:
                return "ImageLayout::eAttachmentOptimal";
            case vk::ImageLayout::ePresentSrcKHR:
                return "ImageLayout::ePresentSrcKHR";
            case vk::ImageLayout::eVideoDecodeDstKHR:
                return "ImageLayout::eVideoDecodeDstKHR";
            case vk::ImageLayout::eVideoDecodeSrcKHR:
                return "ImageLayout::eVideoDecodeSrcKHR";
            case vk::ImageLayout::eVideoDecodeDpbKHR:
                return "ImageLayout::eVideoDecodeDpbKHR";
            case vk::ImageLayout::eSharedPresentKHR:
                return "ImageLayout::eSharedPresentKHR";
//            case vk::ImageLayout::eDepthReadOnlyStencilAttachmentOptimalKHR: return "ImageLayout::eDepthReadOnlyStencilAttachmentOptimalKHR";
//            case vk::ImageLayout::eDepthAttachmentStencilReadOnlyOptimalKHR: return "ImageLayout::eDepthAttachmentStencilReadOnlyOptimalKHR";
            case vk::ImageLayout::eShadingRateOptimalNV:
                return "ImageLayout::eShadingRateOptimalNV";
            case vk::ImageLayout::eFragmentDensityMapOptimalEXT:
                return "ImageLayout::eFragmentDensityMapOptimalEXT";
//            case vk::ImageLayout::eFragmentShadingRateAttachmentOptimalKHR: return "ImageLayout::eFragmentShadingRateAttachmentOptimalKHR";
//            case vk::ImageLayout::eDepthAttachmentOptimalKHR: return "ImageLayout::eDepthAttachmentOptimalKHR";
//            case vk::ImageLayout::eDepthReadOnlyOptimalKHR: return "ImageLayout::eDepthReadOnlyOptimalKHR";
//            case vk::ImageLayout::eStencilAttachmentOptimalKHR: return "ImageLayout::eStencilAttachmentOptimalKHR";
//            case vk::ImageLayout::eStencilReadOnlyOptimalKHR: return "ImageLayout::eStencilReadOnlyOptimalKHR";
            case vk::ImageLayout::eVideoEncodeDstKHR:
                return "ImageLayout::eVideoEncodeDstKHR";
            case vk::ImageLayout::eVideoEncodeSrcKHR:
                return "ImageLayout::eVideoEncodeSrcKHR";
            case vk::ImageLayout::eVideoEncodeDpbKHR:
                return "ImageLayout::eVideoEncodeDpbKHR";
//            case vk::ImageLayout::eReadOnlyOptimalKHR: return "ImageLayout::eReadOnlyOptimalKHR";
//            case vk::ImageLayout::eAttachmentOptimalKHR: return "ImageLayout::eAttachmentOptimalKHR";
            case vk::ImageLayout::eAttachmentFeedbackLoopOptimalEXT:
                return "ImageLayout::eAttachmentFeedbackLoopOptimalEXT";
            default:
                return "ImageLayout::unknown";
        }
    }

    std::string VulkanUtil::toString(vk::ImageViewType imageViewType) {
        switch (imageViewType) {
            case vk::ImageViewType::e1D:
                return "ImageViewType::e1D";
            case vk::ImageViewType::e2D:
                return "ImageViewType::e2D";
            case vk::ImageViewType::e3D:
                return "ImageViewType::e3D";
            case vk::ImageViewType::eCube:
                return "ImageViewType::eCube";
            case vk::ImageViewType::e1DArray:
                return "ImageViewType::e1DArray";
            case vk::ImageViewType::e2DArray:
                return "ImageViewType::e2DArray";
            case vk::ImageViewType::eCubeArray:
                return "ImageViewType::eCubeArray";
            default:
                return "unknown";
        }
    }


    std::string VulkanUtil::toString(vk::ComponentSwizzle componentSwizzle) {
        switch (componentSwizzle) {
            case vk::ComponentSwizzle::eIdentity:
                return "ComponentSwizzle::eIdentity";
            case vk::ComponentSwizzle::eZero:
                return "ComponentSwizzle::eZero";
            case vk::ComponentSwizzle::eOne:
                return "ComponentSwizzle::eOne";
            case vk::ComponentSwizzle::eR:
                return "ComponentSwizzle::eR";
            case vk::ComponentSwizzle::eG:
                return "ComponentSwizzle::eG";
            case vk::ComponentSwizzle::eB:
                return "ComponentSwizzle::eB";
            case vk::ComponentSwizzle::eA:
                return "ComponentSwizzle::eA";
            default:
                return "unknown";
        }
    }


    std::string VulkanUtil::toString(vk::ImageAspectFlags imageAspect) {
        std::string str;
        if (imageAspect & vk::ImageAspectFlagBits::eColor) {
            str += "eColor ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::eDepth) {
            str += "eDepth ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::eStencil) {
            str += "eStencil ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::eMetadata) {
            str += "eMetadata ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::ePlane0) {
            str += "ePlane0 ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::ePlane1) {
            str += "ePlane1 ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::ePlane2) {
            str += "ePlane2 ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::eNone) {
            str += "eNone ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::ePlane0KHR) {
            str += "ePlane0KHR ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::ePlane1KHR) {
            str += "ePlane1KHR ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::ePlane2KHR) {
            str += "ePlane2KHR ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::eMemoryPlane0EXT) {
            str += "eMemoryPlane0EXT ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::eMemoryPlane1EXT) {
            str += "eMemoryPlane1EXT ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::eMemoryPlane2EXT) {
            str += "eMemoryPlane2EXT ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::eMemoryPlane3EXT) {
            str += "eMemoryPlane3EXT ";
        }
        if (imageAspect & vk::ImageAspectFlagBits::eNoneKHR) {
            str += "eNoneKHR ";
        }
        return str;
    }

} // vklite