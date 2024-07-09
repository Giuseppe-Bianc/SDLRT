//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "../GTform.hpp"
#include "../Ray.hpp"

namespace qbRT {

    class ObjectBase {
    public:
        // Constructor and destructor.
        ObjectBase() = default;
        virtual ~ObjectBase() = default;

        // Function to test for intersections.
        virtual bool TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal,
                                      glm::dvec3 &localColor) const noexcept;
        void SetTransformMatrix(const GTform &transformMatrix) noexcept;
        // Function to test whether two floating-point numbers are close to being equal.
        bool CloseEnough(const double f1, const double f2) const noexcept;

        // Public member variables.
        // The base colour of the object.
        glm::dvec3 m_baseColor{};
        GTform m_transformMatrix;
    };

}  // namespace qbRT
   // NOLINTEND(*-include-cleaner)