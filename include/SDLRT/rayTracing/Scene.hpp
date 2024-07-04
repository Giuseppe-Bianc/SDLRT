//
// Created by gbian on 03/07/2024.
//

#pragma once

#include "../timer/Timer.hpp"
#include "Camera.hpp"
#include "ObjSphere.hpp"
#include "qbImage.hpp"
namespace qbRT {

    class Scene {
    public:
        // The default constructor.
        Scene() noexcept;

        // Function to perform the rendering.
        bool Render(qbImage &outputImage);

        // Private functions.
    private:
        // The camera that we will use.
        Camera m_camera;

        // A unit sphere on the origin, for testing.
        ObjSphere m_testSphere;
    };

}  // namespace qbRT
