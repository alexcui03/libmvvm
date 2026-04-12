/**
 * This file is part of libmvvm.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <meta>
#include <ranges>
#include <string_view>

namespace mvvm::utils {

/**
 * Check whether there contains a annotation with given type.
 */
template <class Annotation>
consteval bool has_annotation(std::meta::info info) {
    for (auto annotation : std::meta::annotations_of(info)) {
        if (
            std::meta::type_of(annotation) == ^^Annotation ||
            std::meta::type_of(annotation) == ^^const Annotation    // G++ will treat annotations as const type
        ) {
            return true;
        }
    }
    return false;
}

/**
 * Get member function with identifier. If no valid member function is found,
 * then `class_meta` will be returned.
 */
consteval std::meta::info get_member_function(
    std::meta::info class_meta,
    std::string_view identifier,
    std::meta::access_context context = std::meta::access_context::current()
) {
    for (auto member : std::meta::members_of(class_meta, context)) {
        if (
            std::meta::is_function(member) &&
            std::meta::has_identifier(member) &&
            std::meta::identifier_of(member) == identifier
        ) {
            return member;
        }
    }
    return class_meta;
}

/**
 * Constexpr version of std::toupper. (ASCII characters only)
 */
constexpr char toupper(char c) {
    return (c >= 'a' && c <= 'z') ? (c - 'a' + 'A') : c;
}

/**
 * Convert identifier from snake_case to PascalCase in compile time.
 * Example: `snake_to_pascal_case("test_name"sv)` is `"TestName"`
 */
consteval auto snake_to_pascal_case(std::string_view identifier) {
    using std::operator""sv;
    return std::define_static_string(
        identifier |
        std::views::split("_"sv) |
        std::views::filter([](auto &&token) {
            return token.size() > 0;
        }) |
        std::views::transform([](auto &&token) {
            return std::views::concat(
                std::views::single(toupper(token[0])),
                token | std::views::drop(1)
            );
        }) |
        std::views::join
    );
}

}
