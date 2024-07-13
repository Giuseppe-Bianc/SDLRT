//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner  *-easily-swappable-parameters)
#include "SDLRT/rayTracing/Scene.hpp"
#include "SDLRT/rayTracing/materials/SimpleRefractive.hpp"
#include "SDLRT/rayTracing/materials/simplematerial.hpp"
#include "SDLRT/rayTracing/textures/Checker.hpp"
#include "SDLRT/rayTracing/textures/Image.hpp"

static inline constexpr double max_value = 255.0;
static inline constexpr auto max_valueui8t = NC_UI8T(255.0);
static inline constexpr double offset = 9.0;
static inline constexpr double scale = 0.94605;

DISABLE_WARNINGS_PUSH(26447)

namespace qbRT {
    fs::path calculateRelativePathToSrcRes(const fs::path &executablePath, const fs::path &targetFile) {
        // Get the parent directory of the executable path
        fs::path parentDir = executablePath.parent_path();

        // Traverse up the directory tree until we find a directory containing "src"
        while(!fs::exists(parentDir / "src")) {
            parentDir = parentDir.parent_path();
            // Check if we reached the root directory and "src" was not found
            if(parentDir == parentDir.root_path()) {
                std::cerr << "Error: 'src' directory not found in the path." << std::endl;
                return {};  // Return an empty path or handle error as needed
            }
        }

        // Move up one more level to reach the parent directory of "src"
        parentDir = parentDir.parent_path();
        auto resp = fs::path("src") / "Core" / "res";
        // Construct the relative path to the target file
        auto relativePathToTarget = parentDir / resp / targetFile;
        // Construct the path to the target file under "src/Core/res"

        // Calculate the relative path from the executable's directory
        auto relativePath = fs::relative(relativePathToTarget, executablePath);

        return relativePath.lexically_normal();
    }
    Scene::Scene() noexcept {
        // Configure the camera.
        vnd::Timer ctimer{"scene set camera"};
        // **************************************************************************************
        // Configure the camera.
        // **************************************************************************************
        m_camera.SetPosition({2.0, -5.0, 0.25});
        m_camera.SetLookAt({0.0, 0.0, 0.0});
        m_camera.SetUp({0.0, 0.0, 1.0});
        m_camera.SetHorzSize(1.0);
        m_camera.SetAspect(16.0 / 9.0);
        m_camera.UpdateCameraGeometry();
        LINFO("{}", ctimer);
        const vnd::AutoTimer timer{"scene set word data"};

        // **************************************************************************************
        // Setup ambient lightling.
        // **************************************************************************************
        qbRT::MaterialBase::m_ambientColor = {1.0, 1.0, 1.0};
        qbRT::MaterialBase::m_ambientIntensity = 0.2;

        // **************************************************************************************
        // Create some textures.
        // **************************************************************************************
        auto floorTexture = MAKE_SHARED(qbRT::Texture::Checker, qbRT::Texture::Checker());
        auto imageTexture = MAKE_SHARED(qbRT::Texture::Image, qbRT::Texture::Image());

        // **************************************************************************************
        // Setup the textures.
        // **************************************************************************************
        floorTexture->SetTransform({0.0, 0.0}, 0.0, {16.0, 16.0});
        const auto currentPath = fs::current_path();
        auto testimage = calculateRelativePathToSrcRes(currentPath, fs::path("linux-logo.bmp"));
        imageTexture->LoadImage(testimage.string());
        imageTexture->SetTransform({0.0, 0.0}, 0.0, {1.0, 1.0});

        // **************************************************************************************
        // Create some materials.
        // **************************************************************************************
        auto floorMaterial = MAKE_SHARED(qbRT::SimpleMaterial, qbRT::SimpleMaterial());
        auto imageMaterial = MAKE_SHARED(qbRT::SimpleMaterial, qbRT::SimpleMaterial());
        auto sphereMaterial = MAKE_SHARED(qbRT::SimpleMaterial, qbRT::SimpleMaterial());
        auto sphereMaterial2 = MAKE_SHARED(qbRT::SimpleMaterial, qbRT::SimpleMaterial());
        auto sphereMaterial3 = MAKE_SHARED(qbRT::SimpleMaterial, qbRT::SimpleMaterial());
        auto glassMaterial = MAKE_SHARED(qbRT::SimpleRefractive, qbRT::SimpleRefractive());

        // **************************************************************************************
        // Setup the materials.
        // **************************************************************************************
        floorMaterial->m_baseColor = {1.0, 1.0, 1.0};
        floorMaterial->m_reflectivity = 0.25;
        floorMaterial->m_shininess = 0.0;
        floorMaterial->AssignTexture(floorTexture);

        imageMaterial->m_baseColor = {1.0, 0.125, 0.125};
        imageMaterial->m_reflectivity = 0.0;
        imageMaterial->m_shininess = 0.0;
        imageMaterial->AssignTexture(imageTexture);

        sphereMaterial->m_baseColor = {1.0, 0.2, 0.2};
        sphereMaterial->m_reflectivity = 0.8;
        sphereMaterial->m_shininess = 32.0;

        sphereMaterial2->m_baseColor = {0.2, 1.0, 0.2};
        sphereMaterial2->m_reflectivity = 0.8;
        sphereMaterial2->m_shininess = 32.0;

        sphereMaterial3->m_baseColor = {0.2, 0.2, 1.0};
        sphereMaterial3->m_reflectivity = 0.8;
        sphereMaterial3->m_shininess = 32.0;

        glassMaterial->m_baseColor = {0.7, 0.7, 0.2};
        glassMaterial->m_reflectivity = 0.25;
        glassMaterial->m_shininess = 32.0;
        glassMaterial->m_translucency = 0.75;
        glassMaterial->m_ior = 1.333;

        // **************************************************************************************
        // Create and setup objects.
        // **************************************************************************************
        auto floor = MAKE_SHARED(qbRT::ObjPlane, qbRT::ObjPlane());
        floor->SetTransformMatrix(qbRT::GTform{{0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {16.0, 16.0, 1.0}});
        floor->AssignMaterial(floorMaterial);

        // **************************************************************************************
        auto imagePlane = MAKE_SHARED(qbRT::ObjPlane, qbRT::ObjPlane());
        imagePlane->SetTransformMatrix(qbRT::GTform{{0.0, 5.0, -0.75}, {-PI / 2.0, 0.0, 0.0}, {1.75, 1.75, 1.0}});
        imagePlane->AssignMaterial(imageMaterial);

        // **************************************************************************************
        auto sphere = MAKE_SHARED(qbRT::ObjSphere, qbRT::ObjSphere());
        sphere->SetTransformMatrix(qbRT::GTform{{-2.0, -2.0, 0.25}, {0.0, 0.0, 0.0}, {0.75, 0.75, 0.75}});
        sphere->AssignMaterial(sphereMaterial);

        // **************************************************************************************
        auto sphere2 = MAKE_SHARED(qbRT::ObjSphere, qbRT::ObjSphere());
        sphere2->SetTransformMatrix(qbRT::GTform{{-2.0, -0.5, 0.25}, {0.0, 0.0, 0.0}, {0.75, 0.75, 0.75}});
        sphere2->AssignMaterial(sphereMaterial2);

        // **************************************************************************************
        auto sphere3 = MAKE_SHARED(qbRT::ObjSphere, qbRT::ObjSphere());
        sphere3->SetTransformMatrix(qbRT::GTform{{-2.0, -1.25, -1.0}, {0.0, 0.0, 0.0}, {0.75, 0.75, 0.75}});
        sphere3->AssignMaterial(sphereMaterial3);

        // **************************************************************************************
        auto sphere4 = MAKE_SHARED(qbRT::ObjSphere, qbRT::ObjSphere());
        sphere4->SetTransformMatrix(qbRT::GTform{{2.0, -1.25, 0.25}, {0.0, 0.0, 0.0}, {0.75, 0.75, 0.75}});
        sphere4->AssignMaterial(glassMaterial);

        // **************************************************************************************
        // Put the objects into the scene.
        // **************************************************************************************
        m_objectList.emplace_back(floor);
        m_objectList.emplace_back(imagePlane);
        m_objectList.emplace_back(sphere);
        m_objectList.emplace_back(sphere2);
        m_objectList.emplace_back(sphere3);
        m_objectList.emplace_back(sphere4);

        // **************************************************************************************
        // Construct and setup the lights.
        // **************************************************************************************
        m_lightList.emplace_back(MAKE_SHARED(qbRT::PointLight, qbRT::PointLight()));
        m_lightList.at(0)->m_location = {3.0, -10.0, -5.0};
        m_lightList.at(0)->m_color = {1.0, 1.0, 1.0};
        m_lightList.at(0)->m_intensity = 4.0;

        m_lightList.emplace_back(MAKE_SHARED(qbRT::PointLight, qbRT::PointLight()));
        m_lightList.at(1)->m_location = {0.0, -10.0, -5.0};
        m_lightList.at(1)->m_color = {1.0, 1.0, 1.0};
        m_lightList.at(1)->m_intensity = 2.0;
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
                    // Check if the object has a material.
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