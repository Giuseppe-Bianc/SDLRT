#pragma once
// NOLINTBEGIN

#include "disableWarn.hpp"
// clang-format off
/** \cond */
DISABLE_WARNINGS_PUSH(
    4005 4201 4459 4514 4625 4626 4820
    6244 6285 6385 6386 26409 26415 26418
    26429 26432 26437 26438 26440 26465
    26446 26447 26450 26451 26455 26457
    26459 26460 26461 26467 26472 26473
    26474 26475 26481 26482 26485 26490
    26491 26493 26494 26495 26496 26497
    26498 26800 26814 26818 26826)
/** \endcond */
#include <any>
#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <complex>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <execution>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <initializer_list>
#include <limits>
#include <map>
#include <memory>
#include <memory_resource>
#include <numbers>
#include <ostream>
#include <optional>
#include <print>
#include <random>
#include <ranges>
#include <set>
#include <source_location>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
// clang-format on
#include "cast/casts.hpp"
#include "glm_matld.hpp"
#include "glm_prety_string_cast.hpp"
#include "macros.hpp"
// Restore warning levels.
DISABLE_WARNINGS_POP()

namespace fs = std::filesystem;
namespace ch = std::chrono;

DISABLE_WARNINGS_PUSH(26481)
static inline constexpr std::string_view wtile = "qbRayTracer";
static inline constexpr std::size_t factor = 80;
static inline constexpr auto wwidth = 16 * factor;
static inline constexpr auto wheight = 9 * factor;
static inline constexpr long double NINFINITY = std::numeric_limits<long double>::infinity();
static inline constexpr long double PI = std::numbers::pi_v<long double>;
static inline constexpr long double TWO_PI = 2 * PI;
static inline constexpr long double HALF_PI = PI / 2;
static inline constexpr auto HALF_PID = NC_D(PI / 2);
static inline constexpr auto MAXDBL = std::numeric_limits<double>::max();
static inline constexpr const auto *CNL = "\n";
static inline constexpr const auto *CCR = "\r";
static inline constexpr auto NL = CNL[0];
static inline constexpr auto CR = CCR[0];
static inline constexpr char PNT = '.';
static inline constexpr char ECR = 'E';
static inline constexpr const auto *CRNL = "\r\n";
static inline constexpr char ctab = '\t';

#ifdef _WIN32                                     // Windows
static inline constexpr const auto *NEWL = CRNL;  // Windows
#elif defined macintosh                           // OS 9
static inline constexpr const auto *NEWL = &CCR;  // Classic Mac OS
#elif defined __unix__                            // Linux and Unix-like systems
static inline constexpr const auto *NEWL = CNL;  // Linux and Unix
#elif defined __APPLE__                           // macOS
static inline constexpr const auto *NEWL = CNL;  // macOS
#elif defined __VMS                               // OpenVMS
static inline constexpr const auto *NEWL = CRNL;  // OpenVMS
#elif defined __FreeBSD__                         // FreeBSD
static inline constexpr const auto *NEWL = CNL;  // FreeBSD
#else
static inline constexpr const auto *NEWL = CNL;  // Default case
#endif
DISABLE_WARNINGS_POP()

[[nodiscard]] inline constexpr auto is_divisor(std::integral auto n, std::integral auto d) noexcept -> bool { return n % d == 0; }

template <std::integral T> [[nodiscard]] constexpr auto find_divisors(T num) noexcept -> std::vector<T> {
    if(num < 1) {
        return {};  // Handle edge case where num is less than 1.
    }
    T num_sqrt = T(std::sqrt(num));
    std::vector<T> divisors;
    divisors.reserve(num_sqrt);

    for(const T &val : std::views::iota(T(1), num_sqrt + 1)) {
        T numBval = num / val;
        if(is_divisor(num, val)) {
            divisors.emplace_back(val);
            if(val != numBval) { divisors.emplace_back(numBval); }
        }
    }

    std::ranges::sort(divisors);

    return divisors;
}

/**
 * @brief Specialization of fmt::formatter for glm::vec.
 */
template <typename T, glm::length_t L, glm::qualifier Q> struct fmt::formatter<glm::vec<L, T, Q>> : formatter<std::string_view> {
    /**
     * @brief Format function for glm::vec.
     * @param vector The vector to be formatted.
     * @param ctx The format context.
     * @return The formatted string.
     */
    auto format(const glm::vec<L, T, Q> &vector, format_context &ctx) const -> format_context::iterator {
        return formatter<std::string_view>::format(glmp::to_string(vector), ctx);
    }
};

/**
 * @brief Specialization of fmt::formatter for glm::mat.
 */
template <typename T, glm::length_t C, glm::length_t R, glm::qualifier Q>
struct fmt::formatter<glm::mat<C, R, T, Q>> : formatter<std::string_view> {
    /**
     * @brief Format function for glm::mat.
     * @param matrix The matrix to be formatted.
     * @param ctx The format context.
     * @return The formatted string.
     */
    auto format(const glm::mat<C, R, T, Q> &matrix, format_context &ctx) const -> format_context::iterator {
        return formatter<std::string_view>::format(glmp::to_string(matrix), ctx);
    }
};

/**
 * @brief Specialization of fmt::formatter for glm::qua.
 */
template <typename T, glm::qualifier Q> struct fmt::formatter<glm::qua<T, Q>> : formatter<std::string_view> {
    /**
     * @brief Format function for glm::qua.
     * @param quaternion The quaternion to be formatted.
     * @param ctx The format context.
     * @return The formatted string.
     */
    template <typename FormatContext> auto format(const glm::qua<T, Q> &quaternion, format_context &ctx) const -> format_context::iterator {
        return formatter<std::string_view>::format(glmp::to_string(quaternion), ctx);
    }
};

/**
 * @brief Specialization of fmt::formatter for std::complex.
 */
template <typename T> struct fmt::formatter<std::complex<T>> : fmt::formatter<std::string_view> {
    /**
     * @brief Formats the std::complex for printing.
     * @param nodeType The value to be formatted.
     * @param ctx The formatting context.
     * @return The formatted string.
     */
    auto format(const std::complex<T> &num, format_context &ctx) const -> format_context::iterator {
        std::string name = FORMAT("({}, {})", std::real(num), std::imag(num));
        return fmt::formatter<std::string_view>::format(name, ctx);
    }
};
/** \endcond */

/**
 * @typedef StringPair
 * @brief A pair of strings.
 *
 * This type alias defines a pair of strings using `std::pair` from the C++ Standard Library.
 * It is useful for storing two associated strings together, such as key-value pairs or name-description pairs.
 */
using StringPair = std::pair<std::string, std::string>;

/**
 * @typedef StringPairVec
 * @brief A vector of string pairs.
 * This type alias defines a vector of `StringPair` using `std::vector` from the C++ Standard Library.
 * It is useful for storing a list of associated string pairs, such as a collection of key-value pairs.
 */
using StringPairVec = std::vector<StringPair>;

/**
 * @typedef StringVec
 * @brief A vector of strings.
 * This type alias defines a vector of strings using `std::vector` from the C++ Standard Library.
 * It is useful for storing a list of strings, such as a collection of words or lines from a text.
 */
using StringVec = std::vector<std::string>;

/**
 * @typedef StrViewVec
 * @brief A vector of string views.
 *
 * This type alias defines a vector of string views using `std::vector` from the C++ Standard Library.
 * `std::string_view` provides a non-owning view of a string, which can be useful for performance-sensitive
 * operations where creating a copy of a string is not necessary.
 * This is useful for storing a list of string views that reference parts of strings without owning them.
 */
using StrViewVec = std::vector<std::string_view>;

/**
 * @typedef OptionalSizeT
 * @brief An optional size_t value.
 *
 * This type alias defines an optional size_t using `std::optional` from the C++ Standard Library.
 * `std::optional` can be used to represent a value that may or may not be present. In this case, it represents
 * an optional `size_t` value, which can be useful for functions that may or may not return a valid size or index.
 */
using OptionalSizeT = std::optional<size_t>;

/**
 * @concept StringOrStringView
 * @brief Concept that checks if a type is either std::string or std::string_view.
 *
 * This concept is used to constrain template parameters to ensure that only `std::string` or `std::string_view` types are accepted.
 * It is useful for generic programming where a function or class template should only work with string types.
 *
 * @tparam T The type to check.
 *
 * @note This concept uses `std::same_as` from the C++ Standard Library, which checks if two types are the same.
 *
 * Example usage:
 * @code
 * template <StringOrStringView T>
 * void printString(T str) {
 *     std::cout << str << std::endl;
 * }
 * @endcode
 */
template <typename T>
concept StringOrStringView = std::same_as<std::remove_cvref_t<T>, std::string> || std::same_as<std::remove_cvref_t<T>, std::string_view> ||
                             std::convertible_to<T, std::string> || std::convertible_to<T, std::string_view> || (requires(const T &t) {
                                 { std::ranges::begin(t) } -> std::convertible_to<typename T::const_iterator>;
                                 { std::ranges::end(t) } -> std::convertible_to<typename T::const_iterator>;
                                 { t.data() } -> std::convertible_to<const char *>;
                                 { t.size() } -> std::integral;
                                 { t.length() } -> std::integral;
                             });
// NOLINTEND
