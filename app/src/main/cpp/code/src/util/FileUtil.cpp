//
// Created by leixing on 2025/1/5.
//

#include "FileUtil.h"


std::vector<char> FileUtil::loadFile(AAssetManager *assetManager, const char *filePath) {
    // 打开文件
    AAsset *file = AAssetManager_open(assetManager, filePath, AASSET_MODE_BUFFER);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + std::string(filePath));
    }

    // 获取文件大小
    size_t fileLength = AAsset_getLength(file);

    // 读取文件内容
    std::vector<char> fileContent(fileLength); // 自动管理内存
    AAsset_read(file, fileContent.data(), fileLength);

    // 关闭文件
    AAsset_close(file);

    // 返回文件内容和大小
    return fileContent;
}

std::vector<uint32_t> FileUtil::loadSpvFile(AAssetManager *assetManager, const char *filePath) {
    // 打开文件
    AAsset *file = AAssetManager_open(assetManager, filePath, AASSET_MODE_BUFFER);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + std::string(filePath));
    }

    // 获取文件大小
    size_t fileLength = AAsset_getLength(file);
    if (fileLength == 0) {
        AAsset_close(file);
        throw std::runtime_error("File is empty: " + std::string(filePath));
    }
    if (fileLength % sizeof(uint32_t) != 0) {
        AAsset_close(file);
        throw std::runtime_error("SPIR-V file size is not aligned to 4 bytes: " + std::string(filePath));
    }

    // 读取文件内容
    std::vector<uint32_t> fileContent(fileLength / sizeof(uint32_t)); // 自动管理内存
    int readBytes = AAsset_read(file, fileContent.data(), fileLength);
    if (readBytes != fileLength) {
        AAsset_close(file);
        throw std::runtime_error("Failed to read entire file: " + std::string(filePath));
    }

    // 关闭文件
    AAsset_close(file);

    if (fileContent.empty() || fileContent[0] != 0x07230203) {
        throw std::runtime_error("Invalid SPIR-V magic number in file: " + std::string(filePath));
    }

    // 返回文件内容和大小
    return fileContent;
}