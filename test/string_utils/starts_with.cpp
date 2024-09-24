#include <tuple>

#include <gtest/gtest.h>

#include "string_utils.h"

class ParametrizedStartsWithTest : public testing::TestWithParam<std::tuple<std::string_view, std::string_view, bool>> {
};

TEST_P(ParametrizedStartsWithTest, StartsWith)
{
    std::string_view input;
    std::string_view prefix;
    bool expected;
    std::tie(input, prefix, expected) = GetParam();

    auto actual = starts_with(input, prefix);
    EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
    Table,
    ParametrizedStartsWithTest,
    testing::Values(
        std::make_tuple("string", "prefix", false),
        std::make_tuple("prefixed", "prefix", true),
        std::make_tuple("prefix", "prefixed", false),
        std::make_tuple("", "", true),
        std::make_tuple("string", "", true)
    )
);
