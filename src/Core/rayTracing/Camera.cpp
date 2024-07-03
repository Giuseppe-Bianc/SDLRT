//
// Created by gbian on 03/07/2024.
//

#include "SDLRT/rayTracing/Camera.hpp"

namespace qbRT {
    Camera::Camera() {  // The default constructor.
        m_cameraPosition = glm::dvec3{0.0, -10.0, 0.0};
        m_cameraLookAt = glm::dvec3{0.0, 0.0, 0.0};
        m_cameraUp = glm::dvec3{0.0, 0.0, 1.0};
        m_cameraLength = 1.0;
        m_cameraHorzSize = 1.0;
        m_cameraAspectRatio = 1.0;
    }
    void Camera::SetPosition(const glm::dvec3 &newPosition) { m_cameraPosition = newPosition; }

    void Camera::SetLookAt(const glm::dvec3 &newLookAt) { m_cameraLookAt = newLookAt; }

    void Camera::SetUp(const glm::dvec3 &upVector) { m_cameraUp = upVector; }

    void Camera::SetLength(double newLength) { m_cameraLength = newLength; }

    void Camera::SetHorzSize(double newHorzSize) { m_cameraHorzSize = newHorzSize; }

    void Camera::SetAspect(double newAspect) { m_cameraAspectRatio = newAspect; }

    // Method to return the position of the camera.
    glm::dvec3 Camera::GetPosition() { return m_cameraPosition; }

    // Method to return the look at of the camera.
    glm::dvec3 Camera::GetLookAt() { return m_cameraLookAt; }

    // Method to return the up vector of the camera.
    glm::dvec3 Camera::GetUp() { return m_cameraUp; }

    // Method to return the length of the camera.
    double Camera::GetLength() { return m_cameraLength; }

    double Camera::GetHorzSize() { return m_cameraHorzSize; }

    double Camera::GetAspect() { return m_cameraAspectRatio; }

    glm::dvec3 Camera::GetU() { return m_projectionScreenU; }

    glm::dvec3 Camera::GetV() { return m_projectionScreenV; }

    glm::dvec3 Camera::GetScreenCentre() { return m_projectionScreenCentre; }

    void Camera::UpdateCameraGeometry() {
        // First, compute the vector from the camera position to the LookAt position.
        m_alignmentVector = m_cameraLookAt - m_cameraPosition;
        m_alignmentVector = glm::normalize(m_alignmentVector);

        // Second, compute the alpha and beta unit vectors.
        m_projectionScreenU = glm::cross(m_alignmentVector, m_cameraUp);
        m_projectionScreenU = glm::normalize(m_projectionScreenU);
        m_projectionScreenV = glm::cross(m_projectionScreenU, m_alignmentVector);
        m_projectionScreenV = glm::normalize(m_projectionScreenV);

        // Thirdly, compute the position of the center point of the screen.
        m_projectionScreenCentre = m_cameraPosition + (m_cameraLength * m_alignmentVector);

        // Modify the U and V vectors to match the size and aspect ratio.
        m_projectionScreenU = m_projectionScreenU * m_cameraHorzSize;
        m_projectionScreenV = m_projectionScreenV * (m_cameraHorzSize / m_cameraAspectRatio);
    }

    Ray Camera::GenerateRay(float proScreenX, float proScreenY) {
        // Compute the location of the screen point in world coordinates.
        glm::dvec3 screenWorldPart1 = m_projectionScreenCentre + (m_projectionScreenU * C_D(proScreenX));
        glm::dvec3 screenWorldCoordinate = screenWorldPart1 + (m_projectionScreenV * C_D(proScreenY));

        // Use this point along with the camera position to compute the ray.
        return Ray(m_cameraPosition, screenWorldCoordinate);
    }
}  // namespace qbRT