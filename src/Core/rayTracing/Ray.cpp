//
// Created by gbian on 03/07/2024.
//

#include "SDLRT/rayTracing/Ray.hpp"

namespace qbRT {
    Ray::Ray(const glm::dvec3 &point1, const glm::dvec3 &point2) : m_point1(point1), m_point2(point2), m_lab(point2 - point2) {}

    glm::dvec3 Ray::GetPoint1() const { return m_point1; }
    glm::dvec3 Ray::GetPoint2() const { return m_point2; }
    glm::dvec3 Ray::GetLab() const { return m_lab; }

}  // namespace qbRT