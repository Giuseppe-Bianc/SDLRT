//
// Created by gbian on 11/07/2024.
//

#pragma once
#include "ObjectBase.hpp"

namespace qbRT {

    class Cone : public ObjectBase {
    public:
        Cone() = default;
        virtual ~Cone() override = default;
        virtual bool TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal,
                                      glm::dvec3 &localColor) const noexcept override;
    };

}  // namespace qbRT
