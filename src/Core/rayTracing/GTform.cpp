//
// Created by gbian on 04/07/2024.
//

#include "SDLRT/rayTracing/GTform.hpp"

namespace qbRT {

    static inline constexpr auto IDENT_MAT = glm::dmat4(1.0);

    DISABLE_WARNINGS_PUSH(26447)

    void GTform::SetTransform(const glm::dvec3 &translation, const glm::dvec3 &rotation, const glm::dvec3 &scale) noexcept {
        // Define matrices for translation, rotation, and scale using GLM functions.
        const glm::dmat4 translationMatrix = glm::translate(IDENT_MAT, translation);
        const glm::dmat4 rotationMatrixX = glm::rotate(IDENT_MAT, rotation.x, glm::dvec3(1.0, 0.0, 0.0));
        const glm::dmat4 rotationMatrixY = glm::rotate(IDENT_MAT, rotation.y, glm::dvec3(0.0, 1.0, 0.0));
        const glm::dmat4 rotationMatrixZ = glm::rotate(IDENT_MAT, rotation.z, glm::dvec3(0.0, 0.0, 1.0));
        const glm::dmat4 scaleMatrix = glm::scale(IDENT_MAT, scale);
        // Combine to give the final forward transform matrix.
        m_fwdtfm = translationMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ * scaleMatrix;
        // std::swap(m_fwdtfm[0][3], m_fwdtfm[3][0]);
        //  Combine to give the final forward transform matrix.
        m_bcktfm = glm::inverse(m_fwdtfm);
    }
    DISABLE_WARNINGS_POP()

    Ray GTform::Apply(const Ray &inputRay, bool dirFlag) const noexcept {
        // Create an output object.
        Ray outputRay;

        if(dirFlag) {
            // Apply the forward transform.
            outputRay.m_point1 = this->Apply(inputRay.m_point1, FWDTFORM);
            outputRay.m_point2 = this->Apply(inputRay.m_point2, FWDTFORM);
        } else {
            // Apply the backward transform.
            outputRay.m_point1 = this->Apply(inputRay.m_point1, BCKTFORM);
            outputRay.m_point2 = this->Apply(inputRay.m_point2, BCKTFORM);
        }

        outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
        return outputRay;
    }

    glm::dvec3 GTform::Apply(const glm::dvec3 &inputVector, bool dirFlag) const noexcept {
        // Convert inputVector to a 4-element vector.
        const glm::dvec4 tempVector{inputVector, 1.0};

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
        return glm::dvec3(resultVector);
    }
}  // namespace qbRT

namespace qbRT {
    GTform operator*(const GTform &lhs, const GTform &rhs) {
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
        const glm::dmat4 &matrix = dirFlag ? m_fwdtfm : m_bcktfm;
        LINFO(matrix);
    }
}  // namespace qbRT
