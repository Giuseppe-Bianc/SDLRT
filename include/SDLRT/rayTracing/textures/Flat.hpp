//
// Created by gbian on 13/07/2024.
//

#pragma once
#include "TextureBase.hpp"

namespace qbRT::Texture {

    class Flat : public TextureBase {
    public:
        Flat() = default;
        virtual ~Flat() override = default;
        virtual glm::dvec4 GetColor(const glm::dvec2 &uvCoords) override;

        void SetColor(const glm::dvec4 &inputColor);

    private:
        glm::dvec4 m_color{1.0, 0.0, 0.0, 1.0};
    };
}  // namespace qbRT::Texture
