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
    m_colorData.resize(xSize, std::vector<SDL_Color>(ySize, SDL_COLOR(0.0, 0.0, 0.0)));
    m_xSize = xSize;
    m_ySize = ySize;
    xRange = std::views::iota(0, xSize);
    yRange = std::views::iota(0, ySize);
    totalSize = xSize * ySize;
    m_bufferSize = xSize * TypeSizes::sizeOfSDL_Color;
    // Store the pointer to the renderer.
    m_pRenderer = pRenderer;
    const SDL_FRect rect{0, 0, C_F(xSize), C_F(ySize)};
    srcRect = rect;
    bounds = rect;
    LINFO("{}", timer);
    // Initialise the texture.
    InitTexture();
}

// Function to set pixels.
void qbImage::SetPixel(const int x, const int y, const SDL_Color &color) { m_colorData.at(x).at(y) = color; }

std::vector<SDL_Color> qbImage::ArrangePixels() const {
    vnd::Timer timer{"qbimage::ArrangePixels"};
    std::vector<SDL_Color> tempPixels(C_ST(totalSize), SDL_COLOR(0.0, 0.0, 0.0));
    std::size_t index{};
    for(const auto &x : xRange) {
        for(const auto &y : yRange) {
            index = C_ST((C_ST(y) * m_xSize) + x);
            tempPixels[index] = m_colorData[x][y];
        }
    }
    LINFO("{}", timer);
    return tempPixels;
}

void qbImage::Display(const std::vector<SDL_Color> &colorData) const noexcept {
    // Allocate memory for a pixel buffer.

    // Update the texture with the pixel buffer.
    SDL_UpdateTexture(m_pTexture, nullptr, colorData.data(), C_I(m_bufferSize));

    SDL_RenderTexture(m_pRenderer, m_pTexture, &srcRect, &bounds);
}

void qbImage::InitTexture() noexcept {
    const vnd::AutoTimer timer{"init init qbimage::texture"};
    // Delete any previously created texture.
    if(m_pTexture != nullptr) [[unlikely]] { SDL_DestroyTexture(m_pTexture); }
    // Create the texture that will store the image.
    SDL_Surface *tempSurface = SDL_CreateSurface(m_xSize, m_ySize, SDL_PIXELFORMAT_RGBA32);
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_DestroySurface(tempSurface);
}
// NOLINTEND(*-include-cleaner)
DISABLE_WARNINGS_POP()
