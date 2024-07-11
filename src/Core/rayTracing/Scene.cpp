//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner  *-easily-swappable-parameters)
#include "SDLRT/rayTracing/Scene.hpp"
#include "SDLRT/rayTracing/materials/simplematerial.hpp"

static inline constexpr double max_value = 255.0;
static inline constexpr auto max_valueui8t = NC_UI8T(255.0);
static inline constexpr double offset = 9.0;
static inline constexpr double scale = 0.94605;

DISABLE_WARNINGS_PUSH(26447)

namespace qbRT {

    Scene::Scene() noexcept {
        // Create some materials.
        auto testMaterial = MAKE_SHARED(SimpleMaterial, SimpleMaterial());
        auto testMaterial2 = MAKE_SHARED(SimpleMaterial, SimpleMaterial());
        auto testMaterial3 = MAKE_SHARED(SimpleMaterial, SimpleMaterial());
        auto floorMaterial = MAKE_SHARED(SimpleMaterial, SimpleMaterial());

        // Setup the materials.
        testMaterial->m_baseColor = {0.25, 0.5, 0.8};
        testMaterial->m_reflectivity = 0.1;
        testMaterial->m_shininess = 10.0;

        testMaterial2->m_baseColor = {1.0, 0.5, 0.0};
        testMaterial2->m_reflectivity = 0.75;
        testMaterial2->m_shininess = 10.0;

        testMaterial3->m_baseColor = {1.0, 0.8, 0.0};
        testMaterial3->m_reflectivity = 0.25;
        testMaterial3->m_shininess = 10.0;

        floorMaterial->m_baseColor = {1.0, 1.0, 1.0};
        floorMaterial->m_reflectivity = 0.5;
        floorMaterial->m_shininess = 0.0;

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

        // Define a transform for the plane.
        GTform planeMatrix, testMatrix1, testMatrix2, testMatrix3;
        testMatrix1.SetTransform({-1.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.5});

        testMatrix2.SetTransform({0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.5});

        testMatrix3.SetTransform({1.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.5});

        planeMatrix.SetTransform({0.0, 0.0, 0.75}, {0.0, 0.0, 0.0}, {4.0, 4.0, 1.0});
        m_objectList.at(0)->SetTransformMatrix(testMatrix1);
        m_objectList.at(1)->SetTransformMatrix(testMatrix2);
        m_objectList.at(2)->SetTransformMatrix(testMatrix3);
        m_objectList.at(3)->SetTransformMatrix(planeMatrix);

        m_objectList.at(0)->m_baseColor = {0.25, 0.5, 0.8};
        m_objectList.at(1)->m_baseColor = {1.0, 0.5, 0.0};
        m_objectList.at(2)->m_baseColor = {1.0, 0.8, 0.0};
        m_objectList.at(3)->m_baseColor = {0.5, 0.5, 0.5};

        // Assign materials to objects.
        m_objectList.at(0)->AssignMaterial(testMaterial3);
        m_objectList.at(1)->AssignMaterial(testMaterial);
        m_objectList.at(2)->AssignMaterial(testMaterial2);
        m_objectList.at(3)->AssignMaterial(floorMaterial);

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
        // glm::dvec3 color = {0.0, 0.0, 0.0};
        const double xFact = 1.0 / halfXSise;
        const double yFact = 1.0 / halfYSise;
        double normX = 0;
        double normY = 0;
        bool intersectionFound{};
        vnd::Timer timer{"scene.Render"};
        for(const auto &y : yrange) {
            normY = (C_D(y) * yFact) - 1.0;
            const vnd::AutoTimer yreder{FORMAT("processing line {} of {}", y, ySize)};
            for(const auto &x : xrange) {
                // Normalize the x and y coordinates.
                normX = (C_D(x) * xFact) - 1.0;

                // Generate the ray for this pixel.
                m_camera.GenerateRay(normX, normY, cameraRay);

                std::shared_ptr<ObjectBase> closestObject;
                glm::dvec3 closestIntPoint{};
                glm::dvec3 closestLocalNormal{};
                glm::dvec3 closestLocalColor{};
                intersectionFound = CastRay(cameraRay, closestObject, closestIntPoint, closestLocalNormal, closestLocalColor);
                if(intersectionFound) {
                    if(closestObject->m_hasMaterial) {
                        MaterialBase::m_reflectionRayCount = 0;
                        glm::dvec3 color = closestObject->m_pMaterial->ComputeColor(m_objectList, m_lightList, closestObject,
                                                                                    closestIntPoint, closestLocalNormal, cameraRay);
                        outputImage.SetPixel(x, y, color[0], color[1], color[2]);
                    } else {
                        glm::dvec3 matColor = MaterialBase::ComputeDiffuseColor(m_objectList, m_lightList, closestObject, closestIntPoint,
                                                                                closestLocalNormal, closestObject->m_baseColor);
                        outputImage.SetPixel(x, y, matColor[0], matColor[1], matColor[2]);
                    }
                }
            }
        }
        LINFO("{}", timer);
        return true;
    }
    bool Scene::CastRay(const Ray &castRay, std::shared_ptr<ObjectBase> &closestObject, glm::dvec3 &closestIntPoint,
                        glm::dvec3 &closestLocalNormal, glm::dvec3 &closestLocalColor) const {
        glm::dvec3 intPoint{};
        glm::dvec3 localNormal{};
        glm::dvec3 localColor{};
        double minDist = MAXDBL;
        double dist{};
        bool validInt{};
        bool intersectionFound = false;

        for(const auto &currentObject : m_objectList) {
            validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);
            // If we have a valid intersection, change pixel color to red.
            if(validInt) {
                intersectionFound = true;
                dist = glm::distance(intPoint, castRay.m_point1);
                if(dist < minDist) {
                    minDist = dist;
                    closestObject = currentObject;
                    closestIntPoint = intPoint;
                    closestLocalNormal = localNormal;
                    closestLocalColor = localColor;
                }
            }
        }
        return intersectionFound;
    }
}  // namespace qbRT

DISABLE_WARNINGS_POP()

// NOLINTEND(*-include-cleaner  *-easily-swappable-parameters)
