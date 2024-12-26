#include "os_utils.h"

#include <string_view>
#include <utility>

#if !defined(DISABLE_MPH) && \
    ((defined(__clang__) && __clang_major__ < 15) || (defined(__GNUC__) && __GNUC__ < 12) || defined(__MINGW32__))
#    define DISABLE_MPH
#endif

#if defined(__SIZEOF_INT128__) && __SIZEOF_INT128__ && __cplusplus >= 202002L && !defined(DISABLE_MPH)
#    include <array>
#    include <mph>
#    define USE_MPH
#    define CONTAINER           std::array
#    define CONTAINER_CONSTEXPR constexpr
#else
#    include <unordered_map>
#    define CONTAINER           std::unordered_map
#    define CONTAINER_CONSTEXPR const
#endif

#include <opentelemetry/version.h>

#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
#    include <opentelemetry/sdk/resource/semantic_conventions.h>
#else
#    include <opentelemetry/semconv/incubating/host_attributes.h>
#    include <opentelemetry/semconv/incubating/os_attributes.h>
#    include <opentelemetry/semconv/schema_url.h>
#endif

using std::literals::operator""sv;

std::string get_host_arch(std::string_view machine)
{
#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
    using namespace ::opentelemetry::sdk::resource::SemanticConventions::HostArchValues;
#else
    using namespace ::opentelemetry::semconv::host::HostArchValues;
#endif

    static CONTAINER_CONSTEXPR auto machine_to_arch = CONTAINER{
        std::pair{"x86_64"sv, kAmd64},
        std::pair{"amd64"sv, kAmd64},
        std::pair{"aarch64"sv, kArm64},
        std::pair{"arm64"sv, kArm64},
        std::pair{"arm"sv, kArm32},
        std::pair{"armv7l"sv, kArm32},
        std::pair{"ppc"sv, kPpc32},
        std::pair{"ppc64"sv, kPpc64},
        std::pair{"ppc64le"sv, kPpc64},
        std::pair{"s390x"sv, kS390x},
        std::pair{"i386"sv, kX86},
        std::pair{"i486"sv, kX86},
        std::pair{"i586"sv, kX86},
        std::pair{"i686"sv, kX86},
        std::pair{"x86"sv, kX86},
        std::pair{"i86pc"sv, kX86},
        std::pair{"x86pc"sv, kX86},
        std::pair{"ia64"sv, kIa64}
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
#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
    using namespace ::opentelemetry::sdk::resource::SemanticConventions::OsTypeValues;
#else
    using namespace ::opentelemetry::semconv::os::OsTypeValues;
#endif

    static CONTAINER_CONSTEXPR auto os_to_type = CONTAINER{
        std::pair{"Linux"sv, kLinux},
        std::pair{"Windows_NT"sv, kWindows},
        std::pair{"DragonFly"sv, kDragonflybsd},
        std::pair{"FreeBSD"sv, kFreebsd},
        std::pair{"HP-UX"sv, kHpux},
        std::pair{"AIX"sv, kAix},
        std::pair{"Darwin"sv, kDarwin},
        std::pair{"NetBSD"sv, kNetbsd},
        std::pair{"OpenBSD"sv, kOpenbsd},
        std::pair{"SunOS"sv, kSolaris},
        std::pair{"OS/390"sv, kZOs}
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
        return kWindows;
    }

    return {os.data(), os.length()};
}
