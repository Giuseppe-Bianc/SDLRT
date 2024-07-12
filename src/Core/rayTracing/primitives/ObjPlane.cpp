//
// Created by gbian on 06/07/2024.
//

#include "SDLRT/rayTracing/primitives/ObjPlane.hpp"

DISABLE_WARNINGS_PUSH(26447)

namespace qbRT {
    static inline constexpr glm::dvec3 normalVector{0.0, 0.0, -1.0};
    bool ObjPlane::TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal,
                                    glm::dvec3 &localColor) const noexcept {
        // Copy the ray and apply the backwards transform.
        Ray bckRay = m_transformMatrix.Apply(castRay, BCKTFORM);

        // Copy the m_lab vector from bckRay and normalize it.
        auto k = glm::normalize(bckRay.m_lab);

        /* Check if there is an intersection, ie. if the castRay is not parallel
            to the plane. */
        if(!CloseEnough(k[2], 0.0)) {
            // There is an intersection.
            const double t = bckRay.m_point1[2] / -k[2];

            /* If t is negative, then the intersection point must be behind
                the camera and we can ignore it. */
            if(t > 0.0) {
                // Compute the values for u and v.
                const double u = bckRay.m_point1[0] + (k[0] * t);
                const double v = bckRay.m_point1[1] + (k[1] * t);

                /* If the magnitude of both u and v is less than or equal to one
                    then we must be in the plane. */
                if((abs(u) < 1.0) && (abs(v) < 1.0)) {
                    // Compute the point of intersection.
                    const glm::dvec3 poi = bckRay.m_point1 + t * k;

                    // Transform the intersection point back into world coordinates.
                    intPoint = m_transformMatrix.Apply(poi, FWDTFORM);

                    // Compute the local normal.
                    // const glm::dvec3 localOrigin{0.0, 0.0, 0.0};
                    // const glm::dvec3 normalVector{0.0, 0.0, -1.0};
                    const glm::dvec3 globalOrigin = m_transformMatrix.Apply(localOrigin, FWDTFORM);
                    localNormal = glm::normalize(m_transformMatrix.Apply(normalVector, FWDTFORM) - globalOrigin);

                    // Return the base color.
                    localColor = m_baseColor;

                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }

        return false;
    }
}  // namespace qbRT

DISABLE_WARNINGS_POP()
