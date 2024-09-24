#include "os_utils.h"

#include <unordered_map>

#include <opentelemetry/sdk/resource/semantic_conventions.h>

std::string get_host_arch(std::string_view machine)
{
    static const std::unordered_map<std::string_view, const char*> machine_to_arch{
        {"x86_64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kAmd64},
        {"amd64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kAmd64},
        {"aarch64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm64},
        {"arm64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm64},
        {"arm", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm32},
        {"armv7l", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm32},
        {"ppc", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kPpc32},
        {"ppc64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kPpc64},
        {"ppc64le", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kPpc64},
        {"s390x", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kS390x},
        {"i386", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
        {"i486", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
        {"i586", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
        {"i686", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
        {"x86", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
        {"i86pc", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
        {"x86pc", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86},
        {"ia64", opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kIa64}
    };

    if (const auto it = machine_to_arch.find(machine); it != machine_to_arch.end()) {
        return it->second;
    }

    return {machine.data(), machine.length()};
}

std::string get_os_type(std::string_view os)
{
    static const std::unordered_map<std::string_view, const char*> os_to_type{
        {"Linux", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kLinux},
        {"Windows_NT", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows},
        {"CYGWIN_NT-5.1", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows},
        {"CYGWIN_NT-6.1", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows},
        {"CYGWIN_NT-6.1-WOW64", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows},
        {"CYGWIN_NT-10.0", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows},
        {"MINGW32_NT-6.1", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows},
        {"MSYS_NT-6.1", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows},
        {"MINGW32_NT-10.0", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows},
        {"MSYS_NT-10.0", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows},
        {"DragonFly", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kDragonflybsd},
        {"FreeBSD", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kFreebsd},
        {"HP-UX", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kHpux},
        {"AIX", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kAix},
        {"Darwin", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kDarwin},
        {"NetBSD", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kNetbsd},
        {"OpenBSD", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kOpenbsd},
        {"SunOS", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kSolaris},
        {"OS/390", opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kZOs}
    };

    if (const auto it = os_to_type.find(os); it != os_to_type.end()) {
        return it->second;
    }

    return {os.data(), os.length()};
}
