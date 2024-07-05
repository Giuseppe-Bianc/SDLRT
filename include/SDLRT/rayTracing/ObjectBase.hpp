//
// Created by gbian on 03/07/2024.
//

#pragma once
#include "GTform.hpp"
#include "Ray.hpp"
#include <SDL3/SDL.h>

namespace qbRT {

    class ObjectBase {
    public:
        // Constructor and destructor.
        ObjectBase() = default;
        virtual ~ObjectBase() = default;

        // Function to test for intersections.
        virtual bool TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal,
                                      SDL_Color &localColor) const noexcept;
        void SetTransformMatrix(const GTform &transformMatrix);
        // Function to test whether two floating-point numbers are close to being equal.
        bool CloseEnough(const double f1, const double f2) noexcept;

        // Public member variables.
        // The base colour of the object.
        SDL_Color m_baseColor{};
        GTform  m_transformMatrix;;
    };

}  // namespace qbRT
