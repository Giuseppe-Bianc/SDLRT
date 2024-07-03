//
// Created by gbian on 03/07/2024.
//

#pragma once

#include "../headers.hpp"
#include <SDL3/SDL.h>

class qbImage {
public:
    qbImage() = default;
    qbImage(const qbImage &other) = delete;
    qbImage(qbImage &&other) noexcept = delete;
    qbImage &operator=(const qbImage &other) = delete;
    qbImage &operator=(qbImage &&other) = delete;

    // The destructor.
    ~qbImage() {
        if(m_pTexture != nullptr) [[unlikely]] { SDL_DestroyTexture(m_pTexture); }
    }
    // Function to initialize.
    void Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer);

    // Function to set pixels.
    void SetPixel(const int x, const int y, const double red, const double green, const double blue);

    // Function to return the image for display.
    void Display();

    [[nodiscard]] int &GetXSize() noexcept { return m_xSize; }
    [[nodiscard]] int &GetYSize() noexcept { return m_ySize; }

private:
    std::uint32_t ConvertColor(const double red, const double green, const double blue);
    void InitTexture();

    // Arrays to store image data.
    std::vector<std::vector<double>> m_rChannel;
    std::vector<std::vector<double>> m_gChannel;
    std::vector<std::vector<double>> m_bChannel;

    // And store the size of the image.
    int m_xSize = 0;
    int m_ySize = 0;
    int totalSize = 0;
    std::size_t m_bufferSize;
    std::ranges::iota_view<int, int> xRange;
    std::ranges::iota_view<int, int> yRange;

    // SDL2 stuff.
    SDL_Renderer *m_pRenderer;
    SDL_Texture *m_pTexture;
    SDL_FRect srcRect;
    SDL_FRect bounds;
};