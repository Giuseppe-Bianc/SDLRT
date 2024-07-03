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
    // Resize image data arrays.
    m_rChannel.resize(xSize, std::vector<double>(ySize, 0));
    m_gChannel.resize(xSize, std::vector<double>(ySize, 0));
    m_bChannel.resize(xSize, std::vector<double>(ySize, 0));

    // Store the dimensions.
    m_xSize = xSize;
    m_ySize = ySize;
    xRange = std::views::iota(0, m_xSize);
    yRange = std::views::iota(0, m_ySize);
    totalSize = m_xSize * m_ySize;
    srcRect = SDL_FRect{0, 0, C_F(m_xSize), C_F(m_xSize)};
    bounds = srcRect;
    m_bufferSize = m_xSize * TypeSizes::sizeOfUint32T;
    // Store the pointer to the renderer.
    m_pRenderer = pRenderer;
    LINFO("{}", timer);
    // Initialise the texture.
    InitTexture();
}

// Function to set pixels.
void qbImage::SetPixel(const int x, const int y, const double red, const double green, const double blue) {
    m_rChannel.at(x).at(y) = red;
    m_gChannel.at(x).at(y) = green;
    m_bChannel.at(x).at(y) = blue;
}

// Function to generate the display.
void qbImage::Display() {
    // Allocate memory for a pixel buffer using std::vector.
    std::vector<std::uint32_t> tempPixels(totalSize, 0);

    std::ranges::for_each(xRange, [&](const int x) {
        std::ranges::for_each(yRange, [&](const int y) {
            tempPixels[(y * m_xSize) + x] = ConvertColor(m_rChannel[x][y], m_gChannel[x][y], m_bChannel[x][y]);
        });
    });

    // Update the texture with the pixel buffer.
    SDL_UpdateTexture(m_pTexture, nullptr, tempPixels.data(), C_I(m_bufferSize));
    SDL_RenderTexture(m_pRenderer, m_pTexture, &srcRect, &bounds);
}

// Function to return the image as an SDL2 texture.
void qbImage::InitTexture() {
    vnd::AutoTimer timer{"init texture"};
    // Delete any previously created texture before we create a new one.
    if(m_pTexture != nullptr) [[unlikely]] { SDL_DestroyTexture(m_pTexture); }

    // Create the texture that will store the image.
    SDL_Surface *tempSurface = SDL_CreateSurface(m_xSize, m_ySize, SDL_PIXELFORMAT_RGBA32);
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_DestroySurface(tempSurface);
}

// Function to convert color to Uint32
std::uint32_t qbImage::ConvertColor(const double red, const double green, const double blue) {
    // Convert colours to unsigned char.
    auto r = C_UC(red);
    auto g = C_UC(green);
    auto b = C_UC(blue);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    return (b << 24) + (g << 16) + (r << 8) + ALPHA_VALUE;
#else
    return (ALPHA_VALUE << 24) + (r << 16) + (g << 8) + b;
#endif
}
// NOLINTEND(*-include-cleaner)