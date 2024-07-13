//
// Created by gbian on 03/07/2024.
//

// NOLINTBEGIN(*-include-cleaner *-identifier-length))
#include "SDLRT/rayTracing/primitives/ObjSphere.hpp"

DISABLE_WARNINGS_PUSH(26447)

namespace qbRT {
    static inline constexpr double aa = 1.0;
    static inline constexpr double aad = aa * 2;
    static inline constexpr double aaq = 4 * aa;
    static inline constexpr auto zvec = glm::dvec3{0.0, 0.0, 0.0};

    // NOLINTNEXTLINE(*-easily-swappable-parameters)
    bool ObjSphere::TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal, glm::dvec3 &localColor) noexcept {
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
                if(t1 < t2) {
                    if(t1 > 0.0) {
                        poi = bckRay.m_point1 + (vhat * t1);
                    } else {
                        if(t2 > 0.0) {
                            poi = bckRay.m_point1 + (vhat * t2);
                        } else {
                            return false;
                        }
                    }
                } else {
                    if(t2 > 0.0) {
                        poi = bckRay.m_point1 + (vhat * t2);
                    } else {
                        if(t1 > 0.0) {
                            poi = bckRay.m_point1 + (vhat * t1);
                        } else {
                            return false;
                        }
                    }
                }
            }
            intPoint = m_transformMatrix.Apply(poi, FWDTFORM);
            const glm::dvec3 newObjOrigin = m_transformMatrix.Apply(zvec, FWDTFORM);
            localNormal = glm::normalize(intPoint - newObjOrigin);
            localColor = m_baseColor;
            // Compute and store (u,v) coordinates for possible later use.
            double x = poi[0];
            double y = poi[1];
            double z = poi[2];
            double u = atan2(sqrt(pow(x, 2.0) + pow(y, 2.0)), z);
            double v = atan2(y, x);

            u /= PI;
            v /= PI;

            m_uvCoords = glm::dvec2(u, v);
            return true;
        } else {
            return false;
        }
    }
}  // namespace qbRT

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner *-identifier-length))
