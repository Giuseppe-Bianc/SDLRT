//
// Created by gbian on 04/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once
#include "Ray.hpp"

namespace qbRT {

    static inline constexpr bool FWDTFORM = true;
    static inline constexpr bool BCKTFORM = false;

    class GTform {
    public:
        GTform(const GTform &other) = default;
        GTform(GTform &&other) noexcept = default;
        GTform &operator=(GTform &&other) = default;
        // Constructor & destructor.
        GTform() = default;
        ~GTform() = default;

        // Construct from a pair of matrices.
        GTform(const glm::dmat4 &fwd, const glm::dmat4 &bck) noexcept : m_fwdtfm(fwd), m_bcktfm(bck) {}
        // Function to set translation, rotation and scale components.
        void SetTransform(const glm::dvec3 &translation, const glm::dvec3 &rotation, const glm::dvec3 &scale);

        // Functions to return the transform matrices.
        glm::dmat4 GetForward() const noexcept { return m_fwdtfm; }
        glm::dmat4 GetBackward() const noexcept { return m_bcktfm; }

        // Function to apply the transform.
        Ray Apply(const Ray &inputRay, bool dirFlag) const;
        glm::dvec3 Apply(const glm::dvec3 &inputVector, bool dirFlag) const;

        // Overload operators.
        friend GTform operator*(const GTform &lhs, const GTform &rhs);

        // Overload the assignment operator.
        GTform &operator=(const GTform &rhs) noexcept;

        // Function to print transform matrix to STDOUT.
        void PrintMatrix(bool dirFlag) const;

        // Function to allow printing of vectors.
        // static void PrintVector(const glm::dvec3 &vector);

    private:

        glm::dmat4 m_fwdtfm = glm::dmat4(1.0);
        glm::dmat4 m_bcktfm = glm::dmat4(1.0);
    };
}  // namespace qbRT
// NOLINTEND(*-include-cleaner)