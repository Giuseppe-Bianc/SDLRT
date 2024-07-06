//
// Created by gbian on 03/07/2024.
//

// NOLINTBEGIN(*-include-cleaner *-identifier-length))
#include "SDLRT/rayTracing/ObjSphere.hpp"

DISABLE_WARNINGS_PUSH(26447)

namespace qbRT {
    static inline constexpr double aa = 1.0;
    static inline constexpr double aad = aa * 2;
    static inline constexpr double aaq = 4 * aa;

    // NOLINTNEXTLINE(*-easily-swappable-parameters)
    bool ObjSphere::TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal,
                                     [[maybe_unused]] SDL_Color &localColor) const noexcept {
        const Ray bckRay = m_transformMatrix.Apply(castRay, BCKTFORM);
        // Compute the values of a, b and c.
        const glm::dvec3 vhat = glm::normalize(bckRay.m_lab);

        // Note that a is equal to the squared magnitude of the
        //    direction of the cast ray. As this will be a unit vector,
        //    we can conclude that the value of 'a' will always be 1.

        // Calculate b.
        const double b = 2.0 * glm::dot(bckRay.m_point1, vhat);

        // Calculate c.
        const double c = glm::dot(bckRay.m_point1, bckRay.m_point1) - 1.0;

        // Test whether we actually have an intersection.
        const double intTest = (b * b) - aaq * c;
        glm::dvec3 poi{};

        if(intTest > 0.0) {
            const double numSQRT = std::sqrt(intTest);
            const double t1 = (-b + numSQRT) / aad;
            const double t2 = (-b - numSQRT) / aad;

            // If either t1 or t2 are negative, then at least part of the object is
            //    behind the camera and so we will ignore it.
            if((t1 < 0.0) || (t2 < 0.0)) {
                return false;
            } else {
                // Determine which point of intersection was closest to the camera.
                /*if(t1 < t2) {
                    intPoint = castRay.m_point1 + (vhat * t1);
                } else {
                    intPoint = castRay.m_point1 + (vhat * t2);
                }*/
                // Calculate the intersection point based on the smaller t value
                const double tMin = (t1 < t2) ? t1 : t2;
                poi = bckRay.m_point1 + (vhat * tMin);
            }
            intPoint = m_transformMatrix.Apply(poi, FWDTFORM);
            const glm::dvec3 newObjOrigin = m_transformMatrix.Apply(glm::dvec3{0.0, 0.0, 0.0}, qbRT::FWDTFORM);
            localNormal = glm::normalize(intPoint - newObjOrigin);
            localColor = m_baseColor;
            return true;
        } else {
            // localColor = SDL_COLOR(0.0,0.0,0.0);
            return false;
        }
    }
}  // namespace qbRT

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner *-identifier-length))
