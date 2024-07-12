//
// Created by gbian on 11/07/2024.
//

#include "SDLRT/rayTracing/primitives/Cylinder.hpp"

namespace qbRT {
    DISABLE_WARNINGS_PUSH(26446 26447 26482 28020)
    bool Cylinder::TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal,
                                    glm::dvec3 &localColor) const noexcept {
        // Copy the ray and apply the backwards transform.
        qbRT::Ray bckRay = m_transformMatrix.Apply(castRay, qbRT::BCKTFORM);

        // Copy the m_lab vector from bckRay and normalize it.
        glm::dvec3 v = glm::normalize(bckRay.m_lab);

        // Get the start point of the line.
        const glm::dvec3 p = bckRay.m_point1;

        // // Compute a, b and c.
        // double a = std::pow(v[0], 2.0) + std::pow(v[1], 2.0);
        // double b = 2.0 * (p[0] * v[0] + p[1] * v[1]);
        // double c = std::pow(p[0], 2.0) + std::pow(p[1], 2.0) - 1.0;

        const double a = glm::dot(v, v) - v.z * v.z;
        const double b = 2.0 * (glm::dot(p, v) - p.z * v.z);
        const double c = glm::dot(p, p) - p.z * p.z - 1.0;
        const double ad = (2 * a);
        // Compute b^2 - 4ac.
        double numSQRT = std::sqrt(std::pow(b, 2.0) - 4 * a * c);

        // Test for intersections.
        // First with the cylinder itself.
        std::array<glm::dvec3, 4> poi{};
        std::array<double, 4> t{};
        bool t1Valid{}, t2Valid{}, t3Valid{}, t4Valid{};
        if(numSQRT > 0.0) {
            // There was an intersection.
            // Compute the values for t.
            t[0] = (-b + numSQRT) / ad;
            t[1] = (-b - numSQRT) / ad;

            // Compute the points of intersection.
            poi[0] = bckRay.m_point1 + (v * t[0]);
            poi[1] = bckRay.m_point1 + (v * t[1]);

            // Check if any of these are valid.
            if((t[0] > 0.0) && (std::abs(poi[0][2]) < 1.0)) {
                t1Valid = true;
            } else {
                t1Valid = false;
                t[0] = maxtrav;
            }

            if((t[1] > 0.0) && (std::abs(poi[1][2]) < 1.0)) {
                t2Valid = true;
            } else {
                t2Valid = false;
                t[1] = maxtrav;
            }
        } else {
            t1Valid = false;
            t2Valid = false;
            t[0] = maxtrav;
            t[1] = maxtrav;
        }

        // And test the end caps.
        if(CloseEnough(v[2], 0.0)) {
            t3Valid = false;
            t4Valid = false;
            t[2] = maxtrav;
            t[3] = maxtrav;
        } else {
            // Compute the values of t.
            t[2] = (bckRay.m_point1[2] - 1.0) / -v[2];
            t[3] = (bckRay.m_point1[2] + 1.0) / -v[2];

            // Compute the points of intersection.
            poi[2] = bckRay.m_point1 + t[2] * v;
            poi[3] = bckRay.m_point1 + t[3] * v;

            // Check if these are valid.
            if((t[2] > 0.0) && (sqrt(std::pow(poi[2][0], 2.0) + std::pow(poi[2][1], 2.0)) < 1.0)) {
                t3Valid = true;
            } else {
                t3Valid = false;
                t[2] = maxtrav;
            }

            if((t[3] > 0.0) && (sqrt(std::pow(poi[3][0], 2.0) + std::pow(poi[3][1], 2.0)) < 1.0)) {
                t4Valid = true;
            } else {
                t4Valid = false;
                t[3] = maxtrav;
            }
        }

        // If no valid intersections found, the we can stop.
        if((!t1Valid) && (!t2Valid) && (!t3Valid) && (!t4Valid)) return false;

        // Check for the smallest valid value of t.
        int minIndex = 0;
        double minValue = minValValue;
        for(int i = 0; i < 4; ++i) {
            if(t[i] < minValue) {
                minValue = t[i];
                minIndex = i;
            }
        }

        /* If minIndex is either 0 or 1, then we have a valid intersection
            with the cylinder itself. */
        glm::dvec3 validPOI = poi[minIndex];
        if(minIndex < 2) {
            // Transform the intersection point back into world coordinates.
            intPoint = m_transformMatrix.Apply(validPOI, FWDTFORM);

            // Compute the local normal.
            glm::dvec3 orgNormal{};
            glm::dvec3 newNormal{};
            const glm::dvec3 globalOrigin = m_transformMatrix.Apply(localOrigin, FWDTFORM);
            orgNormal[0] = validPOI[0];
            orgNormal[1] = validPOI[1];
            orgNormal[2] = 0.0;
            newNormal = glm::normalize(m_transformMatrix.Apply(orgNormal, FWDTFORM) - globalOrigin);
            localNormal = newNormal;

            // Return the base color.
            localColor = m_baseColor;

            return true;
        } else {
            // Otherwise check the end caps.
            if(!CloseEnough(v[2], 0.0)) {
                // Check if we are inside the disk.
                if(sqrt(std::pow(validPOI[0], 2.0) + std::pow(validPOI[1], 2.0)) < 1.0) {
                    // Transform the intersection point back into world coordinates.
                    intPoint = m_transformMatrix.Apply(validPOI, qbRT::FWDTFORM);

                    // Compute the local normal.
                    const glm::dvec3 normalVector{0.0, 0.0, 0.0 + validPOI[2]};
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
    DISABLE_WARNINGS_POP()
}  // namespace qbRT
