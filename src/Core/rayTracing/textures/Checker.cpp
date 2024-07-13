//
// Created by gbian on 13/07/2024.
//

#include "SDLRT/rayTracing/textures/Checker.hpp"

namespace qbRT::Texture {
    glm::dvec4 Checker::GetColor(const glm::dvec2 &uvCoords) {
        // Apply the local transform to the (u,v) coordinates.
        const glm::dvec2 newLoc = ApplyTransform(uvCoords);
        const double newU = newLoc[0];
        const double newV = newLoc[1];

        glm::dvec4 localColor{};

        if(const auto check = C_I(floor(newU)) + C_I(floor(newV)); (check % 2) == 0) {
            localColor = m_color1;
        } else {
            localColor = m_color2;
        }

        return localColor;
    }
    void Checker::SetColor(const glm::dvec4 &inputColor1, const glm::dvec4 &inputColor2) {
        m_color1 = inputColor1;
        m_color2 = inputColor2;
    }

}  // namespace qbRT::Texture
