//
// Created by gbian on 13/07/2024.
//

#include "../../../../include/SDLRT/rayTracing/materials/SimpleRefractive.hpp"

namespace qbRT {
    // Function to return the color.
    glm::dvec3 SimpleRefractive::ComputeColor(const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                              const std::vector<std::shared_ptr<LightBase>> &lightList,
                                              const std::shared_ptr<ObjectBase> &currentObject, const glm::dvec3 &intPoint,
                                              glm::dvec3 &localNormal, const Ray &cameraRay) noexcept {
        // Define the initial material colors.
        glm::dvec3 matColor{};
        glm::dvec3 refColor{};
        glm::dvec3 difColor{};
        glm::dvec3 spcColor{};
        glm::dvec3 trnColor{};

        // Compute the diffuse component.
        if(!m_hasTexture)
            difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_baseColor);
        else
            difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal,
                                           glm::dvec3(m_textureList.at(0)->GetColor(currentObject->m_uvCoords)));

        // Compute the reflection component.
        if(m_reflectivity > 0.0) refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

        // Combine the reflection and diffuse components.
        matColor = (refColor * m_reflectivity) + (difColor * (1.0 - m_reflectivity));

        // Compute the refractive component.
        if(m_translucency > 0.0) trnColor = ComputeTranslucency(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

        // And combine with the current color.
        matColor = (trnColor * m_translucency) + (matColor * (1.0 - m_translucency));

        // And compute the specular component.
        if(m_shininess > 0.0) spcColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);

        // Finally, add the specular component.
        matColor = matColor + spcColor;

        return matColor;
    }

    // Function to compute the color due to translucency.
    glm::dvec3 SimpleRefractive::ComputeTranslucency(const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
                                                     const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
                                                     const std::shared_ptr<qbRT::ObjectBase> &currentObject, const glm::dvec3 &intPoint,
                                                     glm::dvec3 &localNormal, const qbRT::Ray &incidentRay) noexcept {
        glm::dvec3 trnColor{};

        // Compute the refracted vector.
        glm::dvec3 p = glm::normalize(incidentRay.m_lab);
        glm::dvec3 tempNormal = localNormal;
        double r = 1.0 / m_ior;
        double c = -glm::dot(tempNormal, p);
        if(c < 0.0) {
            tempNormal = tempNormal * -1.0;
            c = -glm::dot(tempNormal, p);
        }

        glm::dvec3 refractedVector = r * p + (r * c - sqrt(1.0 - pow(r, 2.0) * (1.0 - pow(c, 2.0)))) * tempNormal;

        // Construct the refracted ray.
        qbRT::Ray refractedRay(intPoint + (refractedVector * 0.01), intPoint + refractedVector);

        // Test for secondary intersection with this object.
        std::shared_ptr<qbRT::ObjectBase> closestObject;
        glm::dvec3 closestIntPoint{};
        glm::dvec3 closestLocalNormal{};
        glm::dvec3 closestLocalColor{};
        glm::dvec3 newIntPoint{};
        glm::dvec3 newLocalNormal{};
        glm::dvec3 newLocalColor{};
        bool test = currentObject->TestIntersection(refractedRay, newIntPoint, newLocalNormal, newLocalColor);
        bool intersectionFound = false;
        qbRT::Ray finalRay;
        if(test) {
            // Compute the refracted vector.
            glm::dvec3 p2 = glm::normalize(refractedRay.m_lab);
            glm::dvec3 tempNormal2 = newLocalNormal;
            double r2 = m_ior;
            double c2 = -glm::dot(tempNormal2, p2);
            if(c2 < 0.0) {
                tempNormal2 = tempNormal2 * -1.0;
                c2 = -glm::dot(tempNormal2, p2);
            }
            glm::dvec3 refractedVector2 = r2 * p2 + (r2 * c2 - sqrt(1.0 - pow(r2, 2.0) * (1.0 - pow(c2, 2.0)))) * tempNormal2;

            // Compute the refracted ray.
            qbRT::Ray refractedRay2(newIntPoint + (refractedVector2 * 0.01), newIntPoint + refractedVector2);

            // Cast this ray into the scene.
            intersectionFound = CastRay(refractedRay2, objectList, currentObject, closestObject, closestIntPoint, closestLocalNormal,
                                        closestLocalColor);
            finalRay = refractedRay2;
        } else {
            /* No secondary intersections were found, so continue the original refracted ray. */
            intersectionFound = CastRay(refractedRay, objectList, currentObject, closestObject, closestIntPoint, closestLocalNormal,
                                        closestLocalColor);
            finalRay = refractedRay;
        }

        // Compute the color for closest object.
        glm::dvec3 matColor{3};
        if(intersectionFound) {
            // Check if a material has been assigned.
            if(closestObject->m_hasMaterial) {
                matColor = closestObject->m_pMaterial->ComputeColor(objectList, lightList, closestObject, closestIntPoint,
                                                                    closestLocalNormal, finalRay);
            } else {
                matColor = qbRT::MaterialBase::ComputeDiffuseColor(objectList, lightList, closestObject, closestIntPoint,
                                                                   closestLocalNormal, closestObject->m_baseColor);
            }
        } else {
            // Leave matColor as it is.
        }

        trnColor = matColor;
        return trnColor;
    }

    // Function to compute the specular highlights.
    glm::dvec3 qbRT::SimpleRefractive::ComputeSpecular(const std::vector<std::shared_ptr<qbRT::ObjectBase>> &objectList,
                                                       const std::vector<std::shared_ptr<qbRT::LightBase>> &lightList,
                                                       const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
                                                       const qbRT::Ray &cameraRay) noexcept {
        glm::dvec3 spcColor{};
        double red = 0.0;
        double green = 0.0;
        double blue = 0.0;

        // Loop through all of the lights in the scene.
        for(const auto &currentLight : lightList) {
            /* Check for intersections with all objects in the scene. */
            double intensity = 0.0;

            // Construct a vector pointing from the intersection point to the light.
            glm::dvec3 lightDir = glm::normalize(currentLight->m_location - intPoint);

            // Compute a start point.
            glm::dvec3 startPoint = intPoint + (lightDir * 0.001);

            // Construct a ray from the point of intersection to the light.
            qbRT::Ray lightRay(startPoint, startPoint + lightDir);

            /* Loop through all objects in the scene to check if any
                obstruct light from this source. */
            glm::dvec3 poi{};
            glm::dvec3 poiNormal{};
            glm::dvec3 poiColor{};
            bool validInt = false;
            for(const auto &sceneObject : objectList) {
                validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
                if(validInt) break;
            }

            /* If no intersections were found, then proceed with
                computing the specular component. */
            if(!validInt) {
                // Compute the reflection vector.
                glm::dvec3 d = lightRay.m_lab;
                glm::dvec3 r = glm::normalize(d - (2 * glm::dot(d, localNormal) * localNormal));

                // Compute the dot product.
                glm::dvec3 v = glm::normalize(cameraRay.m_lab);
                double dotProduct = glm::dot(r, v);

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
}  // namespace qbRT