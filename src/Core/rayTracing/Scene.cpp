//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "SDLRT/rayTracing/Scene.hpp"

static inline constexpr double max_value = 255.0;
static inline constexpr auto max_valueui8t = NC_UI8T(255.0);
static inline constexpr double offset = 9.0;
static inline constexpr double scale = 0.94605;

DISABLE_WARNINGS_PUSH(26447)

namespace qbRT {

    Scene::Scene() noexcept {
        // Configure the camera.
        m_camera.SetPosition({0.0, -10.0, 0.0});
        m_camera.SetLookAt({0.0, 0.0, 0.0});
        m_camera.SetUp({0.0, 0.0, 1.0});
        m_camera.SetHorzSize(0.25);
        m_camera.SetAspect(16.0 / 9.0);
        m_camera.UpdateCameraGeometry();
        // Get the screen centre and U,V vectors and display.
        m_objectList.emplace_back(MAKE_SHARED(ObjSphere, ObjSphere{}));
        // Construct a test sphere.
        m_objectList.emplace_back(MAKE_SHARED(ObjSphere, ObjSphere{}));
        m_objectList.emplace_back(MAKE_SHARED(ObjSphere, ObjSphere{}));
        // m_objectList.push_back(std::make_shared<qbRT::ObjSphere>(qbRT::ObjSphere()));

        // Modify the spheres.
        GTform testMatrix1, testMatrix2, testMatrix3;
        testMatrix1.SetTransform({-1.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.75});

        testMatrix2.SetTransform({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.75, 0.5, 0.5});

        testMatrix3.SetTransform({1.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.65, 0.65, 0.65});

        m_objectList.at(0)->SetTransformMatrix(testMatrix1);
        m_objectList.at(1)->SetTransformMatrix(testMatrix2);
        m_objectList.at(2)->SetTransformMatrix(testMatrix3);
        m_objectList.at(0)->m_baseColor = SDL_COLOR(64.0, 128.0, 200.0);
        m_objectList.at(1)->m_baseColor = SDL_COLOR(255.0, 128.0, 0.0);
        m_objectList.at(2)->m_baseColor = SDL_COLOR(255.0, 200.0, 0.0);
        m_lightList.push_back(MAKE_SHARED(PointLight, PointLight()));
        m_lightList.at(0)->m_location = {5.0, -10.0, 5.0};
        m_lightList.at(0)->m_color = SDL_COLOR(255.0, 255.0, 255.0);
    }
    void updateDistances(long double dist, long double &maxDist, long double &minDist) noexcept {
        maxDist = std::max(maxDist, dist);
        minDist = std::min(minDist, dist);
    }
    bool Scene::Render(qbImage &outputImage) const {
        // Get the dimensions of the output image.
        vnd::Timer timerd{"scene.Render::data"};
        const auto xSize = C_ST(outputImage.GetXSize());
        const auto ySize = C_ST(outputImage.GetYSize());
        const auto halfXSise = C_D(xSize) / 2.0;
        const auto halfYSise = C_D(ySize) / 2.0;
        const auto xrange = std::views::iota(C_ST(0), xSize);
        const auto yrange = std::views::iota(C_ST(0), ySize);
        LINFO("{}", timerd);

        // Loop over each pixel in our image.
        Ray cameraRay;
        glm::dvec3 intPoint{0.0};
        glm::dvec3 localNormal{0.0};
        SDL_Color localColor = SDL_COLOR(0.0, 0.0, 0.0);
        SDL_Color color = SDL_COLOR(0.0, 0.0, 0.0);
        const double xFact = 1.0 / halfXSise;
        const double yFact = 1.0 / halfYSise;
        double intensity = 0.0;
        long double minDist = 1e6;
        long double maxDist = 0.0;
        long double dist = 0;
        double normX = 0;
        double normY = 0;
        vnd::Timer timer{"scene.Render"};
        bool validInt{};
        for(const auto &x : xrange) {
            normX = (C_D(x) * xFact) - 1.0;
            for(const auto &y : yrange) {
                // Normalize the x and y coordinates.
                normY = (C_D(y) * yFact) - 1.0;

                // Generate the ray for this pixel.
                m_camera.GenerateRay(normX, normY, cameraRay);

                for(const auto &currentObject : m_objectList) {
                    validInt = currentObject->TestIntersection(cameraRay, intPoint, localNormal, localColor);
                    // If we have a valid intersection, change pixel color to red.
                    if(validInt) {
                        // Compute intensity of illumination.
                        bool validIllum = false;
                        for(const auto &currentLight : m_lightList) {
                            validIllum = currentLight->ComputeIllumination(intPoint, localNormal, m_objectList, currentObject, color,
                                                                           intensity);
                        }

                        // Compute the distance between the camera and the point of intersection.
                        dist = glm::distance(intPoint, cameraRay.m_point1);
                        updateDistances(dist, maxDist, minDist);

                        // outputImage.SetPixel(x, y, 255.0 - ((dist - 9.0) / 0.94605) * 255.0, 0.0, 0.0)
                        if(validIllum) {
                            outputImage.SetPixel(x, y, SDL_COLORM(localColor, intensity));
                        } else {
                            // outputImage.SetPixel(x, y, localColor.r * intensity, localColor.g * intensity, localColor.b * intensity);
                        }
                    } else {
                        // outputImage.SetPixel(x, y, SDL_COLOR(0.0, 0.0, 0.0));
                    }
                }
            }
        }
        LINFO("{}", timer);

        // And display to the terminal.
        LINFO("Minimum distance: {}", minDist);
        LINFO("Maximum distance: {}", maxDist);

        return true;
    }
}  // namespace qbRT
   // NOLINTEND(*-include-cleaner)

DISABLE_WARNINGS_POP()
