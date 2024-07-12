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
        // Configure the camera.
        vnd::Timer ctimer{"scene set camera"};
        m_camera.SetPosition({3.0, -5.0, -2.0});
        m_camera.SetLookAt({0.0, 0.0, 0.0});
        m_camera.SetUp({0.0, 0.0, 1.0});
        m_camera.SetHorzSize(0.75);
        m_camera.SetAspect(16.0 / 9.0);
        m_camera.UpdateCameraGeometry();
        LINFO("{}", ctimer);
        const vnd::AutoTimer timer{"scene set word data"};  // Construct a test sphere.
        // Construct a test plane and spheres.
        auto silverMetal = MAKE_SHARED(SimpleMaterial, SimpleMaterial());
        auto goldMetal = MAKE_SHARED(SimpleMaterial, SimpleMaterial());
        auto blueDiffuse = MAKE_SHARED(SimpleMaterial, SimpleMaterial());
        auto yellowDiffuse = MAKE_SHARED(SimpleMaterial, SimpleMaterial());
        auto orangeDiffuse = MAKE_SHARED(SimpleMaterial, SimpleMaterial());
        auto floorMaterial = MAKE_SHARED(SimpleMaterial, SimpleMaterial());
        auto wallMaterial = MAKE_SHARED(SimpleMaterial, SimpleMaterial());

        // Setup the materials.
        silverMetal->m_baseColor = {0.5, 0.5, 0.8};
        silverMetal->m_reflectivity = 0.5;
        silverMetal->m_shininess = 20.0;

        goldMetal->m_baseColor = {0.8, 0.8, 0.3};
        goldMetal->m_reflectivity = 0.25;
        goldMetal->m_shininess = 20.0;

        blueDiffuse->m_baseColor = {0.2, 0.2, 0.8};
        blueDiffuse->m_reflectivity = 0.05;
        blueDiffuse->m_shininess = 5.0;

        yellowDiffuse->m_baseColor = {0.8, 0.8, 0.2};
        yellowDiffuse->m_reflectivity = 0.05;
        yellowDiffuse->m_shininess = 5.0;

        orangeDiffuse->m_baseColor = {1.0, 0.5, 0.0};
        orangeDiffuse->m_reflectivity = 0.05;
        orangeDiffuse->m_shininess = 5.0;

        floorMaterial->m_baseColor = {1.0, 1.0, 1.0};
        floorMaterial->m_reflectivity = 0.0;
        floorMaterial->m_shininess = 0.0;

        wallMaterial->m_baseColor = {1.0, 0.125, 0.125};
        wallMaterial->m_reflectivity = 0.75;
        wallMaterial->m_shininess = 0.0;
        // Create and setup objects.
        auto cone = MAKE_SHARED(qbRT::Cone, qbRT::Cone());
        cone->SetTransformMatrix(qbRT::GTform{{0.0, 0.0, -0.5}, {0.0, 0.0, 0.0}, {1.0, 1.0, 2.0}});
        cone->AssignMaterial(silverMetal);

        auto leftSphere = MAKE_SHARED(qbRT::ObjSphere, qbRT::ObjSphere());
        leftSphere->SetTransformMatrix(qbRT::GTform{{1.5, -2.0, 0.5}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.5}});
        leftSphere->AssignMaterial(blueDiffuse);

        auto rightSphere = MAKE_SHARED(qbRT::ObjSphere, qbRT::ObjSphere());
        rightSphere->SetTransformMatrix(qbRT::GTform{{1.5, 0.0, 0.0}, {0.0, 0.0, 0.0}, {1.0, 1.0, 1.0}});
        rightSphere->AssignMaterial(yellowDiffuse);

        auto topSphere = MAKE_SHARED(qbRT::ObjSphere, qbRT::ObjSphere());
        topSphere->SetTransformMatrix(qbRT::GTform{{0.0, 0.0, -1.0}, {0.0, 0.0, 0.0}, {0.5, 0.5, 0.5}});
        topSphere->AssignMaterial(orangeDiffuse);

        auto floor = MAKE_SHARED(qbRT::ObjPlane, qbRT::ObjPlane());
        floor->SetTransformMatrix(qbRT::GTform{{0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {16.0, 16.0, 1.0}});
        floor->AssignMaterial(floorMaterial);

        auto leftWall = MAKE_SHARED(qbRT::ObjPlane, qbRT::ObjPlane());
        leftWall->SetTransformMatrix(qbRT::GTform{{-4.0, 0.0, 0.0}, {0.0, -PI / 2.0, -PI / 2.0}, {16.0, 16.0, 1.0}});
        leftWall->AssignMaterial(wallMaterial);

        auto backWall = MAKE_SHARED(qbRT::ObjPlane, qbRT::ObjPlane());
        backWall->SetTransformMatrix(qbRT::GTform{{0.0, 4.0, 0.0}, {-PI / 2.0, 0.0, 0.0}, {16.0, 16.0, 1.0}});
        backWall->AssignMaterial(wallMaterial);

        auto cylinder1 = MAKE_SHARED(qbRT::Cylinder, qbRT::Cylinder());
        cylinder1->SetTransformMatrix(qbRT::GTform{{-1.5, -2.0, 1.0}, {0.0, -PI / 2.0, 0.0}, {0.25, 0.25, 1.0}});
        cylinder1->AssignMaterial(goldMetal);

        auto cylinder2 = MAKE_SHARED(qbRT::Cylinder, qbRT::Cylinder());
        cylinder2->SetTransformMatrix(qbRT::GTform{{-1.0, -2.0, 0.0}, {0.0, 0.0, 0.0}, {0.25, 0.25, 1.0}});
        cylinder2->AssignMaterial(silverMetal);

        auto cone2 = MAKE_SHARED(qbRT::Cone, qbRT::Cone());
        cone2->SetTransformMatrix(qbRT::GTform{{0.0, -1.0, 0.0}, {PI / 4.0, 0.0, 0.0}, {0.5, 0.5, 1.0}});
        cone2->AssignMaterial(goldMetal);

        // Put the objects into the scene.
        m_objectList.emplace_back(cone);
        m_objectList.emplace_back(leftSphere);
        m_objectList.emplace_back(rightSphere);
        m_objectList.emplace_back(topSphere);
        m_objectList.emplace_back(floor);
        m_objectList.emplace_back(leftWall);
        m_objectList.emplace_back(backWall);
        m_objectList.emplace_back(cylinder1);
        m_objectList.emplace_back(cylinder2);
        m_objectList.emplace_back(cone2);

        m_objectList.emplace_back(floor);
        m_objectList.emplace_back(cylinder1);

        m_lightList.emplace_back(MAKE_SHARED(PointLight, PointLight()));
        m_lightList.at(0)->m_location = {3.0, -10.0, -5.0};
        m_lightList.at(0)->m_color = {1.0, 1.0, 1.0};

        m_lightList.emplace_back(MAKE_SHARED(PointLight, PointLight()));
        m_lightList.at(1)->m_location = {0.0, -10.0, -5.0};
        m_lightList.at(1)->m_color = {1.0, 1.0, 1.0};
        // Construct and setup the lights.
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
                        outputImage.SetPixel(x, y, color);
                    } else {
                        glm::dvec3 matColor = MaterialBase::ComputeDiffuseColor(m_objectList, m_lightList, closestObject, closestIntPoint,
                                                                                closestLocalNormal, closestObject->m_baseColor);
                        outputImage.SetPixel(x, y, matColor);
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
