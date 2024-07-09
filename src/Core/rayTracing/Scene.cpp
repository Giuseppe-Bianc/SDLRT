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
        vnd::Timer ctimer{"scene set camera"};
        m_camera.SetPosition({0.0, -10.0, -2.0});
        m_camera.SetLookAt({0.0, 0.0, 0.0});
        m_camera.SetUp({0.0, 0.0, 1.0});
        m_camera.SetHorzSize(0.25);
        m_camera.SetAspect(16.0 / 9.0);
        m_camera.UpdateCameraGeometry();
        LINFO("{}", ctimer);
        const vnd::AutoTimer timer{"scene set word data"};  // Construct a test sphere.
        // Construct a test plane and spheres.
        m_objectList.emplace_back(MAKE_SHARED(ObjSphere, ObjSphere()));
        m_objectList.emplace_back(MAKE_SHARED(ObjSphere, ObjSphere()));
        m_objectList.emplace_back(MAKE_SHARED(ObjSphere, ObjSphere()));
        m_objectList.emplace_back(MAKE_SHARED(ObjPlane, ObjPlane()));

        // Define a transform for the plane, .
        GTform planeMatrix, testMatrix1, testMatrix2, testMatrix3;
        testMatrix1.SetTransform({-1.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.75});

        testMatrix2.SetTransform({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.75, 0.5, 0.5});

        testMatrix3.SetTransform({1.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.75, 0.75, 0.75});

        planeMatrix.SetTransform({0.0, 0.0, 0.75}, {0.0, 0.0, 0.0}, {4.0, 4.0, 1.0});
        m_objectList.at(0)->SetTransformMatrix(testMatrix1);
        m_objectList.at(1)->SetTransformMatrix(testMatrix2);
        m_objectList.at(2)->SetTransformMatrix(testMatrix3);
        m_objectList.at(3)->SetTransformMatrix(planeMatrix);

        m_objectList.at(0)->m_baseColor = {0.25, 0.5, 0.8};
        m_objectList.at(1)->m_baseColor = {1.0, 0.5, 0.0};
        m_objectList.at(2)->m_baseColor = {1.0, 0.8, 0.0};
        m_objectList.at(3)->m_baseColor = {0.5, 0.5, 0.5};

        // Construct a test light.
        m_lightList.emplace_back(MAKE_SHARED(PointLight, PointLight()));
        m_lightList.at(0)->m_location = {5.0, -10.0, -5.0};
        m_lightList.at(0)->m_color = {0.0, 0.0, 1.0};

        m_lightList.emplace_back(MAKE_SHARED(PointLight, PointLight()));
        m_lightList.at(1)->m_location = {-5.0, -10.0, -5.0};
        m_lightList.at(1)->m_color = {1.0, 0.0, 0.0};

        m_lightList.emplace_back(MAKE_SHARED(PointLight, PointLight()));
        m_lightList.at(2)->m_location = {0.0, -10.0, -5.0};
        m_lightList.at(2)->m_color = {0.0, 1.0, 0.0};
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
        glm::dvec3 localColor = {0.0, 0.0, 0.0};
        glm::dvec3 color = {0.0, 0.0, 0.0};
        const double xFact = 1.0 / halfXSise;
        const double yFact = 1.0 / halfYSise;
        [[maybe_unused]] double intensity = 0.0;
        long double minDist2 = 1e6;
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

                std::shared_ptr<ObjectBase> closestObject;
                glm::dvec3 closestIntPoint{};
                glm::dvec3 closestLocalNormal{};
                glm::dvec3 closestLocalColor{};
                long double minDist = 1e6L;
                bool intersectionFound = false;

                for(const auto &currentObject : m_objectList) {
                    validInt = currentObject->TestIntersection(cameraRay, intPoint, localNormal, localColor);
                    // If we have a valid intersection, change pixel color to red.
                    if(validInt) {
                        intersectionFound = true;
                        dist = glm::distance(intPoint, cameraRay.m_point1);
                        updateDistances(dist, maxDist, minDist2);
                        if(dist < minDist) {
                            minDist = dist;
                            closestObject = currentObject;
                            closestIntPoint = intPoint;
                            closestLocalNormal = localNormal;
                            closestLocalColor = localColor;
                        }
                    }
                }
                if(intersectionFound) {
                    // Compute the intensity of illuminatio
                    double red = 0.0;
                    double green = 0.0;
                    double blue = 0.0;
                    bool validIllum = false;
                    bool illumFound = false;
                    for(auto currentLight : m_lightList) {
                        validIllum = currentLight->ComputeIllumination(closestIntPoint, closestLocalNormal, m_objectList, closestObject,
                                                                       color, intensity);

                        if(validIllum) {
                            illumFound = true;
                            red += color[0] * intensity;
                            green += color[1] * intensity;
                            blue += color[2] * intensity;
                        }
                    }

                    if(illumFound) {
                        red *= closestLocalColor[0];
                        green *= closestLocalColor[1];
                        blue *= closestLocalColor[2];
                        outputImage.SetPixel(x, y, red, green, blue);
                    }
                }
            }
        }
        LINFO("{}", timer);

        // And display to the terminal.
        LINFO("Minimum distance: {}", minDist2);
        LINFO("Maximum distance: {}", maxDist);

        return true;
    }
}  // namespace qbRT
   // NOLINTEND(*-include-cleaner)

DISABLE_WARNINGS_POP()
