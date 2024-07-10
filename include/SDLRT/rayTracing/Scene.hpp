//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../timer/Timer.hpp"
#include "./light/PointLight.hpp"
#include "./primitives/ObjPlane.hpp"
#include "./primitives/ObjSphere.hpp"
#include "Camera.hpp"
#include "qbImage.hpp"
namespace qbRT {

    class Scene {
    public:
        // The default constructor.
        Scene() noexcept;

        // Function to perform the rendering.
        bool Render(qbImage &outputImage) const;

        bool CastRay(const Ray &castRay, std::shared_ptr<ObjectBase> &closestObject, glm::dvec3 &closestIntPoint,
                     glm::dvec3 &closestLocalNormal, glm::dvec3 &closestLocalColor) const;
        // Private functions.
    private:
        // The camera that we will use.
        Camera m_camera;

        // A unit sphere on the origin, for testing.
        std::vector<std::shared_ptr<ObjectBase>> m_objectList;
        std::vector<std::shared_ptr<LightBase>> m_lightList;
        long double minDist2 = 1e6;
        long double maxDist = 0.0;
    };

}  // namespace qbRT
// NOLINTEND(*-include-cleaner)
