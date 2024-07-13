//
// Created by gbian on 13/07/2024.
//

#include "SDLRT/rayTracing/textures/TextureBase.hpp"

namespace qbRT::Texture {
    glm::dvec4 TextureBase::GetColor([[maybe_unused]] const glm::dvec2 &uvCoords) { return glm::dvec4{0.0, 0.0, 0.0, 0.0}; }
    void TextureBase::SetTransform(const glm::dvec2 &translation, const double &rotation, const glm::dvec2 &scale) {
        // Combine the rotation matrices
        // glm::dmat3 rotationMatrix = {cos(rotation), -sin(rotation), 0.0, sin(rotation), cos(rotation), 0.0, 0.0, 0.0, 1.0};
        const auto rotationMatrix = glm::dmat3(glm::rotate(glm::dmat4(1.0), rotation, glm::dvec3(0.0, 0.0, 1.0)));

        // glm::dmat3 scaleMatrix = {scale[0], 0.0, 0.0, 0.0, scale[1], 0.0, 0.0, 0.0, 1.0};
        const auto scaleMatrix = glm::dmat3(glm::scale(glm::dmat4(1.0), glm::dvec3(scale, 0.0)));

        // glm::dmat3 translationMatrix = {1.0, 0.0, translation[0], 0.0, 1.0, translation[1], 0.0, 0.0, 1.0};
        const auto translationMatrix = glm::dmat3(glm::translate(glm::dmat4(1.0), glm::dvec3(translation, 0.0)));
        // And combine to form the final transform matrix.
        m_transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    }
    glm::dvec3 TextureBase::BlendColors([[maybe_unused]] const std::vector<glm::bvec3> &inputColorList) {
        return glm::dvec3{0.0, 0.0, 0.0};
    }

    glm::dvec2 TextureBase::ApplyTransform(const glm::dvec2 &inputVector) {
        auto res = m_transformMatrix * glm::dvec3{inputVector, 0.0};
        return glm::dvec2{res};
    }

}  // namespace qbRT::Texture
