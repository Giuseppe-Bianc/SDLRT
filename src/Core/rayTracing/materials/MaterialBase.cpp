//
// Created by gbian on 09/07/2024.
//

#include "SDLRT/rayTracing/materials/MaterialBase.hpp"
namespace qbRT {
    // Function to compute the color of the material
    glm::dvec3 MaterialBase::ComputeColor([[maybe_unused]] const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                          [[maybe_unused]] const std::vector<std::shared_ptr<LightBase>> &lightList,
                                          [[maybe_unused]] const std::shared_ptr<ObjectBase> &currentObject,
                                          [[maybe_unused]] const glm::dvec3 &intPoint, [[maybe_unused]] glm::dvec3 &localNormal,
                                          [[maybe_unused]] const Ray &cameraRay) noexcept {
        // Define an initial material color.
        glm::dvec3 matColor{};

        return matColor;
    }

    // Function to compute the diffuse color.
    glm::dvec3 MaterialBase::ComputeDiffuseColor(const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                                 const std::vector<std::shared_ptr<LightBase>> &lightList,
                                                 const std::shared_ptr<ObjectBase> &currentObject, const glm::dvec3 &intPoint,
                                                 const glm::dvec3 &localNormal, const glm::dvec3 &baseColor) noexcept {
        // Compute the color due to diffuse illumination.
        glm::dvec3 diffuseColor{};
        double intensity{};
        glm::dvec3 color{};
        double red = 0.0;
        double green = 0.0;
        double blue = 0.0;
        bool validIllum = false;
        bool illumFound = false;
        for(const auto &currentLight : lightList) {
            validIllum = currentLight->ComputeIllumination(intPoint, localNormal, objectList, currentObject, color, intensity);
            if(validIllum) {
                illumFound = true;
                red += color[0] * intensity;
                green += color[1] * intensity;
                blue += color[2] * intensity;
            }
        }

        if(illumFound) {
            diffuseColor[0] = red * baseColor[0];
            diffuseColor[1] = green * baseColor[1];
            diffuseColor[2] = blue * baseColor[2];
        } else {
            // The ambient light condition.
            for(int i = 0; i < 3; ++i) diffuseColor[i] = (m_ambientColor[i] * m_ambientIntensity) * baseColor[i];
        }

        // Return the material color.
        return diffuseColor;
    }

    DISABLE_WARNINGS_PUSH(26447)
    // Function to compute the color due to reflection.
    glm::dvec3 MaterialBase::ComputeReflectionColor(const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                                    const std::vector<std::shared_ptr<LightBase>> &lightList,
                                                    const std::shared_ptr<ObjectBase> &currentObject, const glm::dvec3 &intPoint,
                                                    const glm::dvec3 &localNormal, const Ray &incidentRay) const noexcept {
        glm::dvec3 reflectionColor{};

        // Compute the reflection vector.
        // const glm::dvec3 d = incidentRay.m_lab;
        // const glm::dvec3 reflectionVector = d - (2 * glm::dot(d, localNormal) * localNormal);
        const glm::dvec3 reflectionVector = glm::reflect(incidentRay.m_lab, localNormal);
        // Construct the reflection ray.
        const Ray reflectionRay(intPoint, intPoint + reflectionVector);

        /* Cast this ray into the scene and find the closest object that it intersects with. */
        std::shared_ptr<ObjectBase> closestObject;
        glm::dvec3 closestIntPoint{};
        glm::dvec3 closestLocalNormal{};
        glm::dvec3 closestLocalColor{};
        const bool intersectionFound = CastRay(reflectionRay, objectList, currentObject, closestObject, closestIntPoint, closestLocalNormal,
                                               closestLocalColor);

        /* Compute illumination for closest object assuming that there was a
            valid intersection. */
        glm::dvec3 matColor{};
        if((intersectionFound) && (m_reflectionRayCount < m_maxReflectionRays)) {
            // Increment the reflectionRayCount.
            m_reflectionRayCount++;

            // Check if a material has been assigned.
            if(closestObject->m_hasMaterial) {
                // Use the material to compute the color.
                matColor = closestObject->m_pMaterial->ComputeColor(objectList, lightList, closestObject, closestIntPoint,
                                                                    closestLocalNormal, reflectionRay);
            } else {
                matColor = MaterialBase::ComputeDiffuseColor(objectList, lightList, closestObject, closestIntPoint, closestLocalNormal,
                                                             closestObject->m_baseColor);
            }
        }
        // else {
        //   // Leave matColor as it is.
        //}

        reflectionColor = matColor;
        return reflectionColor;
    }

    // Function to cast a ray into the scene.
    bool MaterialBase::CastRay(const Ray &castRay, const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                               const std::shared_ptr<ObjectBase> &thisObject, std::shared_ptr<ObjectBase> &closestObject,
                               glm::dvec3 &closestIntPoint, glm::dvec3 &closestLocalNormal, glm::dvec3 &closestLocalColor) const noexcept {
        // Test for intersections with all of the objects in the scene.
        glm::dvec3 intPoint{};
        glm::dvec3 localNormal{};
        glm::dvec3 localColor{};

        double dist{};
        double minDist = MAXDBL;
        bool validInt{};
        bool intersectionFound = false;
        for(const auto &currentObject : objectList) {
            if(currentObject != thisObject) {
                validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

                // If we have a valid intersection.
                if(validInt) {
                    // Set the flag to show that we found an intersection.
                    intersectionFound = true;

                    // Compute the distance between the source and the intersection point.
                    dist = glm::distance(intPoint, castRay.m_point1);

                    // Store a reference to this object if it is the closest.
                    if(dist < minDist) {
                        minDist = dist;
                        closestObject = currentObject;
                        closestIntPoint = intPoint;
                        closestLocalNormal = localNormal;
                        closestLocalColor = localColor;
                    }
                }
            }
        }

        return intersectionFound;
    }
    DISABLE_WARNINGS_POP()
    void MaterialBase::AssignTexture(const std::shared_ptr<Texture::TextureBase> &inputTexture) {
        m_textureList.emplace_back(inputTexture);
        m_hasTexture = true;
    }

}  // namespace qbRT
