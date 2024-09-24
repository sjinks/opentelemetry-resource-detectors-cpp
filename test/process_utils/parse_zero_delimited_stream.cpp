#include <sstream>
#include <tuple>

#include <gtest/gtest.h>

#include "process_utils.h"

class ParseZeroDelimitedStringTest : public testing::TestWithParam<std::tuple<std::string, std::vector<std::string>>> {
};

TEST_P(ParseZeroDelimitedStringTest, parse)
{
    std::string input;
    std::vector<std::string> expected;

    std::tie(input, expected) = GetParam();

    std::istringstream iss(input);
    auto actual = parse_zero_delimited_stream(iss);
    EXPECT_EQ(actual, expected);
}

using namespace std::string_literals;

INSTANTIATE_TEST_SUITE_P(
    Table,
    ParseZeroDelimitedStringTest,
    testing::Values(
        std::make_tuple(
            "/usr/bin/keepassxc-proxy\0chrome-extension://oboonakemofpalcgghocfoadofidjkkk/\0"s,
            std::vector<std::string>{"/usr/bin/keepassxc-proxy", "chrome-extension://oboonakemofpalcgghocfoadofidjkkk/"}
        ),
        std::make_tuple("cat\0/proc/self/cmdline\0"s, std::vector<std::string>{"cat", "/proc/self/cmdline"}),
        std::make_tuple("mc\0"s, std::vector<std::string>{"mc"}),
        std::make_tuple("", std::vector<std::string>{})
    )
);
