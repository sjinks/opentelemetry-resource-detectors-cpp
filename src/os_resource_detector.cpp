#include "opentelemetry/resource/wwa/os_resource_detector.h"

#include <string>
#include <string_view>

#ifndef _WIN32
#    include <sys/utsname.h>
#else
#    include <array>

#    include <windows.h>

#    include "tstring.h"
#endif

#include <opentelemetry/sdk/resource/semantic_conventions.h>

#include "os_utils.h"

namespace wwa::opentelemetry::resource {

::opentelemetry::sdk::resource::Resource os_resource_detector::Detect()
{
    ::opentelemetry::sdk::resource::ResourceAttributes attrs;

#ifndef _WIN32
    utsname info{};
    if (uname(&info) == -1) {
        return ::opentelemetry::sdk::resource::Resource::GetEmpty();
    }

    // NOLINTBEGIN(*-bounds-array-to-pointer-decay) -- we have no control over the system interface
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kHostArch]  = get_host_arch(info.machine);
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kHostName]  = std::string(info.nodename);
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kOsType]    = get_os_type(info.sysname);
    attrs[::opentelemetry::sdk::resource::SemanticConventions::kOsVersion] = std::string(info.release);
    // NOLINTEND(*-bounds-array-to-pointer-decay)
#else
    SYSTEM_INFO si{};
    GetSystemInfo(&si);
    switch (si.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            attrs[::opentelemetry::sdk::resource::SemanticConventions::kHostArch] =
                std::string(::opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kAmd64);
            break;

        case PROCESSOR_ARCHITECTURE_INTEL:
            attrs[::opentelemetry::sdk::resource::SemanticConventions::kHostArch] =
                std::string(::opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kX86);
            break;

        case PROCESSOR_ARCHITECTURE_ARM64:
            attrs[::opentelemetry::sdk::resource::SemanticConventions::kHostArch] =
                std::string(::opentelemetry::sdk::resource::SemanticConventions::HostArchValues::kArm64);
            break;

        default:
            break;
    }

    std::array<TCHAR, MAX_COMPUTERNAME_LENGTH + 1> buf;
    if (auto size = static_cast<DWORD>(buf.size()); GetComputerName(buf.data(), &size)) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kHostName] = convert(buf.data());
    }

    attrs[::opentelemetry::sdk::resource::SemanticConventions::kOsType] =
        std::string(::opentelemetry::sdk::resource::SemanticConventions::OsTypeValues::kWindows);

    OSVERSIONINFO osvi{};
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    if (GetVersionEx(&osvi)) {
        attrs[::opentelemetry::sdk::resource::SemanticConventions::kOsVersion] =
            std::to_string(osvi.dwMajorVersion)
                .append(".")
                .append(std::to_string(osvi.dwMinorVersion))
                .append(" (Build ")
                .append(std::to_string(osvi.dwBuildNumber))
                .append(")");
    }
#endif

    return ::opentelemetry::sdk::resource::Resource::Create(
        attrs, ::opentelemetry::sdk::resource::SemanticConventions::kSchemaUrl
    );
}

}  // namespace wwa::opentelemetry::resource
