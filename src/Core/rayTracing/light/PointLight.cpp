//
// Created by gbian on 04/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "SDLRT/rayTracing/light/PointLight.hpp"
DISABLE_WARNINGS_PUSH(26447)
namespace qbRT {
    PointLight::PointLight() noexcept {
        m_color = {MAX_COLOR_VALUE, MAX_COLOR_VALUE, MAX_COLOR_VALUE};
        m_intensity = 1.0;
    }
    bool PointLight::ComputeIllumination(const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
                                         const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                         const std::shared_ptr<ObjectBase> &currentObject, glm::dvec3 &color, double &intensity) noexcept {
        const glm::dvec3 lightDir = glm::normalize(m_location - intPoint);
        const glm::dvec3 startPoint = intPoint;
        const qbRT::Ray lightRay(startPoint, startPoint + lightDir);
        glm::dvec3 poi{};
        glm::dvec3 poiNormal{};
        glm::dvec3 poiColor{};
        bool validInt = false;
        for(const auto &sceneObject : objectList) {
            if(sceneObject != currentObject) { validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor); }

            // If we have an intersection, then there is no point checking further
            //     so we can break out of the loop. In other words, this object is
            //     blocking light from this light source. */
            if(validInt) break;
        }
        if(!validInt) {
            const double angle = std::acos(glm::dot(localNormal, lightDir));
            if(angle > HALF_PID) {
                color = m_color;
                intensity = 0.0;
                return false;
            } else {
                color = m_color;
                intensity = m_intensity * (1.0 - (angle / HALF_PID));
                return true;
            }
        } else {
            // Shadow, so no illumination.
            color = m_color;
            intensity = 0.0;
            return false;
        }
    }

}  // namespace qbRT

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner)
