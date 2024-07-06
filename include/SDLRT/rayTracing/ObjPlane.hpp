//
// Created by gbian on 06/07/2024.
//

#pragma once

#include "ObjectBase.hpp"

namespace qbRT {

    class ObjPlane : public ObjectBase {
    public:
        ObjPlane() = default;
        ~ObjPlane() override = default;
        bool TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal,
                              SDL_Color &localColor) const noexcept override;
    };

}  // namespace qbRT
