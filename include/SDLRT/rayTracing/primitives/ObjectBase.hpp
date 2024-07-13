//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "../GTform.hpp"
#include "../Ray.hpp"

namespace qbRT {
    static inline constexpr double minValValue = 10e6;
    static inline constexpr glm::dvec3 localOrigin{0.0, 0.0, 0.0};
    static inline constexpr auto maxtrav = 100e6;

    /* Forward-declare the material base class. This will be
        overriden later. */
    class MaterialBase;

    class ObjectBase {
    public:
        // Constructor and destructor.
        ObjectBase() = default;
        virtual ~ObjectBase() = default;

        // Function to test for intersections.
        virtual bool TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal, glm::dvec3 &localColor) noexcept;
        void SetTransformMatrix(const GTform &transformMatrix) noexcept;
        // Function to test whether two floating-point numbers are close to being equal.
        bool CloseEnough(const double f1, const double f2) const noexcept;

        // Function to assign a material.
        bool AssignMaterial(const std::shared_ptr<MaterialBase> &objectMaterial) noexcept;

        // Public member variables.
        // The base colour of the object.
        glm::dvec3 m_baseColor{};
        GTform m_transformMatrix;

        // A reference to the material assigned to this object.
        std::shared_ptr<MaterialBase> m_pMaterial;
        bool m_hasMaterial = false;
        glm::dvec2 m_uvCoords;
    };

}  // namespace qbRT
   // NOLINTEND(*-include-cleaner)
