#include <tuple>

#include <gtest/gtest.h>

#include "string_utils.h"

class ParametrizedNthWordTest
    : public testing::TestWithParam<std::tuple<std::string_view, std::size_t, std::string_view>> {};

TEST_P(ParametrizedNthWordTest, NthWord)
{
    std::string_view input;
    std::size_t n;
    std::string_view expected;
    std::tie(input, n, expected) = GetParam();

    auto actual = nth_word(input, n);
    EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
    Table,
    ParametrizedNthWordTest,
    testing::Values(
        std::make_tuple("one two three", 1, "one"),
        std::make_tuple("one two three", 2, "two"),
        std::make_tuple("one two three", 3, "three"),
        std::make_tuple("one two three", 0, ""),
        std::make_tuple("one two three", 4, ""),

        std::make_tuple("   one  two   three ", 1, "one"),
        std::make_tuple("   one  two   three ", 2, "two"),
        std::make_tuple("   one  two   three ", 3, "three"),
        std::make_tuple("   one  two   three ", 4, ""),

        std::make_tuple("      ", 1, "")
    )
);
