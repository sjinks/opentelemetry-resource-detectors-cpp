#include <tuple>

#include <gtest/gtest.h>

#include "string_utils.h"

class ParametrizedEndsWithTest : public testing::TestWithParam<std::tuple<std::string_view, std::string_view, bool>> {};

TEST_P(ParametrizedEndsWithTest, EndsWith)
{
    std::string_view input;
    std::string_view suffix;
    bool expected;
    std::tie(input, suffix, expected) = GetParam();

    auto actual = ends_with(input, suffix);
    EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
    Table,
    ParametrizedEndsWithTest,
    testing::Values(
        std::make_tuple("string", "suffix", false),
        std::make_tuple("string with suffix", "suffix", true),
        std::make_tuple("suffix", "the suffix", false),
        std::make_tuple("", "", true),
        std::make_tuple("string", "", true)
    )
);
