//
// Created by leixing on 2025/3/11.
//

#include "VertexConfigure.h"

namespace vklite {

    VertexConfigure::VertexConfigure() = default;

    VertexConfigure::~VertexConfigure() = default;

    VertexConfigure::VertexConfigure(const VertexConfigure &other) = default;

    VertexConfigure &VertexConfigure::operator=(const VertexConfigure &other) {
        if (this != &other) {
            mVertexBindingConfigures = other.mVertexBindingConfigures;
        }
        return *this;
    }

    VertexConfigure::VertexConfigure(VertexConfigure &&other) noexcept
            : mVertexBindingConfigures(std::move(other.mVertexBindingConfigures)) {}

    VertexConfigure &VertexConfigure::operator=(VertexConfigure &&other) noexcept {
        if (this != &other) {
            mVertexBindingConfigures = std::move(other.mVertexBindingConfigures);
        }
        return *this;
    }

    VertexConfigure &VertexConfigure::add(VertexBindingConfigure &&vertexConfigure) {
        mVertexBindingConfigures.emplace(vertexConfigure.getBinding(), std::move(vertexConfigure));
        return *this;
    }

    std::vector<vk::VertexInputBindingDescription> VertexConfigure::createVertexInputBindingDescriptions() const {
        std::vector<vk::VertexInputBindingDescription> vertexInputBindingDescriptions;

        vertexInputBindingDescriptions.reserve(mVertexBindingConfigures.size());
        for (const auto &[binding, vertexConfigure]: mVertexBindingConfigures) {
            vertexInputBindingDescriptions.emplace_back(
                    vk::VertexInputBindingDescription()
                            .setBinding(binding)
                            .setStride(vertexConfigure.getStride())
                            .setInputRate(vertexConfigure.getVertexInputRate())
            );
        }

        return vertexInputBindingDescriptions;
    }


    std::vector<vk::VertexInputAttributeDescription> VertexConfigure::createVertexInputAttributeDescriptions() const {

        std::vector<vk::VertexInputAttributeDescription> vertexInputAttributeDescriptions;

        for (const auto &bindingEntry: mVertexBindingConfigures) {
            uint32_t binding = bindingEntry.first;
            const VertexBindingConfigure &vertexBindingConfigure = bindingEntry.second;

            for (const auto &attributeEntry: vertexBindingConfigure.getAttributes()) {
                uint32_t location = attributeEntry.first;
                const VertexAttributeConfigure &attributeConfigure = attributeEntry.second;

                vk::VertexInputAttributeDescription attributeDescription{};
                attributeDescription
                        .setBinding(binding)
                        .setLocation(location)
                        .setFormat(attributeConfigure.getFormat())
                        .setOffset(attributeConfigure.getOffset());

                vertexInputAttributeDescriptions.push_back(attributeDescription);
            }
        }

        return vertexInputAttributeDescriptions;
    }

} // vklite