//
// Created by gbian on 03/07/2024.
//

#pragma once

#include "Ray.hpp"

namespace qbRT {

    class Camera {
    public:
        Camera();
        void SetPosition(const glm::dvec3 &newPosition);
        void SetLookAt(const glm::dvec3 &newLookAt);
        void SetUp(const glm::dvec3 &upVector);
        void SetLength(double newLength);
        void SetHorzSize(double newSize);
        void SetAspect(double newAspectRatio);

        glm::dvec3 GetPosition();
        glm::dvec3 GetLookAt();
        glm::dvec3 GetUp();
        glm::dvec3 GetU();
        glm::dvec3 GetV();
        glm::dvec3 GetScreenCentre();
        double GetLength();
        double GetHorzSize();
        double GetAspect();

        Ray GenerateRay(float proScreenX, float proScreenY);

        void UpdateCameraGeometry();

    private:
        glm::dvec3 m_cameraPosition;
        glm::dvec3 m_cameraLookAt;
        glm::dvec3 m_cameraUp;
        double m_cameraLength;
        double m_cameraHorzSize;
        double m_cameraAspectRatio;

        glm::dvec3 m_alignmentVector;
        glm::dvec3 m_projectionScreenU;
        glm::dvec3 m_projectionScreenV;
        glm::dvec3 m_projectionScreen;
        glm::dvec3 m_projectionScreenCentre;
    };

}  // namespace qbRT
