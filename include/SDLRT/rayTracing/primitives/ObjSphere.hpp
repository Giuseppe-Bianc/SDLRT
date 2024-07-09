//
// Created by gbian on 03/07/2024.
//

#pragma once
#include "ObjectBase.hpp"

namespace qbRT {

    class ObjSphere : public ObjectBase {
    public:
        ObjSphere() = default;
        virtual ~ObjSphere() override = default;
        bool TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal,
                              glm::dvec3 &localColor) const noexcept override;

    private:
    };

}  // namespace qbRT
