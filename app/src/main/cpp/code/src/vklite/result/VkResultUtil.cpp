//
// Created by leixing on 2025/6/3.
//

#include "VkResultUtil.h"

namespace vklite {

    // 错误码转字符串函数
    const char *VkResultToString(VkResult result) {
        switch (result) {
            // 基础状态码
            case VK_SUCCESS:
                return "VK_SUCCESS";
            case VK_NOT_READY:
                return "VK_NOT_READY";
            case VK_TIMEOUT:
                return "VK_TIMEOUT";
            case VK_EVENT_SET:
                return "VK_EVENT_SET";
            case VK_EVENT_RESET:
                return "VK_EVENT_RESET";
            case VK_INCOMPLETE:
                return "VK_INCOMPLETE";

                // 核心错误码
            case VK_ERROR_OUT_OF_HOST_MEMORY:
                return "VK_ERROR_OUT_OF_HOST_MEMORY";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
            case VK_ERROR_INITIALIZATION_FAILED:
                return "VK_ERROR_INITIALIZATION_FAILED";
            case VK_ERROR_DEVICE_LOST:
                return "VK_ERROR_DEVICE_LOST";
            case VK_ERROR_MEMORY_MAP_FAILED:
                return "VK_ERROR_MEMORY_MAP_FAILED";
            case VK_ERROR_LAYER_NOT_PRESENT:
                return "VK_ERROR_LAYER_NOT_PRESENT";
            case VK_ERROR_EXTENSION_NOT_PRESENT:
                return "VK_ERROR_EXTENSION_NOT_PRESENT";
            case VK_ERROR_FEATURE_NOT_PRESENT:
                return "VK_ERROR_FEATURE_NOT_PRESENT";
            case VK_ERROR_INCOMPATIBLE_DRIVER:
                return "VK_ERROR_INCOMPATIBLE_DRIVER";
            case VK_ERROR_TOO_MANY_OBJECTS:
                return "VK_ERROR_TOO_MANY_OBJECTS";
            case VK_ERROR_FORMAT_NOT_SUPPORTED:
                return "VK_ERROR_FORMAT_NOT_SUPPORTED";
            case VK_ERROR_FRAGMENTED_POOL:
                return "VK_ERROR_FRAGMENTED_POOL";
            case VK_ERROR_UNKNOWN:
                return "VK_ERROR_UNKNOWN";

                // 新增错误码（Vulkan 1.1+）
            case VK_ERROR_OUT_OF_POOL_MEMORY:
                return "VK_ERROR_OUT_OF_POOL_MEMORY";
            case VK_ERROR_INVALID_EXTERNAL_HANDLE:
                return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
            case VK_ERROR_FRAGMENTATION:
                return "VK_ERROR_FRAGMENTATION";
            case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
                return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";

                // 表面相关错误
            case VK_ERROR_SURFACE_LOST_KHR:
                return "VK_ERROR_SURFACE_LOST_KHR";
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
                return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
            case VK_SUBOPTIMAL_KHR:
                return "VK_SUBOPTIMAL_KHR";
            case VK_ERROR_OUT_OF_DATE_KHR:
                return "VK_ERROR_OUT_OF_DATE_KHR";

                // 扩展相关错误
            case VK_ERROR_INVALID_SHADER_NV:
                return "VK_ERROR_INVALID_SHADER_NV";
            case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
                return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
            case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
                return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
            case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
                return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
            case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
                return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
            case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
                return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
            case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:
                return "VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT";

                // 保留值
            case VK_RESULT_MAX_ENUM:
                return "VK_RESULT_MAX_ENUM";

            default:
                return "UNKNOWN_ERROR";
        }
    }

// 错误描述函数
    const char *VkResultDescription(VkResult result) {
        switch (result) {
            // 基础状态描述
            case VK_SUCCESS:
                return "操作成功完成";
            case VK_NOT_READY:
                return "资源尚未准备好";
            case VK_TIMEOUT:
                return "操作超时";
            case VK_EVENT_SET:
                return "事件已触发";
            case VK_EVENT_RESET:
                return "事件已重置";
            case VK_INCOMPLETE:
                return "返回数据不完整（通常用于查询操作）";

                // 核心错误描述
            case VK_ERROR_OUT_OF_HOST_MEMORY:
                return "系统内存耗尽，检查内存泄漏或减少资源使用[9](@ref)";
            case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                return "显存资源耗尽，优化显存管理或减少纹理分辨率[7](@ref)";
            case VK_ERROR_INITIALIZATION_FAILED:
                return "初始化失败，检查参数有效性或硬件兼容性[9](@ref)";
            case VK_ERROR_DEVICE_LOST:
                return "显卡设备发生不可恢复的错误，可能因硬件故障、驱动问题或过热导致[7](@ref)";
            case VK_ERROR_MEMORY_MAP_FAILED:
                return "内存映射失败，检查内存类型是否支持映射操作";
            case VK_ERROR_LAYER_NOT_PRESENT:
                return "请求的校验层未启用或未安装[9](@ref)";
            case VK_ERROR_EXTENSION_NOT_PRESENT:
                return "请求的扩展未在设备上启用，使用vkEnumerateDeviceExtensionProperties验证可用性[9](@ref)";
            case VK_ERROR_FEATURE_NOT_PRESENT:
                return "请求的GPU特性未支持，检查物理设备特性[9](@ref)";
            case VK_ERROR_INCOMPATIBLE_DRIVER:
                return "驱动不兼容，更新显卡驱动到最新版本";
            case VK_ERROR_TOO_MANY_OBJECTS:
                return "创建对象数量超过设备限制";
            case VK_ERROR_FORMAT_NOT_SUPPORTED:
                return "请求的格式不被设备支持";
            case VK_ERROR_FRAGMENTED_POOL:
                return "描述符池内存碎片过多，重建描述符池";
            case VK_ERROR_UNKNOWN:
                return "未知错误，建议启用验证层获取详细信息[10](@ref)";

                // 新增错误描述
            case VK_ERROR_OUT_OF_POOL_MEMORY:
                return "描述符池内存耗尽，增大池大小或减少描述符数量";
            case VK_ERROR_INVALID_EXTERNAL_HANDLE:
                return "无效的外部句柄，检查跨API资源共享配置";
            case VK_ERROR_FRAGMENTATION:
                return "设备内存碎片过多，考虑内存整理策略";
            case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
                return "无效的不透明捕获地址，检查缓冲区创建标志";

                // 表面相关
            case VK_ERROR_SURFACE_LOST_KHR:
                return "表面资源丢失，通常因窗口系统变化导致，需重建交换链";
            case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
                return "原生窗口被其他API占用，确保单API独占窗口";
            case VK_SUBOPTIMAL_KHR:
                return "交换链表面可用但非最佳配置（如旋转/缩放）";
            case VK_ERROR_OUT_OF_DATE_KHR:
                return "交换链过期（窗口尺寸变化），需重建交换链[7](@ref)";

                // 扩展相关
            case VK_ERROR_INVALID_SHADER_NV:
                return "无效的着色器二进制（NVIDIA扩展）";
            case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
                return "请求的图像使用方式不被支持";
            case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
                return "不支持的视频标准版本";
            case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:
                return "着色器二进制与当前设备不兼容";

            default:
                return "未定义的错误码，请查阅最新Vulkan规范";
        }
    }

} // vklite