//
// Created by gbian on 04/07/2024.
//

#include "SDLRT/rayTracing//PointLight.hpp"
DISABLE_WARNINGS_PUSH(26447)
namespace qbRT {
    PointLight::PointLight() noexcept {
        m_color = SDL_COLOR(255.0, 255.0, 255.0);
        m_intensity = 1.0;
    }
    bool PointLight::ComputeIllumination(const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
                                         [[maybe_unused]] const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                         [[maybe_unused]] const std::shared_ptr<ObjectBase> &currentObject, SDL_Color &color,
                                         double &intensity) noexcept {
        const auto lightDir = glm::normalize(m_location - intPoint);
        const glm::dvec3 startPoint = intPoint;
        double angle = std::acos(glm::dot(localNormal, lightDir));
        if(angle > HALF_PID) {
            color = m_color;
            intensity = 0.0;
            return false;
        } else {
            color = m_color;
            intensity = m_intensity * (1.0 - (angle / HALF_PID));
            return true;
        }
    }

}  // namespace qbRT

DISABLE_WARNINGS_POP()
