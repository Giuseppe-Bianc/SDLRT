//
// Created by gbian on 04/07/2024.
//

#pragma once

#include "../primitives/ObjectBase.hpp"
#include "../Ray.hpp"

namespace qbRT {

    class LightBase {
    public:
        // Constructor / destructor.
        LightBase() = default;
        virtual ~LightBase() = default;

        // Function to compute illumination contribution.
        virtual bool ComputeIllumination(const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
                                         const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                         const std::shared_ptr<ObjectBase> &currentObject, glm::dvec3 &color, double &intensity) noexcept;

        // Public member variables.
        glm::dvec3 m_color{};
        glm::dvec3 m_location{};
        double m_intensity = 0.0;
    };

}  // namespace qbRT
