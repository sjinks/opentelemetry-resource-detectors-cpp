#include <tuple>

#include <gtest/gtest.h>

#include <opentelemetry/version.h>
#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
#    include <opentelemetry/sdk/resource/semantic_conventions.h>
#else
#    include <opentelemetry/semconv/incubating/host_attributes.h>
#endif

#include "os_utils.h"

class GetHostArchTest : public testing::TestWithParam<std::tuple<std::string, std::string>> {};

#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
using namespace ::opentelemetry::sdk::resource::SemanticConventions::HostArchValues;
#else
using namespace ::opentelemetry::semconv::host::HostArchValues;
#endif

TEST_P(GetHostArchTest, get_host_arch)
{
    std::string input;
    std::string expected;
    std::tie(input, expected) = GetParam();

    auto actual = get_host_arch(input);
    EXPECT_EQ(actual, expected);
}

INSTANTIATE_TEST_SUITE_P(
    Table,
    GetHostArchTest,
    testing::Values(
        std::make_tuple("x86_64", kAmd64),
        std::make_tuple("amd64", kAmd64),
        std::make_tuple("aarch64", kArm64),
        std::make_tuple("arm64", kArm64),
        std::make_tuple("arm", kArm32),
        std::make_tuple("armv7l", kArm32),
        std::make_tuple("ppc", kPpc32),
        std::make_tuple("ppc64", kPpc64),
        std::make_tuple("ppc64le", kPpc64),
        std::make_tuple("s390x", kS390x),
        std::make_tuple("i386", kX86),
        std::make_tuple("i486", kX86),
        std::make_tuple("i586", kX86),
        std::make_tuple("i686", kX86),
        std::make_tuple("x86", kX86),
        std::make_tuple("i86pc", kX86),
        std::make_tuple("x86pc", kX86),
        std::make_tuple("ia64", kIa64),
        std::make_tuple("UNKNOWN", "UNKNOWN")
    )
);
