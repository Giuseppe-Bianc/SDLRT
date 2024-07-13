//
// Created by gbian on 09/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "SDLRT/rayTracing/materials/SimpleMaterial.hpp"

namespace qbRT {
    // Function to return the color.
    glm::dvec3 SimpleMaterial::ComputeColor(const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                            const std::vector<std::shared_ptr<LightBase>> &lightList,
                                            const std::shared_ptr<ObjectBase> &currentObject, const glm::dvec3 &intPoint,
                                            glm::dvec3 &localNormal, const Ray &cameraRay) noexcept {
        // Define the initial material colors.
        glm::dvec3 matColor{};
        glm::dvec3 refColor{};
        glm::dvec3 difColor{};
        glm::dvec3 spcColor{};

        if(!m_hasTexture)
            difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_baseColor);
        else
            difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal,
                                           glm::dvec3(m_textureList.at(0)->GetColor(currentObject->m_uvCoords)));

        // Compute the reflection component.
        if(m_reflectivity > 0.0) {
            refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);
        }

        // Combine reflection and diffuse components.
        // matColor = (refColor * m_reflectivity) + (difColor * (1 - m_reflectivity));
        matColor = glm::mix(difColor, refColor, m_reflectivity);

        // Compute the specular component.
        if(m_shininess > 0.0) { spcColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay); }

        // Add the specular component to the final color.
        matColor = matColor + spcColor;

        return matColor;
    }

    DISABLE_WARNINGS_PUSH(26447)

    // Function to compute the specular highlights.
    glm::dvec3 SimpleMaterial::ComputeSpecular(const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                               const std::vector<std::shared_ptr<LightBase>> &lightList, const glm::dvec3 &intPoint,
                                               const glm::dvec3 &localNormal, const Ray &cameraRay) const noexcept {
        glm::dvec3 spcColor{};
        double red = 0.0;
        double green = 0.0;
        double blue = 0.0;
        glm::dvec3 poi{};
        glm::dvec3 poiNormal{};
        glm::dvec3 poiColor{};
        bool validInt = false;

        // Loop through all of the lights in the scene.
        for(const auto &currentLight : lightList) {
            /* Check for intersections with all objects in the scene. */
            double intensity = 0.0;

            // Construct a vector pointing from the intersection point to the light.
            const glm::dvec3 lightDir = glm::normalize(currentLight->m_location - intPoint);

            // Compute a start point.
            const glm::dvec3 startPoint = intPoint + (lightDir * 0.001);

            // Construct a ray from the point of intersection to the light.
            const Ray lightRay(startPoint, startPoint + lightDir);

            /* Loop through all objects in the scene to check if any
                obstruct light from this source. */
            for(const auto &sceneObject : objectList) {
                validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
                if(validInt) { break; }
            }

            /* If no intersections were found, then proceed with
                computing the specular component. */
            if(!validInt) {
                // Compute the reflection vector.
                // const glm::dvec3 d = lightRay.m_lab;
                // const glm::dvec3 r = glm::normalize(d - (2 * glm::dot(d, localNormal) * localNormal));
                const glm::dvec3 r = glm::normalize(glm::reflect(lightRay.m_lab, localNormal));

                // Compute the dot product.
                const glm::dvec3 v = glm::normalize(cameraRay.m_lab);
                const double dotProduct = glm::dot(r, v);

                // Only proceed if the dot product is positive.
                if(dotProduct > 0.0) { intensity = m_reflectivity * std::pow(dotProduct, m_shininess); }
            }

            red += currentLight->m_color[0] * intensity;
            green += currentLight->m_color[1] * intensity;
            blue += currentLight->m_color[2] * intensity;
        }

        spcColor[0] = red;
        spcColor[1] = green;
        spcColor[2] = blue;
        return spcColor;
    }
    DISABLE_WARNINGS_POP()
}  // namespace qbRT
   // NOLINTEND(*-include-cleaner)