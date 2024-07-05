//
// Created by gbian on 04/07/2024.
//

#pragma once

#include "LightBase.hpp"

namespace qbRT {

    class PointLight : public LightBase {
    public:
        PointLight() noexcept;
        virtual ~PointLight() override = default;
        virtual bool ComputeIllumination(const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
                                         const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                         const std::shared_ptr<ObjectBase> &currentObject, SDL_Color &color,
                                         double &intensity) noexcept override;
    };

}  // namespace qbRT
