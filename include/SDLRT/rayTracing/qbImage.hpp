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
    void SetPixel(const int x, const int y, const SDL_Color &color);

    // Function to return the image for display.
    void Display();

    [[nodiscard]] int GetXSize() const noexcept { return m_xSize; }
    [[nodiscard]] int GetYSize() const noexcept { return m_ySize; }

private:
    // std::uint32_t ConvertColor(const double red, const double green, const double blue);
    void InitTexture() noexcept;

    // Arrays to store image data.
    std::vector<std::vector<SDL_Color>> m_colorData;

    // And store the size of the image.
    int m_xSize = 0;
    int m_ySize = 0;
    int totalSize = 0;
    std::size_t m_bufferSize = 0;
    std::ranges::iota_view<int, int> xRange;
    std::ranges::iota_view<int, int> yRange;

    // SDL2 stuff.
    SDL_Renderer *m_pRenderer{nullptr};
    SDL_Texture *m_pTexture{nullptr};
};
