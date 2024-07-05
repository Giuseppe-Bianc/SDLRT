//
// Created by gbian on 04/07/2024.
//

#include "SDLRT/rayTracing/LightBase.hpp"
DISABLE_WARNINGS_PUSH(4100)
namespace qbRT {
    bool LightBase::ComputeIllumination(const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
                                        const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                        const std::shared_ptr<ObjectBase> &currentObject, SDL_Color &color, double &intensity) noexcept {
        return false;
    }

} // qbRT
DISABLE_WARNINGS_POP()