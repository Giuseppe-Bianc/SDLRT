//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headersSDL.hpp"

class qbImage {
public:
    qbImage() = default;
    qbImage(const qbImage &other) = delete;
    qbImage(qbImage &&other) noexcept = delete;
    qbImage &operator=(const qbImage &other) = delete;
    qbImage &operator=(qbImage &&other) = delete;

    // The destructor.
    ~qbImage() {
        if(format != nullptr) [[unlikely]] { SDL_DestroyPixelFormat(format); }
        if(m_pTexture != nullptr) [[unlikely]] { SDL_DestroyTexture(m_pTexture); }
    }
    // Function to initialize.
    void Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer);

    // Function to set pixels.
    void SetPixel(const std::size_t x, const size_t y, const glm::dvec3 &color) noexcept;
    void SetPixel(const std::size_t x, const std::size_t y, const glm::dvec4 &color) noexcept;
    void ComputeMaxValues();
    [[nodiscard]] std::vector<std::uint32_t> ArrangePixels();

    // Function to return the image for display.
    void Display(const std::vector<std::uint32_t> &colorData) const noexcept;

    [[nodiscard]] int GetXSize() const noexcept { return m_xSize; }
    [[nodiscard]] int GetYSize() const noexcept { return m_ySize; }

private:
    [[nodiscard]] std::uint32_t ConvertColor(const double red, const double green, const double blue) const noexcept;
    void InitTexture() noexcept;

    // Arrays to store image data.
    std::vector<std::vector<double>> m_rChannel;
    std::vector<std::vector<double>> m_gChannel;
    std::vector<std::vector<double>> m_bChannel;

    // And store the size of the image.
    int m_xSize = 0;
    int m_ySize = 0;
    int totalSize = 0;
    double m_maxRed = 0.0;
    double m_maxGreen = 0.0;
    double m_maxBlue = 0.0;
    double m_overallMax = 0.0;
    SDL_FRect srcRect{};
    SDL_FRect bounds{};
    std::size_t m_bufferSize = 0;
    std::ranges::iota_view<std::size_t, std::size_t> xRange;
    std::ranges::iota_view<std::size_t, std::size_t> yRange;

    // SDL2 stuff.
    SDL_Renderer *m_pRenderer{nullptr};
    SDL_Texture *m_pTexture{nullptr};
    SDL_PixelFormat *format{nullptr};
};
// NOLINTEND(*-include-cleaner)
