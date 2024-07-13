//
// Created by gbian on 13/07/2024.
//

#include "SDLRT/rayTracing/textures/Flat.hpp"

namespace qbRT::Texture {
    glm::dvec4 Flat::GetColor([[maybe_unused]] const glm::dvec2 &uvCoords) { return m_color; }
    void Flat::SetColor(const glm::dvec4 &inputColor) { m_color = inputColor; }

}  // namespace qbRT::Texture
