//
// Created by gbian on 03/07/2024.
//

#include "SDLRT/rayTracing/ObjectBase.hpp"

static inline constexpr auto EPSILON = 1e-21;

namespace qbRT {
    bool ObjectBase::TestIntersection([[maybe_unused]] const Ray &castRay, [[maybe_unused]] glm::dvec3 &intPoint,
                                      [[maybe_unused]] glm::dvec3 &localNormal, [[maybe_unused]] SDL_Color &localColor) const noexcept {
        return false;
    }

    bool ObjectBase::CloseEnough(const double f1, const double f2) noexcept { return std::fabs(f1 - f2) < EPSILON; }
}  // namespace qbRT
