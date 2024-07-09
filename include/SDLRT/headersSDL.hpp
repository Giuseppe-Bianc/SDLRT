//
// Created by gbian on 06/07/2024.
//

#pragma once
// NOLINTBEGIN(*-include-cleaner)
#include "headers.hpp"
#include <SDL3/SDL.h>
static inline constexpr std::uint8_t MAX_COLOR_VALUE = 255;
static inline constexpr auto ALPHA_VALUE = MAX_COLOR_VALUE;
static inline constexpr auto ALPHA_VALUEI = C_I(MAX_COLOR_VALUE);

DISABLE_WARNINGS_PUSH(26447 26460 26481)

// Example usage:
// SDL_Color color = {255, 165, 0, 255};
// std::string s1 = fmt::format("{}", color);                // Default format: (r:255, g:165, b:0, a:255)
// std::string s2 = fmt::format("{:x}", color);              // Uppercase hex format: #FFA500FF
// std::string s3 = fmt::format("{:X}", color);              // Lowercase hex format: #ffa500ff
// std::string s4 = fmt::format("{:R}", color);              // RGB format: FFA500
// std::string s5 = fmt::format("{:a}", color);              // ARGB format: FFA5FF00
// std::string s6 = fmt::format("{:b}", color);              // BGRA format: 00FFA5FF
// std::string s7 = fmt::format("Grayscale: {g}\n", color);  // Grayscale: #805050
// std::string s8 = fmt::format("HSL: {h}\n", color);        // HSL: hsl(30,75.38%,66.67%)
template <> struct fmt::formatter<SDL_Color> : fmt::formatter<std::string_view> {
    // Parse format specifier (e.g., "x" for uppercase hex, "X" for lowercase hex, "r" for RGBA, "g" for grayscale)
    constexpr auto parse(fmt::format_parse_context &ctx) {
        auto it = ctx.begin();
        auto end = ctx.end();
        if(it == nullptr) {
            // Handle the null case
            throw fmt::format_error("Iterator is null");
        }
        if(it != end) {
            switch(*it) {
            case 'x':
            case 'X':
            case 'r':
            case 'R':
            case 'a':
            case 'b':
            case 'g':
                format_style = *it++;
                break;
            default:
                break;
            }
        }
        if(it != end && *it != '}') { throw fmt::format_error("invalid format"); }
        return it;
    }

    int grayscaleFromRGBA(const SDL_Color &color) {
        // Calculate luminance
        const double Y = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;

        // Convert to integer (assuming integer output range)
        int gray = std::clamp(C_I(Y), 0, ALPHA_VALUEI);

        // Optionally, you can multiply by the alpha value to adjust for transparency
        gray = gray * color.a / ALPHA_VALUEI;

        return gray;
    }

    int grayscaleFromBGRA(const SDL_Color &color) {
        // Calculate luminance
        const double Y = 0.2126 * color.b + 0.7152 * color.g + 0.0722 * color.r;

        // Convert to integer (assuming integer output range)
        int gray = std::clamp(C_I(Y), 0, ALPHA_VALUEI);

        // Optionally, you can multiply by the alpha value to adjust for transparency
        gray = gray * color.a / ALPHA_VALUEI;

        return gray;
    }
    template <typename FormatContext> auto format(const SDL_Color &color, FormatContext &ctx) {
        if constexpr(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            return format_color_big_endian(color, ctx);
        } else {
            return format_color_little_endian(color, ctx);
        }
    }

    template <typename FormatContext> auto format_color_big_endian(const SDL_Color &color, FormatContext &ctx) {
        switch(format_style) {
        case 'x':
            return fmt::formatter<std::string_view>::format(FORMAT("#{:02X}{:02X}{:02X}{:02X}", color.r, color.g, color.b, color.a), ctx);
        case 'X':
            return fmt::formatter<std::string_view>::format(FORMAT("#{:#02x}{:02x}{:02x}{:02x}", color.r, color.g, color.b, color.a), ctx);
        case 'R':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}", color.r, color.g, color.b), ctx);
        case 'a':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}{:02X}", color.a, color.r, color.g, color.b), ctx);
        case 'b':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}{:02X}", color.b, color.g, color.r, color.a), ctx);
        case 'g':  // Calculate grayscale value (average of RGB components)
            {
                int grayscale = grayscaleFromRGBA(color);
                return fmt::formatter<std::string_view>::format(FORMAT("#{:X}", grayscale), ctx);
            }
        default:
            return fmt::formatter<std::string_view>::format(FORMAT("(r:{}, g:{}, b:{}, a:{})", color.r, color.g, color.b, color.a), ctx);
        }
    }

    template <typename FormatContext> auto format_color_little_endian(const SDL_Color &color, FormatContext &ctx) {
        switch(format_style) {
        case 'x':
            return fmt::formatter<std::string_view>::format(FORMAT("#{:02X}{:02X}{:02X}{:02X}", color.b, color.g, color.r, color.a), ctx);
        case 'X':
            return fmt::formatter<std::string_view>::format(FORMAT("#{:#02x}{:02x}{:02x}{:02x}", color.b, color.g, color.r, color.a), ctx);
        case 'R':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}", color.b, color.g, color.r), ctx);
        case 'a':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}{:02X}", color.a, color.b, color.g, color.r), ctx);
        case 'b':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}{:02X}", color.r, color.g, color.b, color.a), ctx);
        case 'g':  // Calculate grayscale value (average of RGB components)
            {
                int grayscale = grayscaleFromBGRA(color);
                return fmt::formatter<std::string_view>::format(FORMAT("#{:X}", grayscale), ctx);
            }
        default:
            return fmt::formatter<std::string_view>::format(FORMAT("(r:{}, g:{}, b:{}, a:{})", color.b, color.g, color.r, color.a), ctx);
        }
    }

private:
    char format_style = 'r';  // Default format style is 'r' (RGBA)
};

// add more formatting capabilitis like gray scale and full HSL representation

/*namespace TypeSizes {
    /// Size of a boolean type.
    static inline constexpr std::size_t sizeOfSDL_Color = sizeof(SDL_Color);
}  // namespace TypeSizes
constexpr SDL_Color SDL_COLOR_BLACK() noexcept { return {0, 0, 0, ALPHA_VALUE}; }

inline SDL_Color SDL_COLOR(const double r, const double g, const double b) noexcept {
    SDL_PixelFormat *format = nullptr;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    format = SDL_CreatePixelFormat(SDL_PIXELFORMAT_RGBA32);
#else
    format = SDL_CreatePixelFormat(SDL_PIXELFORMAT_BGRA32);
#endif

    if(format == nullptr) {
        // Handle SDL_CreatePixelFormat failure
        LERROR("SDL_CreatePixelFormat failed Error: {}", SDL_GetError());
        return SDL_COLOR_BLACK();  // Or some default color
    }
    const uint32_t pixel = SDL_MapRGBA(format, NC_UI8T(r), NC_UI8T(g), NC_UI8T(b), ALPHA_VALUE);

    // Extract the RGBA components from the mapped pixel
    std::uint8_t extracted_r{};
    std::uint8_t extracted_g{};
    std::uint8_t extracted_b{};
    std::uint8_t extracted_a{};
    SDL_GetRGBA(pixel, format, &extracted_b, &extracted_g, &extracted_r, &extracted_a);
    SDL_DestroyPixelFormat(format);

    return {extracted_r, extracted_g, extracted_b, extracted_a};
}

constexpr SDL_Color SDL_COLORM(const SDL_Color &color, double intensity) noexcept {
    return {NC_UI8T(color.r * intensity), NC_UI8T(color.g * intensity), NC_UI8T(color.b * intensity), ALPHA_VALUE};
}

*/
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner)
