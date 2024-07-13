//
// Created by gbian on 13/07/2024.
//

#pragma once

#include "../Ray.hpp"

namespace qbRT::Texture {
    class TextureBase {
    public:
        TextureBase() = default;
        virtual ~TextureBase() = default;
        virtual glm::dvec4 GetColor(const glm::dvec2 &uvCoords);
        void SetTransform(const glm::dvec2 &translation, const double &rotation, const glm::dvec2 &scale);
        static glm::dvec3 BlendColors(const std::vector<glm::bvec3> &inputColorList);
        glm::dvec2 ApplyTransform(const glm::dvec2 &inputVector);

    private:
        glm::dmat3 m_transformMatrix = glm::dmat3(1.0);
    };

}  // namespace qbRT::Texture
