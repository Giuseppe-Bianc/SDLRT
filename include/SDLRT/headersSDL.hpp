//
// Created by gbian on 06/07/2024.
//

#pragma once
// NOLINTBEGIN(*-include-cleaner)
#include "headers.hpp"
#include <SDL3/SDL.h>
static inline constexpr std::uint8_t MAX_COLOR_VALUE = 255;
static inline constexpr auto ALPHA_VALUE = MAX_COLOR_VALUE;
namespace TypeSizes {
    /// Size of a boolean type.
    static inline constexpr std::size_t sizeOfSDL_Color = sizeof(SDL_Color);
}  // namespace TypeSizes

inline SDL_Color SDL_COLOR(const double r, const double g, const double b) {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    auto *format = SDL_CreatePixelFormat(SDL_PIXELFORMAT_RGBA32);
#else
    auto *format = SDL_CreatePixelFormat(SDL_PIXELFORMAT_BGRA32);
#endif
    const uint32_t pixel = SDL_MapRGBA(format, NC_UI8T(r), NC_UI8T(g), NC_UI8T(b), ALPHA_VALUE);

    // Extract the RGBA components from the mapped pixel
    std::uint8_t extracted_r{};
    std::uint8_t extracted_g{};
    std::uint8_t extracted_b{};
    std::uint8_t extracted_a{};
    SDL_GetRGBA(pixel, format, &extracted_b, &extracted_g, &extracted_r, &extracted_a);

    return {extracted_r, extracted_g, extracted_b, extracted_a};
}

inline constexpr SDL_Color SDL_COLORM(const SDL_Color &color, double intensity) {
    return {NC_UI8T(color.r * intensity), NC_UI8T(color.g * intensity), NC_UI8T(color.b * intensity), ALPHA_VALUE};
}
// NOLINTEND(*-include-cleaner)
