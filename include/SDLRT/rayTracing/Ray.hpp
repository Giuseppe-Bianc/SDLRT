//
// Created by gbian on 03/07/2024.
//

#pragma once
#include "../headers.hpp"
namespace qbRT {

    class Ray {
    public:
        Ray() : m_point1{0.0, 0.0, 0.0}, m_point2{0.0, 0.0, 1.0}, m_lab{m_point2 - m_point1} {}
        Ray(const glm::dvec3 &point1, const glm::dvec3 &point2);

        [[nodiscard]] glm::dvec3 GetPoint1() const;
        [[nodiscard]] glm::dvec3 GetPoint2() const;
        [[nodiscard]] glm::dvec3 GetLab() const;
        void SetPoint1(const glm::dvec3 &vec);
        void SetPoint2(const glm::dvec3 &vec);
        void SetLab(const glm::dvec3 &vec);

        glm::dvec3 m_point1;
        glm::dvec3 m_point2;
        glm::dvec3 m_lab;
    };

}  // namespace qbRT
