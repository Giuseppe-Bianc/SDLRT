//
// Created by gbian on 11/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "SDLRT/rayTracing/primitives/Cone.hpp"

namespace qbRT {
    DISABLE_WARNINGS_PUSH(26446 26447 26482 28020)
    static inline constexpr glm::dvec3 normalVector{0.0, 0.0, 1.0};
    bool Cone::TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal,
                                glm::dvec3 &localColor) noexcept {  // Copy the ray and apply the backwards transform.
        Ray bckRay = m_transformMatrix.Apply(castRay, BCKTFORM);

        // Copy the m_lab vector from bckRay and normalize it.
        glm::dvec3 v = glm::normalize(bckRay.m_lab);

        // Get the start point of the line.
        glm::dvec3 p = bckRay.m_point1;

        // // Compute a, b and c.
        // const double a = std::pow(v[0], 2.0) + std::pow(v[1], 2.0) - std::pow(v[2], 2.0);
        // const double b = 2 * (p[0] * v[0] + p[1] * v[1] - p[2] * v[2]);
        // const double c = std::pow(p[0], 2.0) + std::pow(p[1], 2.0) - std::pow(p[2], 2.0);
        const double a = glm::length2(glm::dvec2(v.x, v.y)) - v.z * v.z;
        const double b = 2.0 * (p.x * v.x + p.y * v.y - p.z * v.z);
        const double c = glm::length2(glm::dvec2(p.x, p.y)) - p.z * p.z;
        const double ad = (2 * a);

        // Compute b^2 - 4ac.
        const double numSQRT = std::sqrt(std::pow(b, 2.0) - 4 * a * c);

        std::array<glm::dvec3, 3> poi{};
        std::array<double, 3> t{};
        bool t1Valid{}, t2Valid{}, t3Valid{};
        if(numSQRT > 0.0) {
            // Compute the values of t.
            t[0] = (-b + numSQRT) / ad;
            t[1] = (-b - numSQRT) / ad;

            // Compute the points of intersection.
            poi[0] = bckRay.m_point1 + (v * t[0]);
            poi[1] = bckRay.m_point1 + (v * t[1]);

            if((t[0] > 0.0) && (poi[0][2] > 0.0) && (poi[0][2] < 1.0)) {
                t1Valid = true;
            } else {
                t1Valid = false;
                t[0] = maxtrav;
            }

            if((t[1] > 0.0) && (poi[1][2] > 0.0) && (poi[1][2] < 1.0)) {
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

        // And test the end cap.
        if(CloseEnough(v[2], 0.0)) {
            t3Valid = false;
            t[2] = maxtrav;
        } else {
            // Compute values for t.
            t[2] = (bckRay.m_point1[2] - 1.0) / -v[2];

            // Compute points of intersection.
            poi[2] = bckRay.m_point1 + t[2] * v;

            // Check if these are valid.
            if((t[2] > 0.0) && (std::sqrt(std::pow(poi[2][0], 2.0) + std::pow(poi[2][1], 2.0)) < 1.0)) {
                t3Valid = true;
            } else {
                t3Valid = false;
                t[2] = maxtrav;
            }
        }

        // If no valid intersections found, then we can stop.
        if((!t1Valid) && (!t2Valid) && (!t3Valid)) { return false; }

        // Check for the smallest valid value of t.
        int minIndex = 0;
        double minValue = minValValue;
        for(int i = 0; i < 3; ++i) {
            if(t[i] < minValue) {
                minValue = t[i];
                minIndex = i;
            }
        }

        /* If minIndex is either 0 or 1, then we have a valid intersection
            with the cone itself. */
        glm::dvec3 validPOI = poi[minIndex];
        if(minIndex < 2) {
            // Transform the intersection point back into world coordinates.
            intPoint = m_transformMatrix.Apply(validPOI, qbRT::FWDTFORM);

            // Compute the local normal.
            glm::dvec3 orgNormal{};
            glm::dvec3 newNormal{};
            const glm::dvec3 globalOrigin = m_transformMatrix.Apply(localOrigin, qbRT::FWDTFORM);
            double tX = validPOI[0];
            double tY = validPOI[1];
            double tZ = -std::sqrt(pow(tX, 2.0) + pow(tY, 2.0));
            orgNormal[0] = tX;
            orgNormal[1] = tY;
            orgNormal[2] = tZ;
            newNormal = glm::normalize(m_transformMatrix.Apply(orgNormal, qbRT::FWDTFORM) - globalOrigin);
            localNormal = newNormal;

            // Return the base color.
            localColor = m_baseColor;
            // Compute and store the (u,v) coordinates.
            double x = validPOI[0];
            double y = validPOI[1];
            double z = validPOI[2];
            // double u = atan2(y, x) / C_D(PI);
            // double v = (z * 2.0) + 1.0;
            m_uvCoords = glm::dvec2(atan2(y, x) / C_D(PI), (z * 2.0) + 1.0);

            return true;
        } else {
            // Check the end cap.
            if(!CloseEnough(v[2], 0.0)) {
                // Check if we are inside the disk.
                if(std::sqrt(std::pow(validPOI[0], 2.0) + std::pow(validPOI[1], 2.0)) < 1.0) {
                    // Transform the intersection point back into world coordinates.
                    intPoint = m_transformMatrix.Apply(validPOI, qbRT::FWDTFORM);

                    // Compute the local normal.
                    const glm::dvec3 globalOrigin = m_transformMatrix.Apply(localOrigin, qbRT::FWDTFORM);
                    localNormal = glm::normalize(m_transformMatrix.Apply(normalVector, qbRT::FWDTFORM) - globalOrigin);

                    // Return the base color.
                    localColor = m_baseColor;
                    // double x = validPOI[0];
                    // double y = validPOI[1];
                    // double z = validPOI[2];
                    m_uvCoords = glm::dvec2(validPOI[0], validPOI[1]);

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
   // NOLINTEND(*-include-cleaner)