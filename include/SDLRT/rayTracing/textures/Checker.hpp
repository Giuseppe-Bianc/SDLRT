//
// Created by gbian on 13/07/2024.
//

#pragma once
#include "TextureBase.hpp"

namespace qbRT::Texture {

    class Checker : public TextureBase {
    public:
        Checker() = default;
        virtual ~Checker() override = default;
        virtual glm::dvec4 GetColor(const glm::dvec2 &uvCoords) override;
        void SetColor(const glm::dvec4 &inputColor1, const glm::dvec4 &inputColor2);

    private:
        glm::dvec4 m_color1{1.0, 1.0, 1.0, 1.0};
        glm::dvec4 m_color2{0.2, 0.2, 0.2, 1.0};
    };

}  // namespace qbRT::Texture
