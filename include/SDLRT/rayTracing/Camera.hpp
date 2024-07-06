//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "Ray.hpp"

namespace qbRT {

    class Camera {
    public:
        Camera() noexcept;
        void SetPosition(const glm::dvec3 &newPosition) noexcept;
        void SetLookAt(const glm::dvec3 &newLookAt) noexcept;
        void SetUp(const glm::dvec3 &upVector) noexcept;
        void SetLength(double newLength) noexcept;
        void SetHorzSize(double newSize) noexcept;
        void SetAspect(double newAspectRatio) noexcept;

        glm::dvec3 GetPosition() const noexcept;
        glm::dvec3 GetLookAt() const noexcept;
        glm::dvec3 GetUp() const noexcept;
        glm::dvec3 GetU() const noexcept;
        glm::dvec3 GetV()  const noexcept;
        glm::dvec3 GetScreenCentre()  const noexcept;
        double GetLength() const noexcept;
        double GetHorzSize() const noexcept;
        double GetAspect()  const noexcept;

        bool GenerateRay(double proScreenX, double proScreenY, Ray &cameraRay) const noexcept;

        void UpdateCameraGeometry();

    private:
        glm::dvec3 m_cameraPosition{};
        glm::dvec3 m_cameraLookAt{};
        glm::dvec3 m_cameraUp{};
        double m_cameraLength;
        double m_cameraHorzSize;
        double m_cameraAspectRatio;

        glm::dvec3 m_alignmentVector{};
        glm::dvec3 m_projectionScreenU{};
        glm::dvec3 m_projectionScreenV{};
        glm::dvec3 m_projectionScreen{};
        glm::dvec3 m_projectionScreenCentre{};
    };

}  // namespace qbRT
// NOLINTEND(*-include-cleaner)