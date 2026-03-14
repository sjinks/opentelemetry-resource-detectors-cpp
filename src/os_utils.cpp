#include "os_utils.h"

#include <string_view>
#include <utility>

#include <unordered_map>

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

    static const std::unordered_map<std::string_view, std::string_view> machine_to_arch = {
        {"x86_64"sv, kAmd64},
        {"amd64"sv, kAmd64},
        {"aarch64"sv, kArm64},
        {"arm64"sv, kArm64},
        {"arm"sv, kArm32},
        {"armv7l"sv, kArm32},
        {"ppc"sv, kPpc32},
        {"ppc64"sv, kPpc64},
        {"ppc64le"sv, kPpc64},
        {"s390x"sv, kS390x},
        {"i386"sv, kX86},
        {"i486"sv, kX86},
        {"i586"sv, kX86},
        {"i686"sv, kX86},
        {"x86"sv, kX86},
        {"i86pc"sv, kX86},
        {"x86pc"sv, kX86},
        {"ia64"sv, kIa64}
    };

    if (const auto it = machine_to_arch.find(machine); it != machine_to_arch.end()) {
        return std::string(it->second);
    }

    return {machine.data(), machine.length()};
}

std::string get_os_type(std::string_view os)
{
#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 18
    using namespace ::opentelemetry::sdk::resource::SemanticConventions::OsTypeValues;
#else
    using namespace ::opentelemetry::semconv::os::OsTypeValues;
#endif

    static const std::unordered_map<std::string_view, std::string_view> os_to_type = {
        {"Linux"sv, kLinux},
        {"Windows_NT"sv, kWindows},
        {"DragonFly"sv, kDragonflybsd},
        {"FreeBSD"sv, kFreebsd},
        {"HP-UX"sv, kHpux},
        {"AIX"sv, kAix},
        {"Darwin"sv, kDarwin},
        {"NetBSD"sv, kNetbsd},
        {"OpenBSD"sv, kOpenbsd},
        {"SunOS"sv, kSolaris},
#if OPENTELEMETRY_VERSION_MAJOR == 1 && OPENTELEMETRY_VERSION_MINOR < 22
        {"OS/390"sv, kZOs}
#else
        {"OS/390"sv, kZos}
#endif
    };

    if (const auto it = os_to_type.find(os); it != os_to_type.end()) {
        return std::string(it->second);
    }

    if (os.starts_with("CYGWIN") || os.starts_with("MINGW") || os.starts_with("MSYS")) {
        return kWindows;
    }

    return {os.data(), os.length()};
}
