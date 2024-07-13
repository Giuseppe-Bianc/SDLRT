//
// Created by gbian on 09/07/2024.
//

#pragma once

#include "MaterialBase.hpp"

namespace qbRT {

    class SimpleMaterial : public MaterialBase {
    public:
        ~SimpleMaterial() override = default;
        glm::dvec3 ComputeColor(const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                const std::vector<std::shared_ptr<LightBase>> &lightList, const std::shared_ptr<ObjectBase> &currentObject,
                                const glm::dvec3 &intPoint, glm::dvec3 &localNormal, const Ray &cameraRay) noexcept override;
        // Function to compute specular highlights.
        glm::dvec3 ComputeSpecular(const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                   const std::vector<std::shared_ptr<LightBase>> &lightList, const glm::dvec3 &intPoint,
                                   const glm::dvec3 &localNormal, const Ray &cameraRay) const noexcept;

    public:
        glm::dvec3 m_baseColor{1.0, 0.0, 1.0};
        double m_reflectivity = 0.0;
        double m_shininess = 0.0;
    };

}  // namespace qbRT
