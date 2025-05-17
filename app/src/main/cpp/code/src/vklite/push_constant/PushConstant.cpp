//
// Created by leixing on 2025/3/10.
//

#include "PushConstant.h"

namespace vklite {

    PushConstant::PushConstant(const void *data, uint32_t size, uint32_t offset,
                               vk::ShaderStageFlags stageFlags)
            : mOffset(offset), mStageFlags(stageFlags) {
        mData.resize(size);
        std::memcpy(mData.data(), data, size);
    }

    PushConstant::PushConstant(const void *data, uint32_t size,
                               vk::ShaderStageFlags stageFlags)
            : PushConstant(data, size, 0, stageFlags) {}

    PushConstant::PushConstant(const void *data, uint32_t size)
            : PushConstant(data, size, 0, vk::ShaderStageFlagBits::eVertex) {}

    PushConstant::PushConstant(std::vector<uint8_t> &&data,
                               uint32_t offset,
                               vk::ShaderStageFlags stageFlags)
            : mData(std::move(data)),
              mOffset(offset),
              mStageFlags(stageFlags) {}

    PushConstant::PushConstant(std::vector<uint8_t> &&data,
                               vk::ShaderStageFlags stageFlags)
            : PushConstant(std::move(data), 0, stageFlags) {}

    PushConstant::PushConstant(std::vector<uint8_t> &&data)
            : PushConstant(std::move(data), 0, vk::ShaderStageFlagBits::eVertex) {}

    PushConstant::~PushConstant() = default;

    PushConstant::PushConstant(const PushConstant &other) = default;
//            : mData(other.mData), mOffset(other.mOffset), mStageFlags(other.mStageFlags) {}

    PushConstant &PushConstant::operator=(const PushConstant &other) {
        if (this != &other) {
            mData = other.mData;
            mOffset = other.mOffset;
            mStageFlags = other.mStageFlags;
        }
        return *this;
    }

    PushConstant::PushConstant(PushConstant &&other) noexcept
            : mData(std::move(other.mData)), mOffset(other.mOffset), mStageFlags(other.mStageFlags) {}

    PushConstant &PushConstant::operator=(PushConstant &&other) noexcept {
        if (this != &other) {
            mData = std::move(other.mData);
            mOffset = other.mOffset;
            mStageFlags = other.mStageFlags;
        }
        return *this;
    }

    void PushConstant::update(const void *data, uint32_t size) {
        mData.resize(size);
        std::memcpy(mData.data(), data, size);
    }

    vk::ShaderStageFlags PushConstant::getStageFlags() const {
        return mStageFlags;
    }

    uint32_t PushConstant::getOffset() const {
        return mOffset;
    }

    uint32_t PushConstant::getSize() const {
        return mData.size();
    }

    const void *PushConstant::getData() const {
        return mData.data();
    }
} // vklite