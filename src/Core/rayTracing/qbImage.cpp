//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "SDLRT/rayTracing/qbImage.hpp"

#include <SDLRT/timer/Timer.hpp>
DISABLE_WARNINGS_PUSH(6022 26446 26447)
// Function to initialize.
void qbImage::Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer) {
    vnd::Timer timer{"init qbimage"};
    // Resize the image arrays.
    m_rChannel.resize(xSize, std::vector<double>(ySize, 0.0));
    m_gChannel.resize(xSize, std::vector<double>(ySize, 0.0));
    m_bChannel.resize(xSize, std::vector<double>(ySize, 0.0));
    m_xSize = xSize;
    m_ySize = ySize;
    xRange = std::views::iota(C_ST(0), C_ST(xSize));
    yRange = std::views::iota(C_ST(0), C_ST(ySize));
    totalSize = xSize * ySize;
    m_bufferSize = xSize * TypeSizes::sizeOfUint32T;
    // Store the pointer to the renderer.
    m_pRenderer = pRenderer;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    format = SDL_CreatePixelFormat(SDL_PIXELFORMAT_RGBA32);
#else
    format = SDL_CreatePixelFormat(SDL_PIXELFORMAT_BGRA32);
#endif
    const SDL_FRect rect{0, 0, C_F(xSize), C_F(ySize)};
    srcRect = rect;
    bounds = rect;
    LINFO("{}", timer);
    // Initialise the texture.
    InitTexture();
}

// Function to set pixels.
void qbImage::SetPixel(const std::size_t x, const std::size_t y, const glm::dvec3 &color) noexcept {
    m_rChannel[x][y] = color[0];
    m_gChannel[x][y] = color[1];
    m_bChannel[x][y] = color[2];
}

std::vector<std::uint32_t> qbImage::ArrangePixels() {
    ComputeMaxValues();
    vnd::Timer timer{"qbimage::ArrangePixels"};
    std::vector<std::uint32_t> tempPixels(C_ST(totalSize), 0);
    for(const auto &x : xRange) {
        for(const auto &y : yRange) {
            tempPixels[y * C_ST(m_xSize) + x] = ConvertColor(m_rChannel[x][y], m_gChannel[x][y], m_bChannel[x][y]);
        }
    }
    LINFO("{}", timer);
    return tempPixels;
}

void qbImage::Display(const std::vector<std::uint32_t> &colorData) const noexcept {
    // Update the texture with the pixel buffer.
    SDL_UpdateTexture(m_pTexture, nullptr, colorData.data(), C_I(m_bufferSize));

    SDL_RenderTexture(m_pRenderer, m_pTexture, &srcRect, &bounds);
}

void qbImage::InitTexture() noexcept {
    const vnd::AutoTimer timer{"init qbimage::texture"};
    // Delete any previously created texture.
    if(m_pTexture != nullptr) [[unlikely]] { SDL_DestroyTexture(m_pTexture); }
    // Create the texture that will store the image.
    SDL_Surface *tempSurface = SDL_CreateSurface(m_xSize, m_ySize, SDL_PIXELFORMAT_RGBA32);
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_DestroySurface(tempSurface);
}

std::uint32_t qbImage::ConvertColor(const double red, const double green, const double blue) const noexcept {
    const auto r = NC_UI8T((red / m_overallMax) * MAX_COLOR_VALUE);
    const auto g = NC_UI8T((green / m_overallMax) * MAX_COLOR_VALUE);
    const auto b = NC_UI8T((blue / m_overallMax) * MAX_COLOR_VALUE);
    return SDL_MapRGBA(format, r, g, b, ALPHA_VALUE);
}

void qbImage::ComputeMaxValues() {
    const vnd::AutoTimer timer{"qbimage::ComputeMaxValues"};
    for(const auto &x : xRange) {
        for(const auto &y : yRange) {
            m_maxRed = std::max(m_maxRed, m_rChannel[x][y]);
            m_maxGreen = std::max(m_maxGreen, m_gChannel[x][y]);
            m_maxBlue = std::max(m_maxBlue, m_bChannel[x][y]);
        }
    }
    m_overallMax = std::max({m_maxRed, m_maxGreen, m_maxBlue});
}
// NOLINTEND(*-include-cleaner)
DISABLE_WARNINGS_POP()
