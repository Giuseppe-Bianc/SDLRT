//
// Created by gbian on 03/07/2024.
//

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

        glm::dvec3 GetPosition() noexcept;
        glm::dvec3 GetLookAt() noexcept;
        glm::dvec3 GetUp() noexcept;
        glm::dvec3 GetU() noexcept;
        glm::dvec3 GetV() noexcept;
        glm::dvec3 GetScreenCentre() noexcept;
        double GetLength() noexcept;
        double GetHorzSize() noexcept;
        double GetAspect() noexcept;

        bool GenerateRay(double proScreenX, double proScreenY, Ray &cameraRay) const noexcept;

        void UpdateCameraGeometry();

    private:
        glm::dvec3 m_cameraPosition;
        glm::dvec3 m_cameraLookAt;
        glm::dvec3 m_cameraUp;
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
