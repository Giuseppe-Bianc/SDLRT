//
// Created by gbian on 03/07/2024.
//

#include "SDLRT/rayTracing/Scene.hpp"

namespace qbRT {
    bool Scene::Render(qbImage &outputImage) {
        // Get the dimensions of the output image.
        int xSize = outputImage.GetXSize();
        int ySize = outputImage.GetYSize();

        // Create some colour variations.
        for(int x = 0; x < xSize; ++x) {
            for(int y = 0; y < ySize; ++y) {
                auto red = (C_D(x) / C_D(xSize)) * 255.0;
                auto green = (C_D(y) / C_D(ySize)) * 255.0;
                outputImage.SetPixel(x, y, red, green, 0.0);
            }
        }

        return true;
    }
}  // namespace qbRT