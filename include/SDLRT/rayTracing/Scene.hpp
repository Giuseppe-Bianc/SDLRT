//
// Created by gbian on 03/07/2024.
//

#pragma once

#include "qbImage.hpp"

namespace qbRT {

    class Scene {
    public:
        // The default constructor.
        Scene() = default;

        // Function to perform the rendering.
        bool Render(qbImage &outputImage);

        // Private functions.
    private:
        // Private members.
    private:
    };

}  // namespace qbRT
