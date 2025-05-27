//
// Created by leixing on 2025/5/27.
//

#include "QueueFamilyConfigure.h"

namespace vklite {

    QueueFamilyConfigure::QueueFamilyConfigure()
            : mQueueFamilyIndex(0), mPriorities{} {};

    QueueFamilyConfigure::~QueueFamilyConfigure() = default;

    QueueFamilyConfigure::QueueFamilyConfigure(const QueueFamilyConfigure &other) = default;

    QueueFamilyConfigure &QueueFamilyConfigure::operator=(const QueueFamilyConfigure &other) {
        if (this != &other) {
            mQueueFamilyIndex = other.mQueueFamilyIndex;
            mPriorities = other.mPriorities;
        }
        return *this;
    }

    QueueFamilyConfigure::QueueFamilyConfigure(QueueFamilyConfigure &&other) noexcept
            : mQueueFamilyIndex(other.mQueueFamilyIndex),
              mPriorities(std::move(other.mPriorities)) {}

    QueueFamilyConfigure &QueueFamilyConfigure::operator=(QueueFamilyConfigure &&other) noexcept {
        if (this != &other) {
            mQueueFamilyIndex = other.mQueueFamilyIndex;
            mPriorities = std::move(other.mPriorities);
        }
        return *this;
    }

    [[nodiscard]]
    uint32_t QueueFamilyConfigure::getQueueFamilyIndex() const {
        return mQueueFamilyIndex;
    }

    [[nodiscard]]
    const std::vector<float> &QueueFamilyConfigure::getPriorities() const {
        return mPriorities;
    }

    QueueFamilyConfigure &QueueFamilyConfigure::familyIndex(uint32_t queueFamilyIndex) {
        mQueueFamilyIndex = queueFamilyIndex;
        return *this;
    }

    QueueFamilyConfigure &QueueFamilyConfigure::addQueue(float priority) {
        mPriorities.push_back(priority);
        return *this;
    }

} // vklite