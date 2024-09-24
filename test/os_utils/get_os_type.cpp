#include <tuple>

#include <gtest/gtest.h>
#include <opentelemetry/sdk/resource/semantic_conventions.h>

#include "os_utils.h"

class GetOsTypeTest : public testing::TestWithParam<std::tuple<std::string, std::string>> {};

TEST_P(GetOsTypeTest, get_os_type)
{
    std::string input;
    std::string expected;
    std::tie(input, expected) = GetParam();

    auto actual = get_os_type(input);
    EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
    Table,
    GetOsTypeTest,
    testing::Values(
        std::make_tuple("Linux", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kLinux),
        std::make_tuple("Windows_NT", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows),
        std::make_tuple("CYGWIN_NT-5.1", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows),
        std::make_tuple("CYGWIN_NT-6.1", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows),
        std::make_tuple(
            "CYGWIN_NT-6.1-WOW64", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows
        ),
        std::make_tuple("CYGWIN_NT-10.0", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows),
        std::make_tuple("MINGW32_NT-6.1", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows),
        std::make_tuple("MSYS_NT-6.1", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows),
        std::make_tuple("MINGW32_NT-10.0", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows),
        std::make_tuple("MSYS_NT-10.0", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows),
        std::make_tuple("DragonFly", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kDragonflybsd),
        std::make_tuple("FreeBSD", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kFreebsd),
        std::make_tuple("HP-UX", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kHpux),
        std::make_tuple("AIX", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kAix),
        std::make_tuple("Darwin", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kDarwin),
        std::make_tuple("NetBSD", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kNetbsd),
        std::make_tuple("OpenBSD", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kOpenbsd),
        std::make_tuple("SunOS", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kSolaris),
        std::make_tuple("OS/390", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kZOs),
        std::make_tuple("UNKNOWN", "UNKNOWN")
    )
);
