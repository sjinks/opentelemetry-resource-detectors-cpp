#include <tuple>

#include <gtest/gtest.h>

#include <opentelemetry/version.h>

#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
#    include <opentelemetry/sdk/resource/semantic_conventions.h>
#else
#    include <opentelemetry/semconv/incubating/os_attributes.h>
#endif

#include "os_utils.h"

class GetOsTypeTest : public testing::TestWithParam<std::tuple<std::string, std::string>> {};

#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
using namespace ::opentelemetry::sdk::resource::SemanticConventions::OsTypeValues;
#else
using namespace ::opentelemetry::semconv::os::OsTypeValues;
#endif

TEST_P(GetOsTypeTest, get_os_type)
{
    std::string input;
    std::string expected;
    std::tie(input, expected) = GetParam();

    auto actual = get_os_type(input);
    EXPECT_EQ(actual, expected);
}

#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 22
static constexpr const char* z_os = kZOs;
#else
static constexpr const char* z_os = kZos;
#endif

INSTANTIATE_TEST_SUITE_P(
    Table,
    GetOsTypeTest,
    testing::Values(
        std::make_tuple("Linux", kLinux),
        std::make_tuple("Windows_NT", kWindows),
        std::make_tuple("CYGWIN_NT-5.1", kWindows),
        std::make_tuple("CYGWIN_NT-6.1", kWindows),
        std::make_tuple("CYGWIN_NT-6.1-WOW64", kWindows),
        std::make_tuple("CYGWIN_NT-10.0", kWindows),
        std::make_tuple("MINGW32_NT-6.1", kWindows),
        std::make_tuple("MSYS_NT-6.1", kWindows),
        std::make_tuple("MINGW32_NT-10.0", kWindows),
        std::make_tuple("MSYS_NT-10.0", kWindows),
        std::make_tuple("DragonFly", kDragonflybsd),
        std::make_tuple("FreeBSD", kFreebsd),
        std::make_tuple("HP-UX", kHpux),
        std::make_tuple("AIX", kAix),
        std::make_tuple("Darwin", kDarwin),
        std::make_tuple("NetBSD", kNetbsd),
        std::make_tuple("OpenBSD", kOpenbsd),
        std::make_tuple("SunOS", kSolaris),
        std::make_tuple("OS/390", z_os),
        std::make_tuple("UNKNOWN", "UNKNOWN")
    )
);
