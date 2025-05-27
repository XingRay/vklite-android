//
// Created by leixing on 2025/5/27.
//

#pragma once

#include <cstdint>
#include <vector>

namespace vklite {

    class QueueFamilyConfigure {
    private:
        uint32_t mQueueFamilyIndex;
        std::vector<float> mPriorities;

    public:

        QueueFamilyConfigure();

        ~QueueFamilyConfigure();

        QueueFamilyConfigure(const QueueFamilyConfigure &other);

        QueueFamilyConfigure &operator=(const QueueFamilyConfigure &other);

        QueueFamilyConfigure(QueueFamilyConfigure &&other) noexcept;

        QueueFamilyConfigure &operator=(QueueFamilyConfigure &&other) noexcept;

        [[nodiscard]]
        uint32_t getQueueFamilyIndex() const;

        [[nodiscard]]
        const std::vector<float> &getPriorities() const;

        QueueFamilyConfigure &familyIndex(uint32_t queueFamilyIndex);

        QueueFamilyConfigure &addQueue(float priority = 1.0f);
    };

} // vklite
