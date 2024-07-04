//
// Created by gbian on 03/07/2024.
//

// NOLINTBEGIN(*-include-cleaner)
#include "SDLRT/rayTracing/ObjSphere.hpp"

namespace qbRT {
    constexpr double aa = 1.0;

    // NOLINTNEXTLINE(*-easily-swappable-parameters)
    bool ObjSphere::TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, [[maybe_unused]] glm::dvec3 &localNormal,
                                     [[maybe_unused]] SDL_Color &localColor) const noexcept {
        // Compute the values of a, b and c.
        glm::dvec3 vhat = castRay.m_lab;
        vhat = glm::normalize(vhat);

        /* Note that a is equal to the squared magnitude of the
            direction of the cast ray. As this will be a unit vector,
            we can conclude that the value of 'a' will always be 1. */

        // Calculate b.
        const double b = 2.0 * glm::dot(castRay.m_point1, vhat);

        // Calculate c.
        const double c = glm::dot(castRay.m_point1, castRay.m_point1) - 1.0;

        // Test whether we actually have an intersection.
        const double intTest = (b * b) - 4.0 * aa * c;

        if(intTest > 0.0) {
            const double numSQRT = std::sqrt(intTest);
            const double t1 = (-b + numSQRT) / (2.0 * aa);
            const double t2 = (-b - numSQRT) / (2.0 * aa);

            /* If either t1 or t2 are negative, then at least part of the object is
                behind the camera and so we will ignore it. */
            if((t1 < 0.0) || (t2 < 0.0)) {
                return false;
            } else {
                // Determine which point of intersection was closest to the camera.
                if(t1 < t2) {
                    intPoint = castRay.m_point1 + (vhat * t1);
                } else {
                    intPoint = castRay.m_point1 + (vhat * t2);
                }
            }

            return true;
        } else {
            return false;
        }
    }

}  // namespace qbRT
   // NOLINTEND(*-include-cleaner)
