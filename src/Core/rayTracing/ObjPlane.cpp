//
// Created by gbian on 06/07/2024.
//

#include "SDLRT/rayTracing/ObjPlane.hpp"

DISABLE_WARNINGS_PUSH(26447)

namespace qbRT {
    bool ObjPlane::TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal,
                                    SDL_Color &localColor) const noexcept {
        // Copy the ray and apply the backwards transform.
        const Ray bckRay = m_transformMatrix.Apply(castRay, BCKTFORM);

        // Copy the m_lab vector from bckRay and normalize it.
        auto k = glm::normalize(bckRay.m_lab);
        if(!CloseEnough(k[2], 0)) {
            auto t = bckRay.m_point1[2] / -k[2];
            if(t > 0.0) {
                auto u = bckRay.m_point1[0] + (k[0] * t);
                auto v = bckRay.m_point1[1] + (k[1] * t);
                if((std::abs(u) < 1.0) && (std::abs(v) < 1.0)) {
                    // Compute the point of intersection.
                    const glm::dvec3 poi = bckRay.m_point1 + t * k;

                    // Transform the intersection point back into world coordinates.
                    intPoint = m_transformMatrix.Apply(poi, FWDTFORM);

                    // Compute the local normal.
                    const glm::dvec3 localOrigin{0.0, 0.0, 0.0};
                    const glm::dvec3 normalVector{0.0, 0.0, -1.0};
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
