#include "os_utils.h"

#include <array>
#include <string_view>
#include <utility>

#include <mph>
#include <opentelemetry/sdk/resource/semantic_conventions.h>

using std::literals::operator""sv;

namespace {

constexpr auto machine_to_arch = std::array{
    std::pair{"x86_64"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kAmd64},
    std::pair{"amd64"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kAmd64},
    std::pair{"aarch64"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm64},
    std::pair{"arm64"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm64},
    std::pair{"arm"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm32},
    std::pair{"armv7l"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm32},
    std::pair{"ppc"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kPpc32},
    std::pair{"ppc64"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kPpc64},
    std::pair{"ppc64le"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kPpc64},
    std::pair{"s390x"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kS390x},
    std::pair{"i386"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
    std::pair{"i486"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
    std::pair{"i586"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
    std::pair{"i686"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
    std::pair{"x86"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
    std::pair{"i86pc"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
    std::pair{"x86pc"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
    std::pair{"ia64"sv, opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kIa64}
};

constexpr auto os_to_type = std::array{
    std::pair{"Linux"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kLinux},
    std::pair{"Windows_NT"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows},
    std::pair{"DragonFly"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kDragonflybsd},
    std::pair{"FreeBSD"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kFreebsd},
    std::pair{"HP-UX"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kHpux},
    std::pair{"AIX"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kAix},
    std::pair{"Darwin"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kDarwin},
    std::pair{"NetBSD"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kNetbsd},
    std::pair{"OpenBSD"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kOpenbsd},
    std::pair{"SunOS"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kSolaris},
    std::pair{"OS/390"sv, opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kZOs}
};

}  // namespace

std::string get_host_arch(std::string_view machine)
{
    if (const auto arch = mph::find<machine_to_arch>(machine); arch) {
        return arch.value;
    }

    return {machine.data(), machine.length()};
}

std::string get_os_type(std::string_view os)
{
    if (const auto type = mph::find<os_to_type>(os); type) {
        return type.value;
    }

    if (os.starts_with("CYGWIN") || os.starts_with("MINGW") || os.starts_with("MSYS")) {
        return opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows;
    }

    return {os.data(), os.length()};
}
