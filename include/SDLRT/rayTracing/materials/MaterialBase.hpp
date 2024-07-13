//
// Created by gbian on 09/07/2024.
//

#pragma once

#include "../Ray.hpp"
#include "../light/LightBase.hpp"
#include "../primitives/ObjectBase.hpp"
#include "../textures/TextureBase.hpp"
namespace qbRT {

    class MaterialBase {
    public:
        MaterialBase() noexcept = default;
        virtual ~MaterialBase() = default;

        // Function to return the color of the material.
        virtual glm::dvec3 ComputeColor(const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                        const std::vector<std::shared_ptr<LightBase>> &lightList,
                                        const std::shared_ptr<ObjectBase> &currentObject, const glm::dvec3 &intPoint,
                                        glm::dvec3 &localNormal, const Ray &cameraRay) noexcept;

        // Function to compute diffuse color.
        static glm::dvec3 ComputeDiffuseColor(const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                              const std::vector<std::shared_ptr<LightBase>> &lightList,
                                              const std::shared_ptr<ObjectBase> &currentObject, const glm::dvec3 &intPoint,
                                              const glm::dvec3 &localNormal, const glm::dvec3 &baseColor) noexcept;

        // Function to compute the reflection color.
        glm::dvec3 ComputeReflectionColor(const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                          const std::vector<std::shared_ptr<LightBase>> &lightList,
                                          const std::shared_ptr<ObjectBase> &currentObject, const glm::dvec3 &intPoint,
                                          const glm::dvec3 &localNormal, const Ray &incidentRay) const noexcept;

        // Function to cast a ray into the scene.
        bool CastRay(const Ray &castRay, const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                     const std::shared_ptr<ObjectBase> &thisObject, std::shared_ptr<ObjectBase> &closestObject, glm::dvec3 &closestIntPoint,
                     glm::dvec3 &closestLocalNormal, glm::dvec3 &closestLocalColor) const noexcept;
        void AssignTexture(const std::shared_ptr<Texture::TextureBase> &inputTexture);

    public:
        // Counter for the number of relection rays.
        inline static int m_maxReflectionRays = 3;
        inline static int m_reflectionRayCount = 0;
        inline static glm::dvec3 m_ambientColor{1.0, 1.0, 1.0};
        inline static double m_ambientIntensity = 0.2;
        // List of texures assigned to this material.
        std::vector<std::shared_ptr<qbRT::Texture::TextureBase>> m_textureList;

        // Flat to indicate whether at least one texture has been assigned.
        bool m_hasTexture = false;

    private:
    };
}  // namespace qbRT
