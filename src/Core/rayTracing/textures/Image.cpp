#include "SDLRT/rayTracing/textures/Image.hpp"

namespace qbRT::Texture {

    Image::~Image() {
        if(m_imageLoaded) {
            // Make sure that we free the image surface.
            SDL_DestroySurface(m_imageSurface);
            m_imageLoaded = false;
        }
    }

    glm::dvec4 qbRT::Texture::Image::GetColor(const glm::dvec2 &uvCoords) {
        glm::dvec4 outputColor{};

        if(!m_imageLoaded) {
            /* If no image has been loaded yet,
                set the color to the default purple
                regardless of the (u,v) position. */
            outputColor = glm::dvec4{1.0, 0.0, 1.0, 1.0};
        } else {
            // Apply the local transform to the (u,v) coordinates.
            glm::dvec2 inputLoc = uvCoords;
            glm::dvec2 newLoc = ApplyTransform(inputLoc);
            double u = newLoc[0];
            double v = newLoc[1];

            // Convert (u,v) to image dimensions (x,y).
            int x = C_I(round(((u + 1.0) / 2.0) * C_D(m_xSize)));
            int y = m_ySize - (C_I(round(((v + 1.0) / 2.0) * C_D(m_ySize))));

            /* Modulo arithmetic to account for possible tiling.
                For example:
                m_xSize = 10;
                x = 5 =>
                    ((5 % 10) + 10) % 10 = 5

                x = 10 =>
                    ((10 % 10) + 10) % 10 = 0

                x = 11 =>
                    ((11 % 10) + 10) % 10 = 1

                x = -1 =>
                    ((-1 % 10) + 10) % 10 = 9

                x = -5 =>
                    ((-5 % 10) + 10) % 10 = 5

                x = -10 =>
                    ((-10 % 10) + 10) % 10 = 0

                x = -11 =>
                    ((-11 % 10) + 10) % 10 = 9  */

            x = ((x % m_xSize) + m_xSize) % m_xSize;
            y = ((y % m_ySize) + m_ySize) % m_ySize;

            // Verify that we are within the image.
            // Probably not necessary, but seems like a good idea just in case.
            if((x >= 0) && (x < m_xSize) && (y >= 0) && (y < m_ySize)) {
                // Convert (x,y) to a linear index.
                int pixelIndex = x + (y * (m_pitch / m_bytesPerPixel));

                // Get a pointer to the pixel data.
                uint32_t *pixels = (uint32_t *)m_imageSurface->pixels;

                // Extract the current pixel value.
                uint32_t currentPixel = pixels[pixelIndex];

                // Convert to RGB.
                uint8_t r, g, b, a;
                SDL_GetRGBA(currentPixel, m_imageSurface->format, &r, &g, &b, &a);

                // Set the outputColor vector accordingly.
                outputColor[0] = C_D(r) / 255.0;
                outputColor[1] = C_D(g) / 255.0;
                outputColor[2] = C_D(b) / 255.0;
                outputColor[3] = C_D(a) / 255.0;
            }
        }

        return outputColor;
    }

    bool qbRT::Texture::Image::LoadImage(std::string fileName) {
        if(m_imageLoaded) SDL_DestroySurface(m_imageSurface);

        m_fileName = fileName;
        m_imageSurface = SDL_LoadBMP(fileName.c_str());

        if(!m_imageSurface) {
            LERROR("Failed to load image. {}.", SDL_GetError());
            m_imageLoaded = false;
            return false;
        }

        // Extract useful information.
        m_xSize = m_imageSurface->w;
        m_ySize = m_imageSurface->h;
        m_pitch = m_imageSurface->pitch;
        m_pixelFormat = m_imageSurface->format;
        m_bytesPerPixel = m_pixelFormat->bytes_per_pixel;
        m_rMask = m_pixelFormat->Rmask;
        m_gMask = m_pixelFormat->Gmask;
        m_bMask = m_pixelFormat->Bmask;
        m_aMask = m_pixelFormat->Amask;

        LINFO("Loaded {} by {}.", m_imageSurface->w, m_imageSurface->h);
        LINFO("Bytes per pixel = {}", +m_bytesPerPixel);
        LINFO("Pitch = {}", m_pitch);

        m_imageLoaded = true;
        return true;
    }

}  // namespace qbRT::Texture