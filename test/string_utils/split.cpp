#include <tuple>

#include <gtest/gtest.h>

#include "string_utils.h"

class ParametrizedSplitTest
    : public testing::TestWithParam<std::tuple<std::string_view, std::vector<std::string_view>>> {};

TEST_P(ParametrizedSplitTest, Split)
{
    std::string_view input;
    std::vector<std::string_view> expected;
    std::tie(input, expected) = GetParam();

    auto actual = split(input, '/');
    EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
    Table,
    ParametrizedSplitTest,
    testing::Values(
        std::make_tuple("a/b/c", std::vector<std::string_view>{"a", "b", "c"}),
        std::make_tuple("a/b/c/", std::vector<std::string_view>{"a", "b", "c", ""}),
        std::make_tuple("a", std::vector<std::string_view>{"a"}),
        std::make_tuple("", std::vector<std::string_view>{""})
    )
);
