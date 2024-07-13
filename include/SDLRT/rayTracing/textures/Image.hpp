#pragma once

#include "texturebase.hpp"

namespace qbRT::Texture {
    class Image : public TextureBase {
    public:
        Image() = default;
        virtual ~Image() override;

        // Function to return the color.
        virtual glm::dvec4 GetColor(const glm::dvec2 &uvCoords) override;

        // Function to load the image to be used.
        bool LoadImage(std::string fileName);

    private:
        std::string m_fileName;
        SDL_Surface *m_imageSurface;
        SDL_PixelFormat *m_pixelFormat;
        bool m_imageLoaded = false;
        int m_xSize, m_ySize, m_pitch;
        uint8_t m_bytesPerPixel;
        uint32_t m_rMask, m_gMask, m_bMask, m_aMask;
    };
}  // namespace qbRT::Texture
