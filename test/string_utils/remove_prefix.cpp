#include <gtest/gtest.h>

#include "string_utils.h"

TEST(RemovePrefixTest, RemovePrefix)
{
    const std::string_view input    = "prefix_string";
    const std::string_view prefix   = "prefix_";
    const std::string_view expected = "string";

    auto actual = remove_prefix(input, prefix);
    EXPECT_EQ(actual, expected);
}
