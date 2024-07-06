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

template <typename T> constexpr SDL_Color SDL_COLOR(T r, T g, T b) {
    return SDL_Color{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        NC_UI8T(b), NC_UI8T(g), NC_UI8T(r), ALPHA_VALUE
#else
        NC_UI8T(r), NC_UI8T(g), NC_UI8T(b), ALPHA_VALUE
#endif
    };
}

template <typename T> constexpr SDL_Color SDL_COLORM(const SDL_Color &color, T intensity) {
    return SDL_Color{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        NC_UI8T(color.b * intensity), NC_UI8T(color.g * intensity), NC_UI8T(color.r * intensity), ALPHA_VALUE
#else
        NC_UI8T(color.r * intensity), NC_UI8T(color.g * intensity), NC_UI8T(color.b * intensity), ALPHA_VALUE
#endif
    };
}

// NOLINTEND(*-include-cleaner)