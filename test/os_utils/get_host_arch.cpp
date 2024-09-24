#include <tuple>

#include <gtest/gtest.h>
#include <opentelemetry/sdk/resource/semantic_conventions.h>

#include "os_utils.h"

class GetHostArchTest : public testing::TestWithParam<std::tuple<std::string, std::string>> {};

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
        std::make_tuple("x86_64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kAmd64),
        std::make_tuple("amd64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kAmd64),
        std::make_tuple("aarch64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm64),
        std::make_tuple("arm64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm64),
        std::make_tuple("arm", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm32),
        std::make_tuple("armv7l", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm32),
        std::make_tuple("ppc", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kPpc32),
        std::make_tuple("ppc64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kPpc64),
        std::make_tuple("ppc64le", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kPpc64),
        std::make_tuple("s390x", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kS390x),
        std::make_tuple("i386", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86),
        std::make_tuple("i486", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86),
        std::make_tuple("i586", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86),
        std::make_tuple("i686", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86),
        std::make_tuple("x86", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86),
        std::make_tuple("i86pc", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86),
        std::make_tuple("x86pc", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86),
        std::make_tuple("ia64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kIa64),
        std::make_tuple("UNKNOWN", "UNKNOWN")
    )
);
