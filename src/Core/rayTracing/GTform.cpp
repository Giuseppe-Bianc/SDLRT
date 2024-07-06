//
// Created by gbian on 04/07/2024.
//

#include "SDLRT/rayTracing/GTform.hpp"

namespace qbRT {

    void GTform::SetTransform(const glm::dvec3 &translation, const glm::dvec3 &rotation, const glm::dvec3 &scale) {
        // Define matrices for translation, rotation, and scale using GLM functions.
        const glm::dmat4 translationMatrix = glm::translate(glm::dmat4(1.0), translation);
        const glm::dmat4 rotationMatrixX = glm::rotate(glm::dmat4(1.0), rotation.x, glm::dvec3(1.0, 0.0, 0.0));
        const glm::dmat4 rotationMatrixY = glm::rotate(glm::dmat4(1.0), rotation.y, glm::dvec3(0.0, 1.0, 0.0));
        const glm::dmat4 rotationMatrixZ = glm::rotate(glm::dmat4(1.0), rotation.z, glm::dvec3(0.0, 0.0, 1.0));
        const glm::dmat4 scaleMatrix = glm::scale(glm::dmat4(1.0), scale);
        // Combine to give the final forward transform matrix.
        m_fwdtfm = translationMatrix * rotationMatrixZ * rotationMatrixY * rotationMatrixX * scaleMatrix;
        // std::swap(m_fwdtfm[0][3], m_fwdtfm[3][0]);
        //  Combine to give the final forward transform matrix.
        m_bcktfm = glm::inverse(m_fwdtfm);
    }

    Ray GTform::Apply(const qbRT::Ray &inputRay, bool dirFlag) const {
        // Create an output object.
        Ray outputRay;

        if(dirFlag) {
            // Apply the forward transform.
            outputRay.m_point1 = this->Apply(inputRay.m_point1, FWDTFORM);
            outputRay.m_point2 = this->Apply(inputRay.m_point2, FWDTFORM);
            outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
        } else {
            // Apply the backward transform.
            outputRay.m_point1 = this->Apply(inputRay.m_point1, BCKTFORM);
            outputRay.m_point2 = this->Apply(inputRay.m_point2, BCKTFORM);
            outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
        }

        return outputRay;
    }

    glm::dvec3 GTform::Apply(const glm::dvec3 &inputVector, bool dirFlag) const {
        // Convert inputVector to a 4-element vector.
        const glm::dvec4 tempVector{inputVector[0], inputVector[1], inputVector[2], 1.0};

        // Create a vector for the result.
        glm::dvec4 resultVector{};

        if(dirFlag) {
            // Apply the forward transform.
            resultVector = m_fwdtfm * tempVector;
        } else {
            // Apply the backward transform.
            resultVector = m_bcktfm * tempVector;
        }
        // Reform the output as a 3-element vector.
        return {resultVector[0], resultVector[1], resultVector[2]};
    }
}  // namespace qbRT

namespace qbRT {
    GTform operator*(const qbRT::GTform &lhs, const qbRT::GTform &rhs) {
        // Form the product of the two forward transforms.
        const glm::dmat4 fwdResult = lhs.m_fwdtfm * rhs.m_fwdtfm;

        // Compute the backward transform as the inverse of the forward transform.
        const glm::dmat4 bckResult = glm::inverse(fwdResult);

        // Form the final result.
        GTform finalResult(fwdResult, bckResult);

        return finalResult;
    }
}  // namespace qbRT
namespace qbRT {
    GTform &GTform::operator=(const GTform &rhs) noexcept {
        // Make sure that we're not assigning to ourself.
        if(this != &rhs) {
            m_fwdtfm = rhs.m_fwdtfm;
            m_bcktfm = rhs.m_bcktfm;
        }

        return *this;
    }

    void GTform::PrintMatrix(bool dirFlag) const {
        if(dirFlag) {
            LINFO(m_fwdtfm);
        } else {
            LINFO(m_bcktfm);
        }
    }
}  // namespace qbRT
