//
// Created by leixing on 2025/1/5.
//

#pragma once

#include <vector>
#include <cstdint>
#include <android/asset_manager.h>

class FileUtil {
public:

    static std::vector<char> loadFile(AAssetManager *assetManager, const char *filePath);

    static std::vector<uint32_t> loadSpvFile(AAssetManager *assetManager, const char *filePath);

};