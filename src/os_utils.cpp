#include "os_utils.h"

#include <array>
#include <string_view>
#include <utility>

#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ && __cplusplus >= 202002L
#    include <mph>
#    define USE_MPH
#    define CONTAINER           std::array
#    define CONTAINER_CONSTEXPR constexpr
#else
#    include <unordered_map>
#    define CONTAINER           std::unordered_map
#    define CONTAINER_CONSTEXPR const
#endif

#include <opentelemetry/sdk/resource/semantic_conventions.h>

using std::literals::operator""sv;

std::string get_host_arch(std::string_view machine)
{
    static CONTAINER_CONSTEXPR auto machine_to_arch = CONTAINER{
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

#ifdef USE_MPH
    if (const auto arch = mph::find<machine_to_arch>(machine); arch) {
        return arch.value;
    }
#else
    if (const auto it = machine_to_arch.find(machine); it != machine_to_arch.end()) {
        return it->second;
    }
#endif

    return {machine.data(), machine.length()};
}

std::string get_os_type(std::string_view os)
{
    static CONTAINER_CONSTEXPR auto os_to_type = CONTAINER{
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

#ifdef USE_MPH
    if (const auto type = mph::find<os_to_type>(os); type) {
        return type.value;
    }
#else
    if (const auto it = os_to_type.find(os); it != os_to_type.end()) {
        return it->second;
    }
#endif

    if (os.starts_with("CYGWIN") || os.starts_with("MINGW") || os.starts_with("MSYS")) {
        return opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows;
    }

    return {os.data(), os.length()};
}
