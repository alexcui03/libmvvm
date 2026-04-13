/**
 * This file is part of libmvvm.
 * SPDX-License-Identifier: MIT
 */

#include <ranges>
#include <string_view>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <mvvm/utils.hpp>

using std::operator""sv;

TEST(Utils, SnakeToPascalCase) {
    constexpr auto simple = mvvm::utils::snake_to_pascal_case("simple");
    constexpr auto snake_case = mvvm::utils::snake_to_pascal_case("snake_case");
    constexpr auto _start_with_underscore = mvvm::utils::snake_to_pascal_case("_start_with_underscore");
    constexpr auto multiple__underscore = mvvm::utils::snake_to_pascal_case("multiple__underscore");
    constexpr auto single_letter_s = mvvm::utils::snake_to_pascal_case("single_letter_s");
    constexpr auto camelCase = mvvm::utils::snake_to_pascal_case("camelCase");

    EXPECT_STREQ(simple, "Simple");
    EXPECT_STREQ(snake_case, "SnakeCase");
    EXPECT_STREQ(_start_with_underscore, "StartWithUnderscore");
    EXPECT_STREQ(multiple__underscore, "MultipleUnderscore");
    EXPECT_STREQ(single_letter_s, "SingleLetterS");
    EXPECT_STREQ(camelCase, "CamelCase");
}

TEST(Utils, ToUpper) {
    constexpr auto lower = "abcdefghijklmnopqrstuvwxyz0123456789"sv;
    constexpr auto upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"sv;
    for (auto [input, expect] : std::views::zip(lower, upper)) {
        char result = mvvm::utils::toupper(input);
        EXPECT_EQ(result, expect);
    }
}
