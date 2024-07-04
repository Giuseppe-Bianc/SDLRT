//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "SDLRT/rayTracing/Scene.hpp"

namespace qbRT {

    Scene::Scene() {
        // Configure the camera.
        /*m_camera.SetPosition({0.0, -10.0, 0.0});
        m_camera.SetLookAt({0.0, 0.0, 0.0});
        m_camera.SetUp({0.0, 0.0, 1.0});*/
        m_camera.SetHorzSize(0.25);
        m_camera.SetAspect(16.0 / 9.0);
        m_camera.UpdateCameraGeometry();
        // Get the screen centre and U,V vectors and display.
    }
    bool Scene::Render(qbImage &outputImage) {
        // Get the dimensions of the output image.
        SDL_Color color{0, 0, 0, 255};
        const auto xSize = outputImage.GetXSize();
        const auto ySize = outputImage.GetYSize();
        const auto halfXSise = C_D(xSize) / 2.0;
        const auto halfYSise = C_D(ySize) / 2.0;

        // Loop over each pixel in our image.
        Ray cameraRay;
        glm::dvec3 intPoint{0.0};
        glm::dvec3 localNormal{0.0};
        SDL_Color localColor{0, 0, 0, 0};
        const double xFact = 1.0 / halfXSise;
        const double yFact = 1.0 / halfYSise;
        double minDist = 1e6;
        double maxDist = 0.0;
        double normX = 0;
        double normY = 0;
        double dist = 0;
        vnd::Timer timer{"scene.Render"};
        bool validInt{};
        for(auto x : std::views::iota(0, xSize)) {
            normX = (static_cast<double>(x) * xFact) - 1.0;
            for(auto y : std::views::iota(0, ySize)) {
                // Normalize the x and y coordinates.
                normY = (C_D(y) * yFact) - 1.0;

                // Generate the ray for this pixel.
                m_camera.GenerateRay(normX, normY, cameraRay);

                // Test if we have a valid intersection.
                validInt = m_testSphere.TestIntersection(cameraRay, intPoint, localNormal, localColor);

                // If we have a valid intersection, change pixel color to red.
                if(validInt) {
                    dist = glm::distance(intPoint, cameraRay.GetPoint1());
                    if(dist > maxDist) maxDist = dist;

                    if(dist < minDist) minDist = dist;

                    color = SDL_COLOR(255.0 - ((dist - 9.0) / 0.94605) * 255.0, 0.0, 0.0, 255.0);
                    outputImage.SetPixel(x, y, color);
                } else {
                    color = {0, 0, 0, 255};
                    outputImage.SetPixel(x, y, color);
                }
            }
        }
        LINFO("{}", timer);

        auto screenCentre = m_camera.GetScreenCentre();
        auto screenU = m_camera.GetU();
        auto screenV = m_camera.GetV();

        // And display to the terminal.
        LINFO("\nCamera screen centre: {}\nCamera U vector: {}\nCamera V vector:{}", screenCentre, screenU, screenV);
        LINFO("\nMinimum distance: {}\nMaximum distance: {}", minDist, maxDist);

        return true;
    }
}  // namespace qbRT
   // NOLINTEND(*-include-cleaner)