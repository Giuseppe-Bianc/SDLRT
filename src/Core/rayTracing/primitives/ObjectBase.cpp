//
// Created by gbian on 03/07/2024.
//

#include "SDLRT/rayTracing/primitives/ObjectBase.hpp"

static inline constexpr auto EPSILON = 1e-30;

namespace qbRT {
    bool ObjectBase::TestIntersection([[maybe_unused]] const Ray &castRay, [[maybe_unused]] glm::dvec3 &intPoint,
                                      [[maybe_unused]] glm::dvec3 &localNormal, [[maybe_unused]] glm::dvec3 &localColor) const noexcept {
        return false;
    }
    bool ObjectBase::AssignMaterial(const std::shared_ptr<MaterialBase> &objectMaterial) noexcept {
        m_pMaterial = objectMaterial;
        m_hasMaterial = true;
        return m_hasMaterial;
    }

    void ObjectBase::SetTransformMatrix(const GTform &transformMatrix) noexcept { m_transformMatrix = transformMatrix; }

    bool ObjectBase::CloseEnough(const double f1, const double f2) const noexcept { return std::abs(f1 - f2) < EPSILON; }
}  // namespace qbRT
