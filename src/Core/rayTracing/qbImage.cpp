//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "SDLRT/rayTracing/qbImage.hpp"

#include <SDLRT/timer/Timer.hpp>

static inline constexpr std::uint8_t MAX_COLOR_VALUE = 255;
static inline constexpr auto ALPHA_VALUE = MAX_COLOR_VALUE;
// Function to initialize.
void qbImage::Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer) {
    vnd::Timer timer{"init qbimage"};
    m_colorData.resize(xSize, std::vector<SDL_Color>(ySize, {0, 0, 0, 0}));
    m_xSize = xSize;
    m_ySize = ySize;
    xRange = std::views::iota(0, m_xSize);
    yRange = std::views::iota(0, m_ySize);
    totalSize = m_xSize * m_ySize;
    m_bufferSize = m_xSize * sizeof(SDL_Color);
    // Store the pointer to the renderer.
    m_pRenderer = pRenderer;
    LINFO("{}", timer);
    // Initialise the texture.
    InitTexture();
}

// Function to set pixels.
void qbImage::SetPixel(const int x, const int y, const SDL_Color &color) { m_colorData.at(x).at(y) = color; }

void qbImage::Display() {
    // Allocate memory for a pixel buffer.
    std::vector<SDL_Color> tempPixels{C_ST(totalSize), {0, 0, 0, 0}};
    std::size_t index{};

    std::ranges::for_each(xRange, [&](const int x) {
        std::ranges::for_each(yRange, [&](const int y) {
            index = C_ST((y * m_xSize) + x);
            tempPixels[index] = m_colorData[x][y];
        });
    });

    // Update the texture with the pixel buffer.
    SDL_UpdateTexture(m_pTexture, nullptr, tempPixels.data(), C_I(m_bufferSize));

    // Copy the texture to the renderer.
    const SDL_FRect srcRect{0, 0, C_F(m_xSize), C_F(m_ySize)};
    SDL_FRect bounds;
    bounds = srcRect;
    SDL_RenderTexture(m_pRenderer, m_pTexture, &srcRect, &bounds);
}
void qbImage::InitTexture() noexcept {
    // Delete any previously created texture.
    if(m_pTexture != nullptr) SDL_DestroyTexture(m_pTexture);
    // Create the texture that will store the image.
    SDL_Surface *tempSurface = SDL_CreateSurface(m_xSize, m_ySize, SDL_PIXELFORMAT_RGBA32);
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_DestroySurface(tempSurface);
}
// NOLINTEND(*-include-cleaner)